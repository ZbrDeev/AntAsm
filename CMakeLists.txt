cmake_minimum_required(VERSION 3.16)

project(AntAsm VERSION 1.0.0)

file(GLOB antasm_src CONFIGURE_DEPENDS "AntAsm/*.h" "AntAsm/*.c")
add_executable(AntAsm ${antasm_src})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(AntAsm PRIVATE -Wall -Wextra -Wpedantic -Werror)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(AntAsm PRIVATE -Wall -Wextra -Wpedantic)
else()
    message(FATAL_ERROR "Incorrect CMAKE_BUILD_TYPE : ${CMAKE_BUILD_TYPE}")
endif()

target_link_libraries(AntAsm m)
