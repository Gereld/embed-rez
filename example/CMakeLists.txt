cmake_minimum_required(VERSION 3.14)

project(
    example
    VERSION 0.1.0
    DESCRIPTION "Shows how to use embed-rez."
    LANGUAGES CXX)

add_executable(
    example
    src/main.cpp
    src/res.cpp
    res.xml)

set_property(
    TARGET example 
    PROPERTY OUTPUT_NAME example)

target_compile_features(
    example 
    PRIVATE cxx_std_20)
