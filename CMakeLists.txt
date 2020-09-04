cmake_minimum_required (VERSION 3.8)

project ("InformationSecurity")

add_executable (InformationSecurity "src/main.cpp" "src/crypto/lab1.hpp" "src/crypto/export.hpp" "src/crypto/common.hpp")

target_compile_features(InformationSecurity PRIVATE cxx_std_20)