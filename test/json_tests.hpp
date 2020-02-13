//
// Created by john on 2/13/20.
//

#ifndef CPATTRIBUTES_JSON_TESTS_HPP
#define CPATTRIBUTES_JSON_TESTS_HPP

namespace attributes {
    struct [[attributes::json]] Example {
        std::string name;
        bool example;
    };
}

#endif //CPATTRIBUTES_JSON_TESTS_HPP
