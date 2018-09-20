project(ffmpegLeaning)

add_executable(ffmpegLeaning src/main.c)
target_link_libraries(ffmpegLeaning SDL2 SDL2main avutil avformat avcodec swscale swresample)