cmake_minimum_required(VERSION 3.10.0)
project(learnopengl VERSION 0.1.0 LANGUAGES C CXX)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)

add_executable(learnopengl main.cpp "glad.c")

include_directories( SYSTEM "include" )

target_link_libraries(learnopengl)

target_link_libraries(learnopengl glfw)
target_link_libraries(learnopengl OpenGL::GL)