project(cmakeLeaning)

set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)

configure_file (
        "${PROJECT_SOURCE_DIR}/Config.h.in"
        "${PROJECT_BINARY_DIR}/Config.h"
)

include_directories("${PROJECT_BINARY_DIR}")
add_executable(cmakeLeaning src/main.c)
target_link_libraries(cmakeLeaning SDL2 SDL2main avutil avformat avcodec swscale swresample)

install(TARGETS cmakeLeaning RUNTIME DESTINATION ${BIN_DIR})