#######################################################
### Q                                               ###
#######################################################

CPMAddPackage(
        NAME Q
        GITHUB_REPOSITORY cycfi/q
        VERSION 0.0.1
        GIT_TAG 3e246461b40ded09b34a51192a3c0e7184290655
        OPTIONS
        "Q_BUILD_EXAMPLES OFF"
        "Q_BUILD_TEST OFF"
        DOWNLOAD_ONLY TRUE
)

set(Q_ROOT ${Q_SOURCE_DIR})
get_filename_component(Q_ROOT "${Q_ROOT}" ABSOLUTE)
add_subdirectory(${Q_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/q EXCLUDE_FROM_ALL)

#######################################################
### Library                                         ###
#######################################################

add_library(audiolib
        audio/synthesizer.h
        audio/synthesizer.cpp
        audio/midi_processor.cpp
        audio/midi_processor.h
        audio/devices.h)

target_include_directories(audiolib
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(audiolib PUBLIC libq libqio cycfi::infra)