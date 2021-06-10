find_package(Matplot++ QUIET)

if (Matplot++_FOUND)
    add_executable(example_melody melody.cpp)
    target_link_libraries(example_melody PRIVATE composerlib Matplot++::matplot)
    target_exception_options(example_melody)
endif()

add_executable(example_audio audio.cpp)
target_link_libraries(example_audio PRIVATE composerlib)

add_executable(example_play_midi play_midi.cpp)
target_link_libraries(example_play_midi PRIVATE composerlib)
file(COPY resources/twinkle.midi DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/resources")