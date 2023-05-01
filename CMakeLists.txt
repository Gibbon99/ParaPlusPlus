# CMakeList.txt : CMake project for Para++, include source and define
# project specific logic here.
#
project("ParaPlusPlus")
cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)
#
# Get Chipmunk2D directly from Github
FetchContent_Declare(
		Chipmunk2D
		GIT_REPOSITORY https://github.com/slembcke/Chipmunk2D.git
		GIT_TAG master
)
FetchContent_MakeAvailable(Chipmunk2D)

INCLUDE_DIRECTORIES(
		C:/Devel/tools/vcpkg/installed/x64-windows/include
)
LINK_DIRECTORIES(
		C:/Devel/tools/vcpkg/installed/x64-windows/lib
		C:/Devel/tools/vcpkg/installed/x64-windows/lib/manual-link
)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/angelscript/projects/cmake)

# Set default locations
set(CL_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})

file (GLOB_RECURSE source_files "./src/*.cpp")
file (GLOB_RECURSE angelscript_addons_files "./external/angelscript/*.cpp")
file (GLOB_RECURSE angelscript_asm "./external/angelscript/source/as_callfunc_x64_msvc_asm.asm")

set (SOURCES
		src/main.cpp
		${source_files}
		${angelscript_asm}
		${angelscript_addons_files}
		)

include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR}/_deps/chipmunk2d-src/include)
include_directories(SYSTEM ${CMAKE_CURRENT_BINARY_DIR}/external/angelscript/include)

include_directories(
		./hdr
		./hdr/classes
		./hdr/game
		./hdr/gui
		./hdr/io
		./hdr/system
		./hdr/physfs
		./hdr/sdl2_gfx
		./hdr/tinyxml2
)

link_libraries(SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf chipmunk)

set(ENV{CMAKE_BUILD_PARALLEL_LEVEL} 15)

if (WIN32)
	set(BIN_NAME ${PROJECT_NAME}-Win-${CMAKE_BUILD_TYPE})
	add_executable(${BIN_NAME} ${SOURCES})

	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

	if (CMAKE_BUILD_TYPE MATCHES Debug)
		message("Doing DEBUG build")
		target_link_options(${BIN_NAME} PUBLIC /NODEFAULTLIB:MSVCRT)
	endif (CMAKE_BUILD_TYPE MATCHES Debug)

	if (CMAKE_BUILD_TYPE MATCHES Release)
		message("Doing RELEASE build")
		target_link_options(${BIN_NAME} PUBLIC /NODEFAULTLIB:MSVCRTD)
	endif (CMAKE_BUILD_TYPE MATCHES Release)
endif (WIN32)

if (UNIX)
	set(BIN_NAME ${PROJECT_NAME}-Linux-${CMAKE_BUILD_TYPE})

	# Library path
	link_directories(/usr/lib)
	link_libraries(pthread)

	include_directories(
			/usr/include
	)

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
