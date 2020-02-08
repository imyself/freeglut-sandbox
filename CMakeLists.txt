cmake_minimum_required(VERSION 3.15)
project(opengl_test C)

set(FREEGLUT_FLAGS "-lGL -lGLU -lglut -lm")
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "${FREEGLUT_FLAGS}")

message("The value of FREEGLUT_FLAGS is ${FREEGLUT_FLAGS}")

add_executable(opengl_test main.c)