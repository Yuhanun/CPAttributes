#ifndef HEADER_JSON_ATTRIBUTE
#define HEADER_JSON_ATTRIBUTE

#include <nlohmann/json.hpp>
#include "../test/json_tests.hpp"


namespace json {
	using namespace test;

	void to_json(nlohmann::json& output, Example const& object);
	void from_json(nlohmann::json const& data, Example& output);
	void to_json(nlohmann::json& output, Another const& object);
	void from_json(nlohmann::json const& data, Another& output);
	void to_json(nlohmann::json& output, Nested const& object);
	void from_json(nlohmann::json const& data, Nested& output);

	void to_json(nlohmann::json& output, Example const& object) {
		output["name"] = object.name;
		output["example"] = object.example;
	}
	void from_json(nlohmann::json const& data, Example& output) {
		output.name = data["name"];
		output.example = data["example"];
	}
	void to_json(nlohmann::json& output, Another const& object) {
		output["another"] = object.another;
	}
	void from_json(nlohmann::json const& data, Another& output) {
		output.another = data["another"];
	}
	void to_json(nlohmann::json& output, Nested const& object) {
		output["first"] = object.first;
		output["second"] = object.second;
	}
	void from_json(nlohmann::json const& data, Nested& output) {
		output.first = data["first"];
		output.second = data["second"];
	}
} // namespace json
#endif // HEADER_JSON_ATTRIBUTE