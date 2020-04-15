#include "../../hdr/io/fileSystem.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Find the names of all the files in the scripts directory and store ready for loading
void io_getScriptFileNames(const std::string& directoryName)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string tempFileName;

	char **rc = PHYSFS_enumerateFiles(directoryName.c_str());
	char **i;
	//
	// Store the file names into an array
	for (i = rc; *i != nullptr; i++)
	{
		tempFileName = *i;

		paraScriptInstance.addScript(logFile, tempFileName, fileSystem.getString (logFile, tempFileName));

		logFile.write(sys_getString("Script [ %s ]", tempFileName.c_str()));
	}

	PHYSFS_freeList(rc);
}