//
// Created by john on 2/13/20.
//

#ifndef CPATTRIBUTES_CP_ATTRIBUTES_HPP
#define CPATTRIBUTES_CP_ATTRIBUTES_HPP

#include <cppast/libclang_parser.hpp> // libclang_parser
#include <cppast/cpp_enum.hpp> // cpp_enum
#include <cppast/cpp_class.hpp> // cpp_class
#include <cppast/visitor.hpp>  // visit()

#include <initializer_list>
#include <filesystem>
#include <functional>
#include <iterator>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

namespace attributes {
    namespace {
        using function_type = std::function<void(std::vector<std::string> const &,
                                                 cppast::libclang_compile_config const &)>;
        using specific_parser = std::function<void(const cppast::cpp_class &, std::stringstream &)>;

        std::vector<std::string>
        get_all_files(std::string_view base, std::initializer_list<std::string> valid_extensions) {
            std::vector<std::string> result{};
            for (auto const &file : std::filesystem::recursive_directory_iterator(base)) {
                auto const &extension = file.path().extension().string();
                if (std::find(valid_extensions.begin(), valid_extensions.end(), extension) != valid_extensions.end()) {
                    result.emplace_back(file.path().string());
                }
            }
            return result;
        }

        inline void class_call(const cppast::cpp_entity &e, std::stringstream &output, specific_parser const &func) {
            func(dynamic_cast<const cppast::cpp_class &>(e), output);
        }
    }

    namespace json {
        inline void generate_all(const cppast::cpp_class &e, std::stringstream &output) {
            output << "inline nlohmann::json to_json(" << e.name() << " const& object) {"
                   << "nlohmann::json output{};";
//            for (auto const& member : e)
//            {
//
//            }
        }

        inline void generate_function(cppast::cpp_file &file, std::stringstream &output) {
            cppast::visit(file,
                          [&](const cppast::cpp_entity &e) {
                              return e.kind() == cppast::cpp_entity_kind::class_t && !cppast::is_templated(e) &&
                                     cppast::has_attribute(e, "attribute::json");
                          }, [&](const cppast::cpp_entity &e, cppast::visitor_info info) {
                        if (info.is_new_entity()) {
                            class_call(e, output, json::generate_all);
                        }
                        return true;
                    });
        }

        inline void
        generate_json(std::vector<std::string> const &files, cppast::libclang_compile_config const &config) {
            cppast::libclang_parser parser;
            cppast::cpp_entity_index idx;
            std::stringstream output{};
            output << "#ifndef HEADER_JSON_ATTRIBUTE\n#define HEADER_JSON_ATTRIBUTE\n#endif\n\nnamespace json {\n";

            for (auto const &file : files) {
                std::cout << "Generating to_json and from_json for file: " << file << std::endl;
                std::unique_ptr<cppast::cpp_file> parsed_file = parser.parse(idx, std::string(file), config);
                json::generate_function(*parsed_file, output);
            }

            output << "\n} // namespace json";
            std::cout << output.str();
        }


    }


    int run(std::string_view base, std::initializer_list<std::string> extensions,
            std::initializer_list<std::string> types,
            cppast::libclang_compile_config config) {
        static std::unordered_map<std::string, function_type> functions = {
                {"json", json::generate_json}
        };

        std::vector<function_type> funcs{};

        for (auto const &each : types) {
            if (functions.find(each) == functions.end()) {
                continue;
            }
            funcs.push_back(functions[each]);
        }

        auto all_files = get_all_files(base, extensions);

        std::for_each(funcs.begin(), funcs.end(), [&](auto const &func) {
            func(all_files, config);
        });

        return 0;
    }
} // namespace attributes

#endif //CPATTRIBUTES_CP_ATTRIBUTES_HPP
