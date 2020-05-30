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

		src/box2d/collision/b2_broad_phase.cpp
		src/box2d/collision/b2_chain_shape.cpp
		src/box2d/collision/b2_circle_shape.cpp
		src/box2d/collision/b2_collide_circle.cpp
		src/box2d/collision/b2_collide_edge.cpp
		src/box2d/collision/b2_collide_polygon.cpp
		src/box2d/collision/b2_collision.cpp
		src/box2d/collision/b2_distance.cpp
		src/box2d/collision/b2_dynamic_tree.cpp
		src/box2d/collision/b2_edge_shape.cpp
		src/box2d/collision/b2_polygon_shape.cpp
		src/box2d/collision/b2_time_of_impact.cpp
		src/box2d/common/b2_block_allocator.cpp
		src/box2d/common/b2_draw.cpp
		src/box2d/common/b2_math.cpp
		src/box2d/common/b2_settings.cpp
		src/box2d/common/b2_stack_allocator.cpp
		src/box2d/common/b2_timer.cpp
		src/box2d/dynamics/b2_body.cpp
		src/box2d/dynamics/b2_chain_circle_contact.cpp
		hdr/box2d/dynamics/b2_chain_circle_contact.h
		src/box2d/dynamics/b2_chain_polygon_contact.cpp
		hdr/box2d/dynamics/b2_chain_polygon_contact.h
		src/box2d/dynamics/b2_circle_contact.cpp
		hdr/box2d/dynamics/b2_circle_contact.h
		src/box2d/dynamics/b2_contact.cpp
		src/box2d/dynamics/b2_contact_manager.cpp
		src/box2d/dynamics/b2_contact_solver.cpp
		hdr/box2d/dynamics/b2_contact_solver.h
		src/box2d/dynamics/b2_distance_joint.cpp
		src/box2d/dynamics/b2_edge_circle_contact.cpp
		hdr/box2d/dynamics/b2_edge_circle_contact.h
		src/box2d/dynamics/b2_edge_polygon_contact.cpp
		hdr/box2d/dynamics/b2_edge_polygon_contact.h
		src/box2d/dynamics/b2_fixture.cpp
		src/box2d/dynamics/b2_friction_joint.cpp
		src/box2d/dynamics/b2_gear_joint.cpp
		src/box2d/dynamics/b2_island.cpp
		hdr/box2d/dynamics/b2_island.h
		src/box2d/dynamics/b2_joint.cpp
		src/box2d/dynamics/b2_motor_joint.cpp
		src/box2d/dynamics/b2_mouse_joint.cpp
		src/box2d/dynamics/b2_polygon_circle_contact.cpp
		hdr/box2d/dynamics/b2_polygon_circle_contact.h
		src/box2d/dynamics/b2_polygon_contact.cpp
		hdr/box2d/dynamics/b2_polygon_contact.h
		src/box2d/dynamics/b2_prismatic_joint.cpp
		src/box2d/dynamics/b2_pulley_joint.cpp
		src/box2d/dynamics/b2_revolute_joint.cpp
		src/box2d/dynamics/b2_rope_joint.cpp
		src/box2d/dynamics/b2_weld_joint.cpp
		src/box2d/dynamics/b2_wheel_joint.cpp
		src/box2d/dynamics/b2_world.cpp
		src/box2d/dynamics/b2_world_callbacks.cpp

		src/classes/ConvertUTF.cpp

		src/sdl2_gfx/SDL2_gfxPrimitives.cpp
		src/sdl2_gfx/SDL2_imageFilter.cpp
		src/sdl2_gfx/SDL2_rotozoom.cpp

		src/fileWatcher/FileWatcher.cpp
		src/fileWatcher/FileWatcherLinux.cpp
		src/fileWatcher/FileWatcherWin32.cpp

		src/system/scriptEngine.cpp hdr/system/scriptEngine.h
		src/classes/paraLogFile.cpp hdr/classes/paraLogFile.h
		src/system/shutdown.cpp hdr/system/shutdown.h
		src/system/util.cpp hdr/system/util.h
		src/classes/paraScript.cpp hdr/classes/paraScript.h
		src/classes/paraFileSystem.cpp hdr/io/fileSystem.h
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
		src/classes/paraRenderer.cpp hdr/classes/paraRenderer.h src/io/consoleFunctions.cpp hdr/io/consoleFunctions.h src/system/gameEvents.cpp hdr/system/gameEvents.h src/system/physics.cpp hdr/system/physics.h src/game/physicsCollisions.cpp hdr/game/physicsCollisions.h src/io/resources.cpp hdr/io/resources.h src/classes/paraAudio.cpp hdr/classes/paraAudio.h src/game/audio.cpp hdr/game/audio.h src/classes/paraTexture.cpp hdr/classes/paraTexture.h src/game/texture.cpp hdr/game/texture.h src/classes/paraGui.cpp hdr/classes/paraGui.h src/gui/guiButton.cpp hdr/gui/guiButton.h src/gui/guiRender.cpp hdr/gui/guiRender.h src/io/fileWatch.cpp hdr/io/fileWatch.h src/gui/guiLanguage.cpp hdr/gui/guiLanguage.h src/io/keyboard.cpp hdr/io/keyboard.h src/gui/guiInput.cpp hdr/gui/guiInput.h src/io/joystick.cpp hdr/io/joystick.h src/io/mouse.cpp hdr/io/mouse.h src/gui/guiSideview.cpp hdr/gui/guiSideview.h hdr/classes/paraRandom.h src/gui/guiSlider.cpp hdr/gui/guiSlider.h src/gui/guiLabel.cpp hdr/gui/guiLabel.h src/gui/guiScrollbox.cpp hdr/gui/guiScrollbox.h src/gui/guiCheckbox.cpp hdr/gui/guiCheckbox.h src/game/hud.cpp hdr/game/hud.h src/game/database.cpp hdr/game/database.h src/classes/paraSprite.cpp hdr/classes/paraSprite.h src/game/tiles.cpp hdr/game/tiles.h src/game/shipDecks.cpp hdr/game/shipDecks.h src/game/healing.cpp hdr/game/healing.h src/game/lifts.cpp hdr/game/lifts.h src/game/droidClass.cpp hdr/game/droidClass.h src/game/player.cpp hdr/game/player.h src/game/doors.cpp hdr/game/doors.h src/game/terminal.cpp hdr/game/terminal.h hdr/classes/paraLift.h src/classes/paraLift.cpp hdr/classes/paraLift.h src/gui/guiDeckView.cpp hdr/gui/guiDeckView.h src/game/pathFind.cpp hdr/game/pathFind.h src/game/droids.cpp hdr/game/droids.h src/game/game.cpp hdr/game/game.h src/classes/paraAI.cpp hdr/classes/paraAI.h src/game/bullet.cpp hdr/game/bullet.h hdr/classes/paraLOS.h src/classes/paraParticle.cpp hdr/classes/paraParticle.h src/game/particles.cpp hdr/game/particles.h src/classes/paraLightmap.cpp hdr/classes/paraLightmap.h src/game/lightMaps.cpp hdr/game/lightMaps.h)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${COMPILE_FLAGS}")

# Add source to this project's executable.
if (WIN32)
    # Library path
    link_directories(./SDL2_WIN/lib/x86)
    include_directories(
		    ./SDL2_WIN/include
		    ./hdr/script
		    ./hdr/physfs
		    ./hdr
		    ./hdr/box2d
		    ./hdr/sdl2_gfx
		    ./hdr/fileWatcher
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
            add_executable (Win32_ParaPlusPlus_debug ${CXX_SRCS})
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
            add_executable (Win32_ParaPlusPlus_release ${CXX_SRCS})
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
		    ./hdr
		    ./hdr/box2d
		    ./hdr/sdl2_gfx
		    ./hdr/fileWatcher
    )

    if (CMAKE_BUILD_TYPE MATCHES debug)

        link_libraries(
#		        asan
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
