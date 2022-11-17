cmake_minimum_required(VERSION 3.24)
project(cmakeLeaning)

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(variable.cmake)

configure_file (
        "${PROJECT_SOURCE_DIR}/Config.h.in"
        "${PROJECT_BINARY_DIR}/Config.h"
)

include_directories("${PROJECT_BINARY_DIR}")
add_executable(cmakeLeaning)
target_sources(cmakeLeaning PRIVATE ${FILES})

target_compile_features(cmakeLeaning PUBLIC cxx_std_20)

target_include_directories(cmakeLeaning PUBLIC 
                        "${PROJECT_BINARY_DIR}"
                        "${VCPKG_ROOT}/installed/x86-windows/include")

target_link_libraries(cmakeLeaning SDL2 SDL2main avutil avformat avcodec swscale swresample)

install(TARGETS cmakeLeaning RUNTIME DESTINATION ${BIN_DIR})