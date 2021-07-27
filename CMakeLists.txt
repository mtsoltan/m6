cmake_minimum_required(VERSION 3.13)
set(CMAKE_CXX_STANDARD 17)

project(m6)

add_library(cfiles Tokenizer.cc LiteralProcessor.cc TokenTypeChecker.cc Token.cc)

target_include_directories(cfiles PUBLIC include)

add_executable(m6 main.cc errors.cc)

target_link_libraries (m6 cfiles)

install (TARGETS cfiles DESTINATION bin)
install (TARGETS m6 DESTINATION bin)
