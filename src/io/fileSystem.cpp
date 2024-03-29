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
#ifdef DEBUG_SCRIPT
		log_addEvent (sys_getString ("Script [ %s ]", tempFileName.c_str ()));
#endif
	}

	PHYSFS_freeList (rc);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Load function callback for the audio class - this function loads a file and returns a pointer to memory
SDL_RWops * io_loadRawFile(std::string fileName) // NOLINT(performance-unnecessary-value-param)
//-----------------------------------------------------------------------------------------------------------------------
{
	SDL_RWops *memFilePtr {};
//	int       fileSize {};
	char *rawFileData {};
//	std::unique_ptr<char *> soundFileData;

	if (!fileSystem.doesFileExist (fileName))
	{
		sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, sys_getString ("File not found [ %s ]", fileName.c_str ()));
		return nullptr;
	}
	//
	// How much memory does it need
	auto fileSize = fileSystem.getFileSize (fileName);
	if (-1 == fileSize)
	{
		sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, sys_getString ("Unable to get file size [ %s ]", fileName.c_str ()));
		return nullptr;
	}

	rawFileData = sys_malloc (static_cast<int>(fileSize), fileName);  // TODO Needs to be free'd

//	std::unique_ptr<char *>rawFileData = static_cast<const std::unique_ptr<char *> &>(sys_malloc (static_cast<int>(fileSize), fileName));  // TODO Needs to be free'd
	if (nullptr == rawFileData)
	{
		sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, sys_getString ("Unable to get memory to hold file [ %s ]", fileName.c_str ()));
		sys_shutdownWithError (sys_getString ("Unable to get memory to hold file [ %s ]", fileName.c_str ()));
	}
	//
	// Load into memory
	fileSystem.getFileIntoMemory (fileName, rawFileData);

	memFilePtr = SDL_RWFromMem (rawFileData, static_cast<int>(fileSize));

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
