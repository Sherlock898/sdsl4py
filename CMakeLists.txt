cmake_minimum_required(VERSION 3.4...3.18)
project(sdsl4py)

add_subdirectory(sdsl-lite)
add_subdirectory(pybind11)

target_compile_options(sdsl PRIVATE -fPIC)
pybind11_add_module(sdsl4py sdsl4py/sdsl4py.cpp)
target_include_directories(sdsl4py PRIVATE sdsl-lite/include)
target_link_libraries(sdsl4py PRIVATE sdsl)