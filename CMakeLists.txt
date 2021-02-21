cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(termgram VERSION 1.0 LANGUAGES CXX)

find_package(Td 1.7.0 REQUIRED)

add_executable(termgram)
set(CMAKE_CXX_FLAGS "-O2 -Wall -Werror")
set_property(TARGET termgram PROPERTY CXX_STANDARD 20)

target_link_libraries(termgram PRIVATE Td::TdStatic curses)

include_directories(include)
target_sources(termgram PRIVATE
    src/main.cpp
    src/TermgramClient.cpp
)
