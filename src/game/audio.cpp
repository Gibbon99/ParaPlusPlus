#include <string>
#include "game/audio.h"

//-----------------------------------------------------------------------------------------------------------------------
//
// Load function callback for the audio class - this function loads a file and returns a pointer to memory
SDL_RWops* io_loadAudioFile(std::string fileName)
//-----------------------------------------------------------------------------------------------------------------------
{
	SDL_RWops*  memFilePtr;
	int fileSize;
	char *soundFileData;

	if (!fileSystem.doesFileExist(fileName))
	{
		sys_addEvent(EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, sys_getString("File not found [ %s ]", fileName.c_str()));
		return nullptr;
	}
	//
	// How much memory does it need
	fileSize = fileSystem.getFileSize(fileName);
	if (-1 == fileSize)
	{
		sys_addEvent(EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, sys_getString("Unable to get file size [ %s ]", fileName.c_str()));
		return nullptr;
	}

	soundFileData = sys_malloc(fileSize, fileName);
	if (nullptr == soundFileData)
	{
		sys_addEvent(EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, sys_getString("Unable to get memory to hold file [ %s ]", fileName.c_str()));
		sys_shutdownWithError(sys_getString("Unable to get memory to hold file [ %s ]", fileName.c_str()));
	}
	//
	// Load into memory
	fileSystem.getFileIntoMemory(fileName, soundFileData);

	memFilePtr = SDL_RWFromMem(soundFileData, fileSize);

	return memFilePtr;
}