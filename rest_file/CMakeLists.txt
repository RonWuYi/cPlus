project(rest)

add_executable(rest src/myqueue_ut.cpp src/myqueue.h)
target_link_libraries(rest gtest_main)
add_test(NAME example_test COMMAND rest)