# CMakeList.txt : CMake project for Para++, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)
include(FetchContent)

project("ParaPlusPlus")
#
# Get SDL2 directly from Github
FETCHCONTENT_DECLARE(
		SDL2
		GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
		GIT_TAG SDL2
)
FetchContent_MakeAvailable(SDL2)
#
# Get libOpus directly from Github
FETCHCONTENT_DECLARE(
		Opus
		GIT_REPOSITORY https://github.com/xiph/opus.git
		GIT_TAG Master
)
FetchContent_MakeAvailable(Opus)
#
# Get SDL2_Mixer directly from Github
FETCHCONTENT_DECLARE(
		SDL2_Mixer
		GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git
		GIT_TAG SDL2
)
FetchContent_MakeAvailable(SDL2_Mixer)
#
# Get SDL2_TTF directly from Github
FETCHCONTENT_DECLARE(
		SDL2_TTF
		GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git
		GIT_TAG SDL2
)
FetchContent_MakeAvailable(SDL2_TTF)
#
# Get SDL2_Image directly from Github
FETCHCONTENT_DECLARE(
		SDL2_Image
		GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
		GIT_TAG SDL2_Image
)
FetchContent_MakeAvailable(SDL2_Image)
#
# Get Chipmunk2D directly from Github
FETCHCONTENT_DECLARE(
		SDL2_Image
		GIT_REPOSITORY https://github.com/slembcke/Chipmunk2D.git
		GIT_TAG Chipmunk2D
)
FetchContent_MakeAvailable(Chipmunk2D)

if (WIN32)
	set(ANGEL_SCRIPT_OUTPUT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external/angelscript/msvc/)
else()
	set(ANGEL_SCRIPT_OUTPUT_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/external/angelscript/gnuc/)
endif()

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/angelscript/projects/cmake ${ANGEL_SCRIPT_OUTPUT_LOCATION})

IF (CMAKE_VERSION VERSION_LESS "3.8")
    SET(CMAKE_CXX_STANDARD 14)
ELSEIF (CMAKE_VERSION VERSION_LESS "3.11")
    SET(CMAKE_CXX_STANDARD 17)
ELSE()
    SET(CMAKE_CXX_STANDARD 17)
ENDIF()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default locations
set(CL_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})

file (GLOB_RECURSE source_files "./src/classes/*.cpp")

set (SOURCES
		src/main.cpp
		${source_files}
		)

include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR}/_deps/sdl3-src/include)
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR}/_deps/enet-src/include)
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR}/_deps/chipmunk-src/include)
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR}/external/angelscript/include)

include_directories(
		./hdr
		./hdr/physfs
		./hdr/sdl2_gfx
		./hdr/tinyxml2
)

link_libraries(SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf)

set(ENV{CMAKE_BUILD_PARALLEL_LEVEL} 15)

if (WIN32)
	set(BIN_NAME ${PROJECT_NAME}-Window-${CMAKE_BUILD_TYPE})

	if (CMAKE_BUILD_TYPE MATCHES Debug)
		message("Doing DEBUG build")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
		add_executable(${BIN_NAME} ${SOURCES})
		target_link_libraries(Win32_Para_debug)
#		target_link_libraries(Win32_Para_debug)
	endif (CMAKE_BUILD_TYPE MATCHES Debug)

	if (CMAKE_BUILD_TYPE MATCHES Release)
		message("Doing RELEASE build")
		add_executable(Win32_Para_release ${SOURCES})
		target_link_libraries(Win32_Para_release)
	endif (CMAKE_BUILD_TYPE MATCHES Release)
endif (WIN32)

if (UNIX)
	set(BIN_NAME ${PROJECT_NAME}-Linux-${CMAKE_BUILD_TYPE})

	# Library path
	link_directories(/usr/lib)
	link_libraries(pthread)

	include_directories(
			/usr/include
			./hdr
			./hdr/physfs
			./hdr/sdl2_gfx
			./hdr/tinyxml2)

	if (CMAKE_BUILD_TYPE MATCHES Debug)
		message("Doing DEBUG build")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -g -O0 -Wimplicit-fallthrough -Wtype-limits -Wint-in-bool-context -Wcast-function-type -fno-omit-frame-pointer -Wsign-compare -fpermissive -Wall -Wextra -g -fsanitize=leak")
		add_executable(Linux_Para_debug ${SOURCES} hdr/io/libfmemopen.h)
#		target_compile_definitions(Linux_Para_debug PUBLIC MY_DEBUG)
	endif (CMAKE_BUILD_TYPE MATCHES Debug)

	if (CMAKE_BUILD_TYPE MATCHES Release)
		message("Doing RELEASE build")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -DNDEBUG")
		add_executable(Linux_Para_release ${SOURCES})
	endif (CMAKE_BUILD_TYPE MATCHES Release)

endif (UNIX)
