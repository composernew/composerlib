#######################################################
### Dependencies                                    ###
#######################################################
# Threads
find_package(Threads)

# Ranges library
CPMAddPackage(NAME range-v3 URL https://github.com/ericniebler/range-v3/archive/0.11.0.zip VERSION 0.11.0 DOWNLOAD_ONLY TRUE)
add_library(range-v3 INTERFACE IMPORTED)
target_include_directories(range-v3 INTERFACE "${range-v3_SOURCE_DIR}/include")

# Infra library
FetchContent_Declare(
        infra
        GIT_REPOSITORY https://github.com/cycfi/infra.git
        GIT_TAG f3e33fe0340d77f9ce4ace1fc6f1a569e033e173
)
FetchContent_MakeAvailable(infra)

# Q library
FetchContent_Declare(
        Q
        GIT_REPOSITORY https://github.com/cycfi/q
        GIT_TAG 3e246461b40ded09b34a51192a3c0e7184290655
)

option(Q_BUILD_EXAMPLES OFF)
option(Q_BUILD_TEST OFF)

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
add_library(composerlib
        composer/harmony.cpp
        composer/harmony.h
        composer/melody.cpp
        composer/melody.h
        composer/objective_parameters.cpp
        composer/objective_parameters.h
)
target_include_directories(composerlib
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
               $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(composerlib PUBLIC Threads::Threads range-v3)
target_pedantic_options(composerlib)
target_exception_options(composerlib)

add_library(audiolib
        audio/synth.h
        audio/synth.cpp
        )

target_include_directories(audiolib
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(audiolib PUBLIC libq libqio cycfi::infra)