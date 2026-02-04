cmake_minimum_required(VERSION 3.20)
project(sycl_hello LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sycl_hello main.cpp)

# DPC++ uses Clang with -fsycl for both compile and link.
target_compile_options(sycl_hello PRIVATE -fsycl)
target_link_options(sycl_hello PRIVATE -fsycl)
