#ifndef HEADER_JSON_ATTRIBUTE
#define HEADER_JSON_ATTRIBUTE

#include <nlohmann/json.hpp>

namespace json {

	inline nlohmann::json to_json(Example const& object);
	inline Example example_from_json(nlohmann::json const& data);
	inline nlohmann::json to_json(Another const& object);
	inline Another another_from_json(nlohmann::json const& data);
	inline nlohmann::json to_json(Nested const& object);
	inline Nested nested_from_json(nlohmann::json const& data);

	inline nlohmann::json to_json(Example const& object) {
		nlohmann::json output{};
		output["name"] = name;
		output["example"] = example;
		return output;
	}

	inline Example example_from_json(nlohmann::json const& data) {
		Example output;
		output.name = data["name"];
		output.example = data["example"];
		return output;
	}

	inline nlohmann::json to_json(Another const& object) {
		nlohmann::json output{};
		output["another"] = another;
		return output;
	}

	inline Another another_from_json(nlohmann::json const& data) {
		Another output;
		output.another = data["another"];
		return output;
	}

	inline nlohmann::json to_json(Nested const& object) {
		nlohmann::json output{};
		output["first"] = to_json(object.first);
		output["second"] = to_json(object.second);
		return output;
	}

	inline Nested nested_from_json(nlohmann::json const& data) {
		Nested output;
		output.first = test::another_from_json(data["first"]);
		output.second = test::example_from_json(data["second"]);
		return output;
	}

} // namespace json
#endif // HEADER_JSON_ATTRIBUTE