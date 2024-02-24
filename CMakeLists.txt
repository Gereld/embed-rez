cmake_minimum_required(VERSION 3.14)

project(
    embed-rez
    VERSION 0.1.0
    DESCRIPTION "Shows how to use embed-rez."
    LANGUAGES CXX)

add_subdirectory(embed-rez)
add_subdirectory(example)

add_custom_command(
    OUTPUT ${CMAKE_SOURCE_DIR}/example/res.inc
    COMMAND embed-rez ${CMAKE_SOURCE_DIR}/example/res.xml ${CMAKE_SOURCE_DIR}/example/res.inc
    MAIN_DEPENDENCY ${CMAKE_SOURCE_DIR}/example/res.xml
    DEPENDS embed-rez
    COMMENT "Building resources...")

add_custom_target(resources DEPENDS ${CMAKE_SOURCE_DIR}/example/res.inc)

add_dependencies(example resources)

set_property(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} 
    PROPERTY VS_STARTUP_PROJECT example)
