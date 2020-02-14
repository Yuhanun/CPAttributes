#ifndef HEADER_JSON_ATTRIBUTE
#define HEADER_JSON_ATTRIBUTE

#include <nlohmann/json.hpp>
#include "/home/john/git/CPAttributes/./test/json_tests.hpp"



namespace test{
inline void to_json(nlohmann::json& output, Example const& object);
inline void from_json(nlohmann::json const& data, Example& output);
}

namespace test{
inline void to_json(nlohmann::json& output, Another const& object);
inline void from_json(nlohmann::json const& data, Another& output);
}

namespace test{
inline void to_json(nlohmann::json& output, Nested const& object);
inline void from_json(nlohmann::json const& data, Nested& output);
}


namespace test{
inline void to_json(nlohmann::json& output, Example const& object) {
	output["name"] = object.name;
	output["example"] = object.example;
}
inline void from_json(nlohmann::json const& data, Example& output) {
		output.name = data.at("name");
		output.example = data.at("example");
}
}

namespace test{
inline void to_json(nlohmann::json& output, Another const& object) {
	output["another"] = object.another;
}
inline void from_json(nlohmann::json const& data, Another& output) {
		output.another = data.at("another");
}
}

namespace test{
inline void to_json(nlohmann::json& output, Nested const& object) {
	output["first"] = object.first;
	output["second"] = object.second;
}
inline void from_json(nlohmann::json const& data, Nested& output) {
		output.first = data.at("first");
		output.second = data.at("second");
}
}

#endif // HEADER_JSON_ATTRIBUTE