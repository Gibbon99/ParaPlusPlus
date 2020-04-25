# CMakeList.txt : CMake project for Para++, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("Para++")

set(CMAKE_CXX_STANDARD "14")

#set_property (TARGET ${PROJECT_NAME}
#PROPERTY
#CXX_STANDARD 14
#)

# Set default locations
set(CL_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})

# Define the CXX sources
set (CXX_SRCS
     "src/main.cpp"
     "hdr/main.h"
     src/script/as_callfunc_arm_gcc.S
     src/script/as_callfunc_arm_vita.S
     src/script/as_callfunc_arm_xcode.S
     src/script/as_atomic.cpp
     src/script/as_builder.cpp
     src/script/as_bytecode.cpp
     src/script/as_callfunc_arm.cpp
     src/script/as_callfunc_mips.cpp
     src/script/as_callfunc_ppc_64.cpp
     src/script/as_callfunc_ppc.cpp
     src/script/as_callfunc_sh4.cpp
     src/script/as_callfunc_x64_gcc.cpp
     src/script/as_callfunc_x64_mingw.cpp
     src/script/as_callfunc_x64_msvc.cpp
     src/script/as_callfunc_x86.cpp
     src/script/as_callfunc_xenon.cpp
     src/script/as_callfunc.cpp
     src/script/as_compiler.cpp
     src/script/as_configgroup.cpp
     src/script/as_context.cpp
     src/script/as_datatype.cpp
     src/script/as_gc.cpp
     src/script/as_generic.cpp
     src/script/as_globalproperty.cpp
     src/script/as_memory.cpp
     src/script/as_module.cpp
     src/script/as_objecttype.cpp
     src/script/as_outputbuffer.cpp
     src/script/as_parser.cpp
     src/script/as_restore.cpp
     src/script/as_scriptcode.cpp
     src/script/as_scriptengine.cpp
     src/script/as_scriptfunction.cpp
     src/script/as_scriptnode.cpp
     src/script/as_scriptobject.cpp
     src/script/as_string_util.cpp
     src/script/as_string.cpp
     src/script/as_thread.cpp
     src/script/as_tokenizer.cpp
     src/script/as_typeinfo.cpp
     src/script/as_variablescope.cpp
     src/script/as_callfunc_arm_msvc.asm
     src/script/as_callfunc_x64_msvc_asm.asm
     src/script/scriptarray.cpp
     src/script/scriptbuilder.cpp
     src/script/scriptstdstring.cpp
     src/script/scriptstdstring_utils.cpp

     src/physfs/physfs.cpp
     src/physfs/physfs_archiver_7z.cpp
     src/physfs/physfs_archiver_dir.cpp
     src/physfs/physfs_archiver_grp.cpp
     src/physfs/physfs_archiver_hog.cpp
     src/physfs/physfs_archiver_iso9660.cpp
     src/physfs/physfs_archiver_mvl.cpp
     src/physfs/physfs_archiver_qpak.cpp
     src/physfs/physfs_archiver_slb.cpp
     src/physfs/physfs_archiver_unpacked.cpp
     src/physfs/physfs_archiver_vdf.cpp
     src/physfs/physfs_archiver_wad.cpp
     src/physfs/physfs_archiver_zip.cpp
     src/physfs/physfs_byteorder.cpp
     src/physfs/physfs_platform_haiku.cpp
     src/physfs/physfs_platform_os2.cpp
     src/physfs/physfs_platform_posix.cpp
     src/physfs/physfs_platform_qnx.cpp
     src/physfs/physfs_platform_unix.cpp
     src/physfs/physfs_platform_windows.cpp
     src/physfs/physfs_platform_winrt.cpp
     src/physfs/physfs_unicode.cpp

     src/system/scriptEngine.cpp hdr/system/scriptEngine.h
     src/classes/paraLogFile.cpp hdr/classes/paraLogFile.h
     src/system/shutdown.cpp hdr/system/shutdown.h
     src/system/util.cpp hdr/system/util.h
     src/classes/paraScript.cpp hdr/classes/paraScript.h
     src/classes/paraFileSystem.cpp hdr/io/fileSystem.h
     src/classes/IniFileParser.cpp
     src/system/scriptConfig.cpp hdr/system/scriptConfig.h
     src/system/startup.cpp hdr/system/startup.h
     src/io/fileSystem.cpp hdr/classes/paraFileSystem.h
     src/classes/paraFont.cpp hdr/classes/paraFont.h hdr/wrapper.h
     src/classes/paraConsole.cpp hdr/classes/paraConsole.h
     src/system/frameUpdate.cpp hdr/system/frameUpdate.h src/system/frameRender.cpp
     hdr/system/frameRender.h src/io/configFile.cpp
     hdr/io/configFile.h src/io/console.cpp hdr/io/console.h
     src/classes/paraEvent.cpp hdr/classes/paraEvent.h
     src/system/eventsEngine.cpp hdr/system/eventsEngine.h
     src/io/logFile.cpp hdr/io/logFile.h
     hdr/system/enums.h)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

# Add source to this project's executable.
if (WIN32)
    # Library path
    link_directories(./SDL2_WIN/lib/x86)
    include_directories(
            ./SDL2_WIN/include
            ./hdr/script
            ./hdr/physfs
    )

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
    include_directories(
            /usr/local/include
            /usr/local/include/SDL2
            ./hdr/script
            ./hdr/physfs
    )

    if (CMAKE_BUILD_TYPE MATCHES debug)

        link_libraries(
                asan            # Memleak and address checking
                SDL2main
                SDL2
                SDL2_image
                SDL2_mixer
                SDL2_ttf
                pthread
        )

        message("Doing DEBUG build")
        add_executable (Linux_Para++_debug ${CXX_SRCS} src/system/util.cpp hdr/system/util.h src/classes/paraScript.cpp hdr/classes/paraScript.h)
    elseif (CMAKE_BUILD_TYPE MATCHES release)

        link_libraries(
                SDL2main
                SDL2
                SDL2_image
                SDL2_mixer
                SDL2_ttf
                pthread
        )

        message("Doing RELEASE build")
        add_executable (Linux_Para++_release ${CXX_SRCS})
    endif (CMAKE_BUILD_TYPE MATCHES debug)
endif (UNIX)
