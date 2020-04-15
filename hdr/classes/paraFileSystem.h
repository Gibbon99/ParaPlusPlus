#ifndef PARA_PARAFILESYSTEM_H
#define PARA_PARAFILESYSTEM_H

#include <string>
#include "paraLogFile.h"
#include "../physfs/physfs.h"

class paraFileSystem
{
public:
	bool init(paraLogFile& outFile, const std::string& baseDirectory, const std::string& writeDirectory);

	bool addPath(paraLogFile& outFile, const std::string& newDirectory);

	PHYSFS_sint64 getFileSize ( paraLogFile &outFile, const std::string &fileName );

	int getFileIntoMemory ( paraLogFile &outFile, const std::string &fileName, void *results );

	bool doesFileExist(paraLogFile &outFile, const std::string& fileName);

	std::string getString(paraLogFile &outFile, const std::string &fileName);

private:
	bool fileSystemReady = false;
};


#endif //PARA_PARAFILESYSTEM_H
