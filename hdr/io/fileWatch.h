#pragma once

#include <system/util.h>
#include "system/gameEvents.h"
#include "main.h"
#include "../fileWatcher/FileWatcher.h"
#include "logFile.h"

// Start the file watcher
void io_startFileWatcher();

// Check for file changes
void io_checkFileWatcher();

//----------------------------------------------------------------------------------------------------------------------
//
// Process a file action
class UpdateListener : public FW::FileWatchListener
//----------------------------------------------------------------------------------------------------------------------
{
public:

	UpdateListener() = default;;

	void handleFileAction([[maybe_unused]]FW::WatchID watchid, const FW::String &dir, const FW::String &filename, FW::Action action) override
	{
		switch (action)
		{
			case FW::Actions::Add:
				//
				// Don't restart if the file already exists - can't be a new file being added
				if (!paraScriptInstance.isScriptName (filename))
				{
//					gam_addEvent (EVENT_ACTION_GAME_SCRIPT_RESTART, 5, "");
					log_addEvent (sys_getString ("[ %s ] File [ %s ] added.", __func__, sys_getString ("%s\\%s\\", dir.c_str (), filename.c_str ()).c_str ()));
				}

				break;

			case FW::Actions::Delete:
//				std::cout << "File (" << dir + "\\" + filename << ") Deleted! " << std::endl;
				break;

			case FW::Actions::Modified:
				//
				// Don't restart on temporary files being written from a file save
				if (paraScriptInstance.isScriptName (filename))
				{
					gam_addEvent (EVENT_ACTION_GAME_SCRIPT_RESTART, 5, (std::string) " | | | ");
					log_addEvent (sys_getString ("[ %s ] File [ %s ] modifed.", __func__, sys_getString ("%s\\%s\\", dir.c_str (), filename.c_str ()).c_str ()));
				}

				break;

			default:
				std::cout << "Should never happen!" << std::endl;
				break;
		}
	}
};
