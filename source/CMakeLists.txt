#######################################################
### Dependencies                                    ###
#######################################################

# Ranges library
CPMAddPackage(NAME range-v3 URL https://github.com/ericniebler/range-v3/archive/0.11.0.zip VERSION 0.11.0 DOWNLOAD_ONLY TRUE)
add_library(range-v3 INTERFACE IMPORTED)
target_include_directories(range-v3 INTERFACE "${range-v3_SOURCE_DIR}/include")

FetchContent_Declare(
        pareto
        GIT_REPOSITORY https://github.com/alandefreitas/pareto.git
        GIT_TAG 47f491eeaead1b5a95e27ee3d6bc4c591b0e4462
)

FetchContent_GetProperties(pareto)
if (NOT pareto_POPULATED)
    FetchContent_Populate(pareto)
    add_subdirectory(${pareto_SOURCE_DIR} ${pareto_BINARY_DIR} EXCLUDE_FROM_ALL)
    set(pareto_BUILD_PARETO_WITH_PMR_BY_DEFAULT OFF)
endif ()

#######################################################
### Library                                         ###
#######################################################
add_library(composerlib
        composerlib/genetic_algorithm.cpp
        composerlib/genetic_algorithm.h
        composerlib/melody.cpp
        composerlib/melody.h
        composerlib/melody_problem.cpp
        composerlib/melody_problem.h
        composerlib/nsga_ii.cpp
        composerlib/nsga_ii.h
        composerlib/optimizer.h
        composerlib/optimizer.hpp
)

target_include_directories(composerlib
        PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
target_link_libraries(composerlib PUBLIC range-v3 pareto)
target_pedantic_options(composerlib)
target_exception_options(composerlib)