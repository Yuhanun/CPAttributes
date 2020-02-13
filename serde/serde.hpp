#ifndef HEADER_JSON_ATTRIBUTE
#define HEADER_JSON_ATTRIBUTE

#include <nlohmann/json.hpp>

namespace json {

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

} // namespace json
#endif // HEADER_JSON_ATTRIBUTE