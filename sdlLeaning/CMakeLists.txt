#cmake_minimum_required(VERSION 3.8.2)
project(sdlLeaning)
#set(CMAKE_CXX_STANDARD 11)
#
#set(SDL2_PATH "")
add_executable(sdlLeaning src/normal001.cpp)

#target_link_libraries(sdlLeaning SDL2 SDL2main avutil avformat avcodec swscale swresample)
#target_link_libraries(sdlLeaning SDL2 SDL2main gtest_main)
target_link_libraries(sdlLeaning gtest_main)
add_test(NAME example_test COMMAND sdlLeaning)