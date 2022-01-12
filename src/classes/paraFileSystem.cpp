#include <vector>
#include <cstdarg>
#include <SDL_rwops.h>
#include "classes/paraFileSystem.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Set a function to call when displaying any output
void paraFileSystem::setOutputFunction(filesystemFuncPtrIntStr outputFunction)
//----------------------------------------------------------------------------------------------------------------------
{
	funcOutput = outputFunction;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set a function to call when allocating memory
void paraFileSystem::setMallocFunction(functionPtrMalloc mallocFunctionPtr)
//----------------------------------------------------------------------------------------------------------------------
{
	mallocFunction = mallocFunctionPtr;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Pass in string and parameters to format and return a string
// https://stackoverflow.com/questions/19009094/c-variable-arguments-with-stdstring-only
std::string paraFileSystem::int_getString(std::string format, ...)
//----------------------------------------------------------------------------------------------------------------------
{
	const char *const zcFormat = format.c_str ();

	// initialize use of the variable argument array
	va_list vaArgs;
	va_start(vaArgs, format);

	// reliably acquire the size from a copy of the variable argument array
	// and a functionally reliable call to mock the formatting
	va_list vaCopy;
	va_copy(vaCopy, vaArgs);
	const int iLen = std::vsnprintf (nullptr, 0, zcFormat, vaCopy);
	va_end(vaCopy);

	// return a formatted string without risking memory mismanagement  and without assuming any compiler
	// or platform specific behavior
	std::vector<char> zc (iLen + 1);
	std::vsnprintf (zc.data (), zc.size (), zcFormat, vaArgs);
	va_end(vaArgs);

	return std::string (zc.data ());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Start the packfile system
bool paraFileSystem::init(const std::string &baseDirectory, const std::string &writeDirectory)
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_Version compiled;
	PHYSFS_Version linked;

	if (PHYSFS_init (nullptr) == 0)
	{
		funcOutput (-1, int_getString ("Error: Filesystem failed to start - [ %s ]", PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		fileSystemReady = false;
		return false;
	}

	PHYSFS_VERSION(&compiled);
	PHYSFS_getLinkedVersion (&linked);

	std::string outputTest;
	outputTest = int_getString ("Compiled against PhysFS version %d.%d.%d.", compiled.major, compiled.minor, compiled.patch);

	funcOutput (-1, int_getString ("%s", outputTest.c_str ()));
	funcOutput (-1, int_getString ("Linked against PhysFS version %d.%d.%d.", linked.major, linked.minor, linked.patch));

	//
	// Setup directory to write if needed
	if (0 == PHYSFS_setWriteDir (writeDirectory.c_str ()))
	{
		funcOutput (-1, int_getString ("Failed to set write path [ %s ] - [ %s ]", writeDirectory.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		fileSystemReady = false;
		return false;
	}
	//
	// Set base directory
	if (0 == PHYSFS_mount (baseDirectory.c_str (), "/", 1))
	{
		funcOutput (-1, int_getString ("Failed to set search path [ %s ] - [ %s ]", baseDirectory.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));

		funcOutput (-1, int_getString ("The directory [ %s ] holding all the data files is not present. Check the installation.", baseDirectory.c_str ()));

		fileSystemReady = false;
		return false;
	}

	//
	// What compression types are available
	//	io_getArchivers();

	fileSystemReady = true;

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
	if (0 == PHYSFS_mount (newDirectory.c_str (), "/", 1))
	{
		funcOutput (-1, int_getString ("Failed to set search path - [ %s ] - [ %s ]", newDirectory.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		fileSystemReady = false;
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the length of a file
PHYSFS_sint64 paraFileSystem::getFileSize(std::string fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_file   *compFile;
	PHYSFS_sint64 fileLength;

	if (!fileSystemReady)
	{
		funcOutput (-1, int_getString ("PHYSFS system has not been initialised. Can't process [ %s ].", fileName.c_str ()));
		return -1;
	}

	//
	// Get a handle to the file
	compFile = PHYSFS_openRead (fileName.c_str ());

	if (nullptr == compFile)
	{
		funcOutput (-1, int_getString ("Filesystem can't open file [ %s ] - [ %s ].", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return -1;
	}

	//
	// Get it's size
	fileLength = PHYSFS_fileLength (compFile);

	if (-1 == fileLength)
	{
		funcOutput (-1, int_getString ("Unable to determine file length for [ %s ] - [ %s ].", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return -1;
	}

	//
	// Finished with it now so close it
	PHYSFS_close (compFile);

	return fileLength;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load a file into a pointer
std::string paraFileSystem::getString(std::string fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_file   *compFile = nullptr;
	PHYSFS_sint64 fileLength;
	char          *results;
	std::string   stringResult;

	if (!fileSystemReady)
	{
		funcOutput (-1, int_getString ("PHYSFS system has not been initialised. Can't load [ %s ].", fileName.c_str ()));
		return "";
	}

	//
	// Get a handle to the file
	compFile = PHYSFS_openRead (fileName.c_str ());

	if (nullptr == compFile)
	{
		funcOutput (-1, int_getString ("Filesystem can't open file [ %s ] - [ %s ].", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return "";
	}

	//
	// Get it's size
	fileLength = PHYSFS_fileLength (compFile);

	if (-1 == fileLength)
	{
		funcOutput (-1, int_getString ("Unable to determine file length for [ %s ] - [ %s ].", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		PHYSFS_close (compFile);
		return "";
	}

	results = mallocFunction (fileLength, fileName);
	if (nullptr == results)
	{
		funcOutput (-1, int_getString ("Memory error. Allocation failed for file [ %s ]", fileName.c_str ()));
		return "";
	}

	//
	// Read contents of file into the pointer
	PHYSFS_sint64 returnCode = PHYSFS_readBytes (compFile, static_cast<void *>(results), fileLength);

	if (-1 == returnCode)
	{
		funcOutput (-1, int_getString ("Filesystem read failed - [ %s ] for [ %s ].", PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ()), fileName.c_str ()));
		PHYSFS_close (compFile);
		return "";
	}

	//
	// Finished with it now so close it
	PHYSFS_close (compFile);

	for (int i = 0; i != fileLength; i++)
	{
		stringResult.push_back (results[i]);
	}

	return stringResult;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load a file into a pointer - data is stored in by reference variable results
int paraFileSystem::getFileIntoMemory(const std::string &fileName, void *results)
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_file   *compFile = nullptr;
	PHYSFS_sint64 fileLength;

	if (!fileSystemReady)
	{
		funcOutput (-1, int_getString ("PHYSFS system has not been initialised. Can't load [ %s ].", fileName.c_str ()));
		return -1;
	}

	//
	// Get a handle to the file
	compFile = PHYSFS_openRead (fileName.c_str ());

	if (nullptr == compFile)
	{
		funcOutput (-1, int_getString ("Filesystem can't open file [ %s ] - [ %s ].", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return -1;
	}

	//
	// Get it's size
	fileLength = PHYSFS_fileLength (compFile);

	if (-1 == fileLength)
	{
		funcOutput (-1, int_getString ("Unable to determine file length for [ %s ] - [ %s ].", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		PHYSFS_close (compFile);
		return -1;
	}

	//
	// Read contents of file into the pointer
	PHYSFS_sint64 returnCode = PHYSFS_readBytes (compFile, static_cast<void *>(results), fileLength);

	if (-1 == returnCode)
	{
		funcOutput (-1, int_getString ("Filesystem read failed - [ %s ] for [ %s ].", PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ()), fileName.c_str ()));
		PHYSFS_close (compFile);
		return -1;
	}

	//
	// Finished with it now so close it
	PHYSFS_close (compFile);

	return 1;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Check if a file exists
bool paraFileSystem::doesFileExist(const std::string &fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	if (!fileSystemReady)
	{
		funcOutput (-1, int_getString ("File system not ready. Can not check for file [ %s ]", fileName.c_str ()));
		return false;
	}

	return static_cast<bool>(PHYSFS_exists (fileName.c_str ()));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Write a file to disk
bool paraFileSystem::save(const std::string &fileName, const void *buffer, PHYSFS_uint32 size)
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_file   *fileHandle;
	PHYSFS_sint64 returnCode;

	fileHandle = PHYSFS_openWrite (fileName.c_str ());
	if (fileHandle == nullptr)
	{
		funcOutput (-1, int_getString ("Unable to open file for writing [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return false;
	}

	returnCode = PHYSFS_writeBytes (fileHandle, buffer, size);
	if (returnCode < size)
	{
		funcOutput (-1, int_getString ("Incomplete file write [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return false;
	}

	PHYSFS_close (fileHandle);

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Display the current search path
void paraFileSystem::getSearchPath()
//----------------------------------------------------------------------------------------------------------------------
{
	char **i;
	for (i = PHYSFS_getSearchPath (); *i != nullptr; i++)
	{
		funcOutput (-1, int_getString ("[%s] is in the search path.", *i));
	}
}