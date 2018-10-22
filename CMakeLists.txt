cmake_minimum_required(VERSION 3.12)
project(renderer)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -Wall -Wextra -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-fopenmp -DNDEBUG -O3")
#set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mtune=native -march=native -mfpmath=both")

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_FILES main.cpp vector.h image.h shape.h hit.h ray.h scene.h camera.h random.h material.h)
add_executable(renderer ${SOURCE_FILES})