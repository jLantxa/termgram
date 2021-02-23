cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(termgram VERSION 1.0 LANGUAGES CXX)

find_package(Td 1.7.0 REQUIRED)

set(SRC ${CMAKE_SOURCE_DIR}/src)
set(TEST ${CMAKE_SOURCE_DIR}/test)

add_executable(termgram)
set(CMAKE_CXX_FLAGS "-O2 -Wall -Werror")
set_property(TARGET termgram PROPERTY CXX_STANDARD 20)
target_link_libraries(termgram PRIVATE Td::TdStatic ncursesw)
include_directories(termgram
    ${CMAKE_SOURCE_DIR}/include
)
target_sources(termgram PRIVATE
    ${SRC}/TermgramClient.cpp
    ${SRC}/main.cpp
)
