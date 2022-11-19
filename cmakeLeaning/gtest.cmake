include(FetchContent)
# FetchContent_Declare(benchmark
# GIT_REPOSITORY https://github.com/google/benchmark.git
# GIT_TAG v1.6.1
# )
FetchContent_Declare(GTest
GIT_REPOSITORY https://github.com/google/googletest.git
GIT_TAG release-1.12.0
)
FetchContent_MakeAvailable(GTest)
# add_library(GTest::Main gtest_main)

message(STATUS "gtest source dir is ${gtest_SOURCE_DIR}")
message(STATUS "gtest binary dir is ${gtest_BINARY_DIR}")