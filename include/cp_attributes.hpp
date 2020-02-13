//
// Created by john on 2/13/20.
//

#ifndef CPATTRIBUTES_CP_ATTRIBUTES_HPP
#define CPATTRIBUTES_CP_ATTRIBUTES_HPP

#include <cppast/cpp_class.hpp>  // cpp_class
#include <cppast/cpp_enum.hpp>   // cpp_enum
#include <cppast/cpp_member_variable.hpp>
#include <cppast/libclang_parser.hpp>  // libclang_parser
#include <cppast/visitor.hpp>          // visit()
#include <filesystem>
#include <fstream>
#include <functional>
#include <initializer_list>
#include <iostream>
#include <iterator>
#include <string>
#include <vector>
#include <set>

namespace attributes {
    namespace {
        using function_type = std::function<void(std::vector<std::string> const &,
                                                 cppast::libclang_compile_config const &, std::string const &)>;
        using specific_parser = std::function<void(const cppast::cpp_class &, std::stringstream &,
                                                   std::stringstream &)>;

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

        inline void class_call(const cppast::cpp_entity &e, std::stringstream &output, std::stringstream &header,
                               specific_parser const &func) {
            func(dynamic_cast<const cppast::cpp_class &>(e), output, header);
        }
    }  // namespace

    namespace json {
        inline void generate_to_json(const cppast::cpp_class &e, std::stringstream &output, std::stringstream &header) {
            header << "\n\tinline void to_json(nlohmann::json& output, " << e.name() << " const& object);";
            output << "\n\tinline void to_json(nlohmann::json& output, " << e.name() << " const& object) {\n";

            for (auto const &each : e) {
                if (each.kind() == cppast::cpp_entity_kind::member_variable_t) {
                    output << "\t\toutput[\"" << each.name() << "\"] = object." << each.name() << ";\n";
                }
            }
            output << "\t}";
        }

        inline void
        generate_from_json(const cppast::cpp_class &e, std::stringstream &output, std::stringstream &header) {
            header << "\n\tinline void from_json(nlohmann::json const& data, " << e.name() << "& output);";
            output << "\n\tinline void from_json(nlohmann::json const& data, " << e.name() << "& output) {\n";

            for (auto const &each : e) {
                if (each.kind() == cppast::cpp_entity_kind::member_variable_t) {
                    auto const &field = dynamic_cast<cppast::cpp_member_variable const &>(each);
                    auto type_name = cppast::to_string(field.type());
                    output << "\t\toutput." << each.name() << " = data[\"" << each.name() << "\"];\n";
                }
            }
            output << "\t}";
        }

        inline void generate_all(const cppast::cpp_class &e, std::stringstream &output, std::stringstream &header) {
            generate_to_json(e, output, header);
            generate_from_json(e, output, header);
        }

        inline void generate_function(cppast::cpp_file &file, std::stringstream &output, std::stringstream &header) {
            cppast::visit(
                    file,
                    [&](const cppast::cpp_entity &) {
                        return true;
                    },
                    [&](const cppast::cpp_entity &e, cppast::visitor_info info) {
                        if (info.is_new_entity()) {
                            if (e.kind() == cppast::cpp_entity_kind::class_t) {
                                class_call(e, output, header, json::generate_all);
                            }
                            if (e.kind() == cppast::cpp_entity_kind::namespace_t) {
                                header << "\tusing namespace " << e.name() << ";\n";
                            }
                        }
                        return true;
                    });
        }

        inline void
        generate_json(std::vector<std::string> const &files, cppast::libclang_compile_config const &config, std::string const& filename) {
            cppast::libclang_parser parser;
            cppast::cpp_entity_index idx;
            std::stringstream output{};
            std::stringstream header{};
            std::ofstream output_file{filename};
            output_file << "#ifndef HEADER_JSON_ATTRIBUTE" << std::endl
                        << "#define HEADER_JSON_ATTRIBUTE\n\n"
                        << "#include <nlohmann/json.hpp>\n";
            for (auto const &each : files) {
                output_file << "#include \"" << each << "\"\n";
            }
            output_file << "\n\nnamespace json {\n";

            for (auto const &file : files) {
                std::cout << "Generating to_json and from_json for file: " << file << std::endl;
                std::unique_ptr<cppast::cpp_file> parsed_file = parser.parse(idx, std::string(file), config);
                json::generate_function(*parsed_file, output, header);
            }

            output_file << header.str() << "\n";
            output_file << output.str();
            output_file << "\n} // namespace json\n#endif // HEADER_JSON_ATTRIBUTE";
        }


    }  // namespace json


    int run(std::string_view base, std::initializer_list<std::string> extensions,
            std::initializer_list<std::string> types,
            cppast::libclang_compile_config config = cppast::libclang_compile_config(),
            std::string const &output_file = "serde.hpp",
            std::string const& folders = ".") {
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

        std::filesystem::create_directories(folders);

        std::for_each(funcs.begin(), funcs.end(), [&](auto const &func) {
            func(all_files, config, folders + "/" + output_file);
        });

        return 0;
    }
}  // namespace attributes

#endif  //CPATTRIBUTES_CP_ATTRIBUTES_HPP
