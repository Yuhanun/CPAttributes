cmake_minimum_required(VERSION 3.15)
project(CPAttributes)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(cppast)

add_executable(CPAttributes main.cpp)

target_include_directories(CPAttributes
        PRIVATE include
        )

if(MSVC)
    target_compile_options(CPAttributes PRIVATE /W4 /WX)
else()
    target_compile_options(CPAttributes PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

target_link_libraries(CPAttributes
        PRIVATE cppast
        )


