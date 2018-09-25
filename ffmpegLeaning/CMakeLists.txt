project(ffmpegLeaning)

add_executable(ffmpegLeaning src/example.c src/success.cpp)
target_link_libraries(ffmpegLeaning SDL2 SDL2main avutil avformat avcodec swscale swresample)


target_link_libraries(ffmpegLeaning ${SDL2_LIBRARY})
install(TARGETS ffmpegLeaning RUNTIME DESTINATION ${BIN_DIR})