#include "../../hdr/classes/paraLogFile.h"

bool paraLogFile::open(std::string fileName)
{
    logFileName = std::move(fileName);

    paraLogFile::outFile.open(logFileName);
    if (!paraLogFile::outFile)
        return false;

    fileLoggingOn = true;
    return true;
}

void paraLogFile::write(const std::string& logText)
{
	paraLogFile::outFile << logText << std::endl;
}

void paraLogFile::close()
{
    flush(outFile);
	paraLogFile::outFile.close();
}

bool paraLogFile::enabled() const
{
    return fileLoggingOn;
}
