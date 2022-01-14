# CMakeList.txt : CMake project for Para++, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

project("ParaPlusPlus")

# set(CMAKE_CXX_STANDARD "17")


IF (CMAKE_VERSION VERSION_LESS "3.8")
    SET(CMAKE_CXX_STANDARD 14)
ELSEIF (CMAKE_VERSION VERSION_LESS "3.11")
    SET(CMAKE_CXX_STANDARD 17)
ELSE()
    SET(CMAKE_CXX_STANDARD 20)
ENDIF()

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default locations
set(CL_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})

file (GLOB classes_source_files "./src/classes/*.cpp")
file (GLOB script_source_files "./src/script/*.cpp")
file (GLOB chipmunk_source_files "./src/chipmunk/*.c")
file (GLOB fileWatch_source_files "./src/fileWatcher/*.cpp")
file (GLOB game_source_files "./src/game/*.cpp")
file (GLOB gui_source_files "./src/gui/*.cpp")
file (GLOB io_source_files "./src/io/*.cpp")
file (GLOB physfs_source_files "./src/physfs/*.cpp")
file (GLOB sdl2_gfx_source_files "./src/sdl2_gfx/*.cpp")
file (GLOB system_source_files "./src/system/*.cpp")
file (GLOB tinyxml2_source_files "./src/tinyxml2/*.cpp")

set (SOURCES
		src/main.cpp
		${classes_source_files}
		${script_source_files}
		${chipmunk_source_files}
		${fileWatch_source_files}
		${game_source_files}
		${gui_source_files}
		${io_source_files}
		${physfs_source_files}
		${sdl2_gfx_source_files}
		${system_source_files}
		${tinyxml2_source_files}
		)

if (WIN32)
	# Library path
	link_directories(./SDL2_WIN/lib/x86)

	link_libraries(SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf)

	include_directories(
			./hdr
			./hdr/script
			./hdr/physfs
			./hdr/sdl2_gfx
			./hdr/fileWatcher
			./SDL2_WIN/include
			./hdr/chipmunk
			./hdr/tinyxml2
	)

	set(ENV{CMAKE_BUILD_PARALLEL_LEVEL} 5)

	if (CMAKE_BUILD_TYPE MATCHES Debug)
		message("Doing DEBUG build")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")		
		add_executable(Win32_Para_debug ${SOURCES})
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
	# Library path
	link_directories(/usr/lib)

	link_libraries(SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf pthread)

	include_directories(
			/usr/include
			/usr/include/SDL2
			./hdr
			./hdr/script
			./hdr/physfs
			./hdr/sdl2_gfx
			./hdr/fileWatcher
			./hdr/chipmunk
			./hdr/tinyxml2)

	set(ENV{CMAKE_BUILD_PARALLEL_LEVEL} 5)

	if (CMAKE_BUILD_TYPE MATCHES Debug)
		message("Doing DEBUG build")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -g -O0 -Wimplicit-fallthrough -Wtype-limits -Wint-in-bool-context -Wcast-function-type -fno-omit-frame-pointer -Wsign-compare -fpermissive -Wall -Wextra -g -fsanitize=leak")
		add_executable(Linux_Para_debug ${SOURCES})
		target_compile_definitions(Linux_Para_debug PUBLIC MY_DEBUG)
	endif (CMAKE_BUILD_TYPE MATCHES Debug)

	if (CMAKE_BUILD_TYPE MATCHES Release)
		message("Doing RELEASE build")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -DNDEBUG")
		add_executable(Linux_Para_release ${CXX_SRCS})
	endif (CMAKE_BUILD_TYPE MATCHES Release)

endif (UNIX)
