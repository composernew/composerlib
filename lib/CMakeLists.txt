#######################################################
### Q                                               ###
#######################################################

include(FetchContent)

FetchContent_Declare(
        Q
        GIT_REPOSITORY https://github.com/cycfi/q
        GIT_TAG 3e246461b40ded09b34a51192a3c0e7184290655
)

set(Q_BUILD_EXAMPLES OFF)
set(Q_BUILD_TEST OFF)

FetchContent_GetProperties(Q)
if(NOT q_POPULATED)
    FetchContent_Populate(Q)
    set(q_ROOT ${q_SOURCE_DIR})
    get_filename_component(q_ROOT "${q_ROOT}" ABSOLUTE)
    add_subdirectory(${q_ROOT} ${CMAKE_CURRENT_BINARY_DIR}/q EXCLUDE_FROM_ALL)
endif()

#######################################################
### Library                                         ###
#######################################################

add_library(audiolib
        audio/synthesizer.h
        audio/synthesizer.cpp
        )

target_include_directories(audiolib
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(audiolib PUBLIC libq libqio cycfi::infra)