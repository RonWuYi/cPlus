list(APPEND FILES src/sdlExample.cpp src/example.cpp src/gt.cpp)

set(VCPKG_ROOT C:/Work/vcpkg)

list(APPEND LIBS SDL2 SDL2main avutil avformat avcodec swscale swresample gtest_main gtest)

list(APPEND INCLUDES
${PROJECT_BINARY_DIR}
${VCPKG_ROOT}/installed/x86-windows/include
${gtest_SOURCE_DIR}/googletest/include)


