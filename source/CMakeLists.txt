#######################################################
### Dependencies                                    ###
#######################################################
# Threads
find_package(Threads)

# Ranges library
CPMAddPackage(NAME range-v3 URL https://github.com/ericniebler/range-v3/archive/0.11.0.zip VERSION 0.11.0 DOWNLOAD_ONLY TRUE)
add_library(range-v3 INTERFACE IMPORTED)
target_include_directories(range-v3 INTERFACE "${range-v3_SOURCE_DIR}/include")

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