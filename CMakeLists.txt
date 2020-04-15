# CMakeList.txt : CMake project for Para++, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Para++")

# Set default locations
set(CL_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})

# Define the CXX sources
set ( CXX_SRCS
        "src/main.cpp"
        "hdr/main.h"
        )

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

# Add source to this project's executable.
if (WIN32)
    # Library path
    link_directories(./SDL2_WIN/lib/x86)
    include_directories(./SDL2_WIN/include)

        if (CMAKE_BUILD_TYPE MATCHES Debug)

            link_libraries(
                    SDL2main
                    SDL2
                    SDL2_image
                    SDL2_mixer
                    SDL2_ttf
                    #asan_dynamic-i386
                    clang_rt.asan_dynamic_runtime_thunk-i386
            )

            message("-")
            message("Doing DEBUG build")
            message("-")
            add_executable (Win32_Para++_debug ${CXX_SRCS})
        elseif (CMAKE_BUILD_TYPE MATCHES Release)

            link_libraries(
                    SDL2main
                    SDL2
                    SDL2_image
                    SDL2_mixer
                    SDL2_ttf
            )

            message("-")
            message("Doing RELEASE build")
            message("-")
            add_executable (Win32_Para++_release ${CXX_SRCS})
        endif (CMAKE_BUILD_TYPE MATCHES Debug)
endif (WIN32)

if (UNIX)
    # Library path
    link_directories(/usr/local/lib)
    include_directories(/usr/local/include)

    if (CMAKE_BUILD_TYPE MATCHES debug)

        link_libraries(
                SDL2main
                SDL2
                SDL2_image
                SDL2_mixer
                SDL2_ttf
                asan            # Memleak and address checking
        )

        message("Doing DEBUG build")
        add_executable (Linux_Para++_debug ${CXX_SRCS})
    elseif (CMAKE_BUILD_TYPE MATCHES release)

        link_libraries(
                SDL2main
                SDL2
                SDL2_image
                SDL2_mixer
                SDL2_ttf
        )

        message("Doing RELEASE build")
        add_executable (Linux_Para++_release ${CXX_SRCS})
    endif (CMAKE_BUILD_TYPE MATCHES debug)
endif (UNIX)
