cmake_minimum_required(VERSION 3.12)
project(renderer)

set(CMAKE_CXX_STANDARD 17)

add_executable(renderer main.cpp vector.h ray.h image.h hit.h scene.h object.h)