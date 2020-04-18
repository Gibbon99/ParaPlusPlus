#include "../../hdr/classes/paraFileSystem.h"
#include "../../hdr/system/util.h"
#include "../../hdr/io/logFile.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Start the packfile system
bool paraFileSystem::init(const std::string &baseDirectory, const std::string &writeDirectory)
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_Version compiled;
	PHYSFS_Version linked;

	if (PHYSFS_init(nullptr) == 0)
	{
		log_addEvent(sys_getString("Error: Filesystem failed to start - [ %s ]", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		paraFileSystem::fileSystemReady = false;
		return false;
	}

	PHYSFS_VERSION (&compiled);
	PHYSFS_getLinkedVersion(&linked);

	log_addEvent(sys_getString("Compiled against PhysFS version %d.%d.%d.", compiled.major, compiled.minor, compiled.patch));
	log_addEvent(sys_getString("Linked against PhysFS version %d.%d.%d.", linked.major, linked.minor, linked.patch));

	//
	// Setup directory to write if needed
	if (0 == PHYSFS_setWriteDir(writeDirectory.c_str()))
	{
		log_addEvent(sys_getString("Failed to set write path [ %s ] - [ %s ]", writeDirectory.c_str(),
		                            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		paraFileSystem::fileSystemReady = false;
		return false;
	}
	//
	// Set base directory
	if (0 == PHYSFS_mount(baseDirectory.c_str(), "/", 1))
	{
		log_addEvent(sys_getString("Failed to set search path [ %s ] - [ %s ]", baseDirectory.c_str(),
		                            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));

		log_addEvent(sys_getString("The directory [ %s ] holding all the data files is not present. Check the installation.",
		                            baseDirectory.c_str()));

		paraFileSystem::fileSystemReady = false;
		return false;
	}

	//
	// What compression types are available
//	io_getArchivers();

	paraFileSystem::fileSystemReady = true;

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add another directory to use in seach path
bool paraFileSystem::addPath(const std::string &newDirectory)
//----------------------------------------------------------------------------------------------------------------------
{
	//
	// Add archive file
	if (0 == PHYSFS_mount(newDirectory.c_str(), "/", 1))
	{
		log_addEvent(sys_getString("Failed to set search path - [ %s ] - [ %s ]", newDirectory.c_str(),
		                            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		paraFileSystem::fileSystemReady = false;
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the length of a file
PHYSFS_sint64 paraFileSystem::getFileSize(const std::string &fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_file   *compFile;
	PHYSFS_sint64 fileLength;

	if (!paraFileSystem::fileSystemReady)
	{
		log_addEvent(sys_getString("PHYSFS system has not been initialised. Can't process [ %s ].", fileName.c_str()));
		return -1;
	}

	//
	// Get a handle to the file
	compFile = PHYSFS_openRead(fileName.c_str());

	if (nullptr == compFile)
	{
		log_addEvent(sys_getString("Filesystem can't open file [ %s ] - [ %s ].", fileName.c_str(),
		                            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		return -1;
	}

	//
	// Get it's size
	fileLength = PHYSFS_fileLength(compFile);

	if (-1 == fileLength)
	{
		log_addEvent(sys_getString("Unable to determine file length for [ %s ] - [ %s ].", fileName.c_str(),
		                            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		return -1;
	}

	//
	// Finished with it now so close it
	PHYSFS_close(compFile);

	return fileLength;
}


//PHYSFS_sint64 PHYSFS_readBytes(
//		PHYSFS_File *handle, void *buffer,
//		PHYSFS_uint64 len);

//----------------------------------------------------------------------------------------------------------------------
//
// Load a file into a pointer
std::string paraFileSystem::getString(const std::string &fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_file   *compFile = nullptr;
	PHYSFS_sint64 fileLength;
	char           *results;
	std::string     stringResult;

	if (!paraFileSystem::fileSystemReady)
	{
		log_addEvent(sys_getString("PHYSFS system has not been initialised. Can't load [ %s ].", fileName.c_str()));
		return "";
	}

	//
	// Get a handle to the file
	compFile = PHYSFS_openRead(fileName.c_str());

	if (nullptr == compFile)
	{
		log_addEvent(sys_getString("Filesystem can't open file [ %s ] - [ %s ].", fileName.c_str(),
		                            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		return "";
	}

	//
	// Get it's size
	fileLength = PHYSFS_fileLength(compFile);

	if (-1 == fileLength)
	{
		log_addEvent(sys_getString("Unable to determine file length for [ %s ] - [ %s ].", fileName.c_str(),
		                            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		PHYSFS_close(compFile);
		return "";
	}

	results = sys_malloc(fileLength, fileName);
	if (nullptr == results)
	{
		log_addEvent(sys_getString("Memory error. Allocation failed for file [ %s ]", fileName.c_str()));
		return "";
	}

	//
	// Read contents of file into the pointer
	PHYSFS_sint64 returnCode = PHYSFS_readBytes(compFile, (void *) results, fileLength);

	if (-1 == returnCode)
	{
		log_addEvent(sys_getString("Filesystem read failed - [ %s ] for [ %s ].", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()),
		                            fileName.c_str()));
		PHYSFS_close(compFile);
		return "";
	}

	//
	// Finished with it now so close it
	PHYSFS_close(compFile);

	stringResult = results;

	return stringResult;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load a file into a pointer
int paraFileSystem::getFileIntoMemory(const std::string &fileName, void *results)
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_file   *compFile = nullptr;
	PHYSFS_sint64 fileLength;

	if (!paraFileSystem::fileSystemReady)
	{
		log_addEvent(sys_getString("PHYSFS system has not been initialised. Can't load [ %s ].", fileName.c_str()));
		return -1;
	}

	//
	// Get a handle to the file
	compFile = PHYSFS_openRead(fileName.c_str());

	if (nullptr == compFile)
	{
		log_addEvent(sys_getString("Filesystem can't open file [ %s ] - [ %s ].", fileName.c_str(),
		                            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		return -1;
	}

	//
	// Get it's size
	fileLength = PHYSFS_fileLength(compFile);

	if (-1 == fileLength)
	{
		log_addEvent(sys_getString("Unable to determine file length for [ %s ] - [ %s ].", fileName.c_str(),
		                            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode())));
		PHYSFS_close(compFile);
		return -1;
	}

	//
	// Read contents of file into the pointer
	PHYSFS_sint64 returnCode = PHYSFS_readBytes(compFile, (void *) results, fileLength);

	if (-1 == returnCode)
	{
		log_addEvent(sys_getString("Filesystem read failed - [ %s ] for [ %s ].", PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()),
		                            fileName.c_str()));
		PHYSFS_close(compFile);
		return -1;
	}

	//
	// Finished with it now so close it
	PHYSFS_close(compFile);

	return 1;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Check if a file exists
bool paraFileSystem::doesFileExist(const std::string &fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (!paraFileSystem::fileSystemReady)
	{
		log_addEvent(sys_getString("File system not ready. Can not check for file [ %s ]", fileName.c_str()));
		return false;
	}

	return static_cast<bool>(PHYSFS_exists(fileName.c_str()));
}