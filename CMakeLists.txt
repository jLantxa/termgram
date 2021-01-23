cmake_minimum_required(VERSION 3.1 FATAL_ERROR)

project(termgram VERSION 1.0 LANGUAGES CXX)

find_package(Td 1.7.0 REQUIRED)

add_executable(termgram src/main.cpp src/TermgramClient.cpp)
set(CMAKE_CXX_FLAGS "-O2 -Wall -Werror")
include_directories(include)
target_link_libraries(termgram PRIVATE Td::TdStatic)
set_property(TARGET termgram PROPERTY CXX_STANDARD 20)
