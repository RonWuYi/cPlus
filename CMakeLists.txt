cmake_minimum_required(VERSION 3.8)
project(cPlus)

set(CMAKE_CXX_STANDARD 11)

#set(SOURCE_FILES main.cpp)
#add_executable(main1 ${SOURCE_FILES})

set(SOURCE_FILES strngbad.h stringbad.cpp vegnews.cpp)
add_executable(1 ${SOURCE_FILES})