find_package(Matplot++ QUIET)

if (Matplot++_FOUND)
    add_executable(example_melody melody.cpp)
    target_link_libraries(example_melody PRIVATE composerlib Matplot++::matplot)
    target_exception_options(example_melody)
endif()

add_executable(example_note note.cpp)
target_link_libraries(example_note PRIVATE noteslib)