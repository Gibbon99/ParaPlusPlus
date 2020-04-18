#ifndef PARA_PARAFILESYSTEM_H
#define PARA_PARAFILESYSTEM_H

#include <string>
#include "paraLogFile.h"
#include "../physfs/physfs.h"

class paraFileSystem
{
public:
	bool init(const std::string &baseDirectory, const std::string &writeDirectory);

	bool addPath(const std::string &newDirectory);

	PHYSFS_sint64 getFileSize(const std::string &fileName);

	int getFileIntoMemory(const std::string &fileName, void *results);

	bool doesFileExist(const std::string &fileName);

	std::string getString(const std::string &fileName);

private:
	bool fileSystemReady = false;
};

#endif //PARA_PARAFILESYSTEM_H
