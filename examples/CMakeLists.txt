find_package(Matplot++ QUIET)

if (Matplot++_FOUND)
    add_executable(example_melody melody.cpp)
    target_link_libraries(example_melody PRIVATE composerlib Matplot++::matplot)
    target_exception_options(example_melody)
endif()

add_executable(example_audio audio.cpp)
target_link_libraries(example_audio PRIVATE composerlib)

configure_file("resources/exp-01.midi" "${CMAKE_CURRENT_BINARY_DIR}/resources/exp-01.midi" COPYONLY)
add_executable(example_midi play_midi.cpp)
target_link_libraries(example_midi PRIVATE composerlib)
target_exception_options(example_midi)

add_executable(mono_objective mono_objective.cpp)
target_link_libraries(mono_objective PRIVATE composerlib)
target_exception_options(mono_objective)