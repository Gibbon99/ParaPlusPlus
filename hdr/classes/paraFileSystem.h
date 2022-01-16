#pragma once

#include <string>
#include "../physfs/physfs.h"

typedef void      (*filesystemFuncPtrIntStr)(int, std::string);

typedef void      (*filesystemFuncPtrStr)(int, std::string);

typedef char *(*functionPtrMalloc)(int, std::string);

class paraFileSystem
{
public:
	bool init(const std::string &baseDirectory, const std::string &writeDirectory);

	bool addPath(const std::string &newDirectory);

	PHYSFS_sint64 getFileSize(std::string fileName);

	int getFileIntoMemory(const std::string &fileName, void *results);

	bool doesFileExist(const std::string &fileName);

	std::string getString(const std::string& fileName);

	std::string int_getString(std::string format, ...);

	void setOutputFunction(filesystemFuncPtrIntStr outputFunction);

	void setMallocFunction(functionPtrMalloc mallocFunctionPtr);

	bool save(const std::string &fileName, const void *buffer, PHYSFS_uint32 size);

	void getSearchPath();

private:
	bool                    fileSystemReady = false;
	filesystemFuncPtrStr    funcShutdown;
	filesystemFuncPtrIntStr funcOutput;
	functionPtrMalloc       mallocFunction;
};