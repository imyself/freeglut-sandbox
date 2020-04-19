cmake_minimum_required(VERSION 3.15)
project(opengl_test C)
set(CMAKE_C_COMPILER "clang")
SET (CMAKE_C_FLAGS                "-g -Wall -lGL -lGLU -lglut -lm -lGLEW -std=c99")

#set(FREEGLUT_FLAGS "-lGL -lGLU -lglut -lm")
#set(CMAKE_C_STANDARD 99)
#set(CMAKE_CXX_FLAGS "-lGL -lGLU -lglut -lm -lGLEW")
#add_link_options(-lGLEW)

#message("The value of FREEGLUT_FLAGS is ${FREEGLUT_FLAGS}")

add_executable(opengl_test main.c
               shaderprogram.h shaderprogram.c
               drawable.h drawable.c
               linear_alg.h linear_alg.c
        camera.h camera.c)