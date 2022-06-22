add_executable(example_audio audio.cpp)
target_link_libraries(example_audio PRIVATE composerlib)

add_executable(example_optimizer optimizer.cpp)
target_link_libraries(example_optimizer PRIVATE composerlib)
