cmake_minimum_required(VERSION 3.16)

project(AntAsm VERSION 1.0.0)

file(GLOB antasm_src CONFIGURE_DEPENDS "AntAsm/*.h" "AntAsm/*.c")
add_executable(AntAsm ${antasm_src})
target_compile_options(AntAsm PRIVATE -Wall -Wextra -Wpedantic -Werror)
target_link_libraries(AntAsm m)
