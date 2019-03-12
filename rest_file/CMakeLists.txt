project(rest)

add_executable(rest src/logger.hpp src/3.cpp src/derived.hpp src/derived.cpp src/main1.cpp)
#target_link_libraries(rest gtest_main)
#add_test(NAME example_test COMMAND rest)