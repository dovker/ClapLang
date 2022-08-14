cmake_minimum_required(VERSION 3.5)
project(ClapLang)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB_RECURSE CLAPLANG_SRC_FILES "src/*.cpp" "src/*.h")

add_executable(ClapLang ${CLAPLANG_SRC_FILES})