#pragma once

#include <system/gameEvents.h>
#include "../main.h"
#include "../fileWatcher/FileWatcher.h"

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

	void handleFileAction(FW::WatchID watchid, const FW::String &dir, const FW::String &filename, FW::Action action) override
	{
		switch (action)
		{
			case FW::Actions::Add:
				//
				// Don't restart if the file already exists - can't be a new file being added
				if (!paraScriptInstance.isScriptName(filename))
				{
//					gam_addEvent (EVENT_ACTION_GAME_SCRIPT_RESTART, 5, "");
					std::cout << "File (" << dir + "\\" + filename << ") Added! " << std::endl;
				}

				break;

			case FW::Actions::Delete:
//				std::cout << "File (" << dir + "\\" + filename << ") Deleted! " << std::endl;
				break;

			case FW::Actions::Modified:
				//
				// Don't restart on temporary files being written from a file save
				if (paraScriptInstance.isScriptName(filename))
				{
					gam_addEvent (EVENT_ACTION_GAME_SCRIPT_RESTART, 5, (string &) "");
					std::cout << "File (" << dir + "\\" + filename << ") Modified! " << std::endl;
				}

				break;

			default:
				std::cout << "Should never happen!" << std::endl;
				break;
		}
	}
};
