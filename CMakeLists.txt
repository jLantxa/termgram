cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(tgram VERSION 1.0 LANGUAGES CXX)

find_package(Td 1.7.0 REQUIRED)

set(SRC ${CMAKE_SOURCE_DIR}/src)
set(TEST ${CMAKE_SOURCE_DIR}/test)

add_executable(tgram)
set(CMAKE_CXX_FLAGS "-O2 -Wall -Werror")
set_property(TARGET tgram PROPERTY CXX_STANDARD 20)
target_link_libraries(tgram PRIVATE Td::TdStatic ncursesw)
include_directories(tgram
    ${CMAKE_SOURCE_DIR}/include
)
target_sources(tgram PRIVATE
    ${SRC}/TGramClient.cpp
    ${SRC}/main.cpp
)
add_executable(textbox-test)
set(CMAKE_CXX_FLAGS "-O2 -Wall -Werror")
set_property(TARGET textbox-test PROPERTY CXX_STANDARD 20)
target_link_libraries(textbox-test PRIVATE Td::TdStatic ncursesw)
target_sources(textbox-test PRIVATE
    ${SRC}/ui/TextBox.cpp
    ${TEST}/TextBoxTest.cpp
)
