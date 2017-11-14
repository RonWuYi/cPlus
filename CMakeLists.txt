cmake_minimum_required(VERSION 3.8)
project(untitled)

set(CMAKE_CXX_STANDARD 11)

#set(SOURCE_FILES main.cpp)
#add_executable(main1 ${SOURCE_FILES})

set(SOURCE_FILES stocks.cpp stock1.h usestock1.cpp stack.h stack.cpp stacker.cpp vect.h)
add_executable(1 ${SOURCE_FILES})