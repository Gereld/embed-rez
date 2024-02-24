cmake_minimum_required(VERSION 3.14)

project(
    embed-rez
    VERSION 0.1.0
    DESCRIPTION "Embed files as C++ code."
    LANGUAGES CXX)

add_executable(
    embed-rez 
    src/main.cpp
    src/path/path.cpp)

set_property(
    TARGET embed-rez 
    PROPERTY OUTPUT_NAME embed-rez)

target_compile_features(
    embed-rez 
    PRIVATE cxx_std_20)

target_include_directories(
    embed-rez ${warning_guard}
    PRIVATE "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/3rdparty/pugixml/src>")

target_compile_definitions(
    embed-rez 
    PRIVATE _UNICODE UNICODE)
