//
// Created by john on 2/13/20.
//

#ifndef CPATTRIBUTES_JSON_TESTS_HPP
#define CPATTRIBUTES_JSON_TESTS_HPP

#include <string>

namespace test {
    struct [[attribute::json]] Example {
        std::string name;
        bool example;
    };

    struct [[attribute::json]] Another {
        std::string another;
    };

    struct [[attribute::json]] Nested {
        Another first;
        Example second;
    };
}  // namespace test

#endif  //CPATTRIBUTES_JSON_TESTS_HPP
