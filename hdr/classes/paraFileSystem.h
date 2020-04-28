#ifndef PARA_PARAFILESYSTEM_H
#define PARA_PARAFILESYSTEM_H

#include <string>
#include "../physfs/physfs.h"

typedef void (*functionPtrStr)(std::string);

typedef char *(*functionPtrMalloc)(int, std::string);

class paraFileSystem
{
public:
	bool init(const std::string &baseDirectory, const std::string &writeDirectory);

	bool addPath(const std::string &newDirectory);

	PHYSFS_sint64 getFileSize(std::string fileName);

	int getFileIntoMemory(std::string fileName, void *results);

	bool doesFileExist(const std::string &fileName);

	std::string getString (std::string fileName);

	std::string int_getString (std::string format, ...);

	void setOutputFunction (functionPtrStr outputFunction);

	void setMallocFunction (functionPtrMalloc mallocFunctionPtr);

private:
	bool              fileSystemReady = false;
	functionPtrStr    funcShutdown;
	functionPtrStr    funcOutput;
	functionPtrMalloc mallocFunction;
};

#endif //PARA_PARAFILESYSTEM_H
