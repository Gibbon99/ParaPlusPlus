#include <main.h>
#include "../../hdr/classes/paraLogFile.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Open the logfile
bool paraLogFile::open(std::string fileName)
//----------------------------------------------------------------------------------------------------------------------
{
	logFileName = std::move (fileName);

	paraLogFile::outFile.open (logFileName);
	if (!paraLogFile::outFile)
		return false;

	fileLoggingOn = true;
	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Write a string to the open logfile
void paraLogFile::write(const std::string &logText)
//----------------------------------------------------------------------------------------------------------------------
{
	if (paraLogFile::enabled ())
		paraLogFile::outFile << sys_getCurrentTime () << " - " << logText << std::endl;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Flush the ofstream and then close it
void paraLogFile::close()
//----------------------------------------------------------------------------------------------------------------------
{
	if (paraLogFile::enabled ())
	{
		flush (outFile);
		paraLogFile::outFile.close ();
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Check if file logging is available
bool paraLogFile::enabled() const
//----------------------------------------------------------------------------------------------------------------------
{
	return fileLoggingOn;
}
