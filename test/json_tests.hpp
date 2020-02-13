//
// Created by john on 2/13/20.
//

#ifndef CPATTRIBUTES_JSON_TESTS_HPP
#define CPATTRIBUTES_JSON_TESTS_HPP

#include <string>

namespace test {
    class [[attribute::json]] Example {
        std::string name;
        bool example;
    };

    struct [[attribute::json]] Another {
        std::string another;
    };
}  // namespace test

#endif  //CPATTRIBUTES_JSON_TESTS_HPP
