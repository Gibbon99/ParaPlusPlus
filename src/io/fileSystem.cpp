#include "system/util.h"
#include "io/logFile.h"
#include "io/fileSystem.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Find the names of all the files in the scripts directory and store ready for loading
void io_getScriptFileNames(std::string directoryName)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string tempFileName;

	char **rc = PHYSFS_enumerateFiles (directoryName.c_str ());
	char **i;
	//
	// Store the file names into an array
	for (i = rc; *i != nullptr; i++)
	{
		tempFileName = *i;

		paraScriptInstance.addScript (tempFileName, fileSystem.getString (tempFileName));

		log_addEvent (sys_getString ("Script [ %s ]", tempFileName.c_str ()));
	}

	PHYSFS_freeList (rc);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Load function callback for the audio class - this function loads a file and returns a pointer to memory
SDL_RWops *io_loadRawFile(const std::string& fileName)
//-----------------------------------------------------------------------------------------------------------------------
{
	SDL_RWops *memFilePtr {};
	int       fileSize {};
	char      *rawFileData {};
//	std::unique_ptr<char *> soundFileData;

	if (!fileSystem.doesFileExist (fileName))
	{
		sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, sys_getString ("File not found [ %s ]", fileName.c_str ()));
		return nullptr;
	}
	//
	// How much memory does it need
	fileSize = fileSystem.getFileSize (fileName);
	if (-1 == fileSize)
	{
		sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, sys_getString ("Unable to get file size [ %s ]", fileName.c_str ()));
		return nullptr;
	}

	rawFileData = sys_malloc (fileSize, fileName);  // TODO Needs to be free'd
	if (nullptr == rawFileData)
	{
		sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, sys_getString ("Unable to get memory to hold file [ %s ]", fileName.c_str ()));
		sys_shutdownWithError (sys_getString ("Unable to get memory to hold file [ %s ]", fileName.c_str ()));
	}
	//
	// Load into memory
	fileSystem.getFileIntoMemory (fileName, rawFileData);

	memFilePtr = SDL_RWFromMem (rawFileData, fileSize);

	return memFilePtr;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return a file pointer to a block of memory to be used like a file
//SDL_RWops *io_openMemFile (char *memoryPointer, int memorySize)
UniqueRWops io_openMemFile(char *memoryPointer, int memorySize)
//----------------------------------------------------------------------------------------------------------------------
{
	return UniqueRWops (SDL_RWFromMem (memoryPointer, memorySize));
}
