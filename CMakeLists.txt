# CMakeList.txt : CMake project for Para++, include source and define
# project specific logic here.
#
cmake_minimum_required(VERSION 3.8)

project("ParaPlusPlus")

set(CMAKE_CXX_STANDARD "17")

# Set default locations
set(CL_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CL_OUTPUT_DIRECTORY})

# Define the CXX sources
set(CXX_SRCS
		src/main.cpp
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
		src/box2d/dynamics/b2_chain_polygon_contact.cpp
		src/box2d/dynamics/b2_circle_contact.cpp
		src/box2d/dynamics/b2_contact.cpp
		src/box2d/dynamics/b2_contact_manager.cpp
		src/box2d/dynamics/b2_contact_solver.cpp
		src/box2d/dynamics/b2_distance_joint.cpp
		src/box2d/dynamics/b2_edge_circle_contact.cpp
		src/box2d/dynamics/b2_edge_polygon_contact.cpp
		src/box2d/dynamics/b2_fixture.cpp
		src/box2d/dynamics/b2_friction_joint.cpp
		src/box2d/dynamics/b2_gear_joint.cpp
		src/box2d/dynamics/b2_island.cpp
		src/box2d/dynamics/b2_joint.cpp
		src/box2d/dynamics/b2_motor_joint.cpp
		src/box2d/dynamics/b2_mouse_joint.cpp
		src/box2d/dynamics/b2_polygon_circle_contact.cpp
		src/box2d/dynamics/b2_polygon_contact.cpp
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

		src/system/scriptEngine.cpp
		src/classes/paraLogFile.cpp
		src/system/shutdown.cpp
		src/system/util.cpp
		src/classes/paraScript.cpp
		src/classes/paraFileSystem.cpp
		src/system/scriptConfig.cpp
		src/system/startup.cpp
		src/io/fileSystem.cpp
		src/classes/paraFont.cpp
		src/classes/paraConsole.cpp
		src/system/frameUpdate.cpp
		src/system/frameRender.cpp
		src/io/configFile.cpp
		src/io/console.cpp
		src/classes/paraEvent.cpp
		src/system/eventsEngine.cpp
		src/io/logFile.cpp
		src/classes/paraRenderer.cpp
		src/io/consoleFunctions.cpp
		src/system/gameEvents.cpp
		src/system/physics.cpp
		src/game/physicsCollisions.cpp
		src/io/resources.cpp
		src/classes/paraAudio.cpp
		src/game/audio.cpp
		src/classes/paraTexture.cpp
		src/game/texture.cpp
		src/classes/paraGui.cpp
		src/gui/guiButton.cpp
		src/gui/guiRender.cpp
		src/io/fileWatch.cpp
		src/gui/guiLanguage.cpp
		src/io/keyboard.cpp
		src/gui/guiInput.cpp
		src/io/joystick.cpp
		src/io/mouse.cpp
		src/gui/guiSideview.cpp
		src/gui/guiSlider.cpp
		src/gui/guiLabel.cpp
		src/gui/guiScrollbox.cpp
		src/gui/guiCheckbox.cpp
		src/game/hud.cpp
		src/game/database.cpp
		src/classes/paraSprite.cpp
		src/game/tiles.cpp
		src/game/shipDecks.cpp
		src/game/healing.cpp
		src/game/lifts.cpp
		src/game/player.cpp
		src/game/doors.cpp
		src/game/terminal.cpp
		src/classes/paraLift.cpp
		src/gui/guiDeckView.cpp
		src/game/droids.cpp
		src/game/game.cpp
		src/classes/paraBullet.cpp
		src/classes/paraParticle.cpp
		src/game/particles.cpp
		src/classes/paraLightmap.cpp
		src/game/lightMaps.cpp
		src/game/lineOfSight.cpp
		src/game/score.cpp
		src/game/alertLevel.cpp
		src/game/transfer.cpp
		src/game/transferRender.cpp
		src/gui/guiTextbox.cpp
		src/gui/guiImage.cpp
		src/game/transferDroidAI.cpp
		src/game/transferGame.cpp
		src/game/transferPlayer.cpp
		src/game/pauseMode.cpp
		src/gui/guiLostScreen.cpp
		src/gui/guiWonScreen.cpp
		src/gui/guiHighScore.cpp
		src/gui/guiDialogBox.cpp
		src/system/util.cpp
		src/classes/paraScript.cpp
		src/classes/paraAStar.cpp
		src/classes/paraAI2.cpp
		src/classes/paraDroid.cpp
		src/classes/paraDroid.cpp
		src/classes/paraPhysicsDestruction.cpp
		src/classes/paraStarfield.cpp
		src/classes/paraHighScore.cpp
		src/classes/paraEmitter.cpp

		src/chipmunk/chipmunk.c
		src/chipmunk/cpArbiter.c
		src/chipmunk/cpArray.c
		src/chipmunk/cpBBTree.c
		src/chipmunk/cpBody.c
		src/chipmunk/cpCollision.c
		src/chipmunk/cpConstraint.c
		src/chipmunk/cpDampedRotarySpring.c
		src/chipmunk/cpDampedSpring.c
		src/chipmunk/cpGearJoint.c
		src/chipmunk/cpGrooveJoint.c
		src/chipmunk/cpHashSet.c
		src/chipmunk/cpHastySpace.c
		src/chipmunk/cpMarch.c
		src/chipmunk/cpPinJoint.c
		src/chipmunk/cpPivotJoint.c
		src/chipmunk/cpPolyline.c
		src/chipmunk/cpPolyShape.c
		src/chipmunk/cpRatchetJoint.c
		src/chipmunk/cpRobust.c
		src/chipmunk/cpRotaryLimitJoint.c
		src/chipmunk/cpShape.c
		src/chipmunk/cpSimpleMotor.c
		src/chipmunk/cpSlideJoint.c
		src/chipmunk/cpSpace.c
		src/chipmunk/cpSpaceComponent.c
		src/chipmunk/cpSpaceDebug.c
		src/chipmunk/cpSpaceHash.c
		src/chipmunk/cpSpaceQuery.c
		src/chipmunk/cpSpaceStep.c
		src/chipmunk/cpSpatialIndex.c
		src/chipmunk/cpSweep1D.c
		src/chipmunk/prime.h

		src/chipmunkpp/arbiter.cpp
		src/chipmunkpp/bb.cpp
		src/chipmunkpp/body.cpp
		src/chipmunkpp/chipmunk.cpp
		src/chipmunkpp/circleshape.cpp
		src/chipmunkpp/polyshape.cpp
		src/chipmunkpp/segmentshape.cpp
		src/chipmunkpp/shape.cpp
		src/chipmunkpp/space.cpp
		src/chipmunkpp/types.cpp
		src/chipmunkpp/vect.cpp
		)

if (WIN32)
	# Library path
	link_directories(./SDL2_WIN/lib/x86)

	link_libraries(SDL2main SDL2 SDL2_image SDL2_mixer SDL2_ttf)

	include_directories(
			./hdr
			./hdr/script
			./hdr/physfs
			./hdr/box2d
			./hdr/sdl2_gfx
			./hdr/fileWatcher
			./SDL2_WIN/include
			./hdr/chipmunk
			./hdr/chipmunkpp
	)

	if (CMAKE_BUILD_TYPE MATCHES Debug)
		message("-")
		message("Doing DEBUG build")
		message("-")
		add_executable(Win32_Para_debug ${CXX_SRCS})
		target_link_libraries(Win32_Para_debug Debug clang_rt.asan_dynamic_runtime_thunk-i386)
	endif (CMAKE_BUILD_TYPE MATCHES Debug)

	if (CMAKE_BUILD_TYPE MATCHES Release)
		message("-")
		message("Doing RELEASE build")
		message("-")
		add_executable(Win32_Para_release ${CXX_SRCS})
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
			./hdr/box2d
			./hdr/sdl2_gfx
			./hdr/fileWatcher
			./hdr/chipmunk
			./hdr/chipmunkpp)

	set(ENV{CMAKE_BUILD_PARALLEL_LEVEL} 5)

	if (CMAKE_BUILD_TYPE MATCHES Debug)
		message("Doing DEBUG build")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg -g -O0 -fno-omit-frame-pointer -Wint-in-bool-context -Wsign-compare -fpermissive -std=gnu++0x -Wall -Wextra -fsanitize=address")
		add_executable(Linux_Para_debug ${CXX_SRCS})
		target_link_libraries(Linux_Para_debug asan)
		target_compile_definitions(Linux_Para_debug PUBLIC MY_DEBUG)
	endif (CMAKE_BUILD_TYPE MATCHES Debug)

	if (CMAKE_BUILD_TYPE MATCHES Release)
		message("Doing RELEASE build")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall -DNDEBUG")
		add_executable(Linux_Para_release ${CXX_SRCS})
	endif (CMAKE_BUILD_TYPE MATCHES Release)

endif (UNIX)
