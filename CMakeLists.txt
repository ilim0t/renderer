cmake_minimum_required(VERSION 3.12)
project(renderer)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
set(CMAKE_CXX_STANDARD 17)

add_executable(renderer main.cpp vector.h image.h shape.h hit.h ray.h scene.h camera.h)
