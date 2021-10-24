#pragma once

#include <string>
#include <fstream>
#include <utility>

class paraLogFile
{

public:
	bool open(std::string fileName);

	void close();

	void write(const std::string &logText);

	bool enabled() const;

private:
	std::string   logFileName;
	bool          fileLoggingOn = false;
	std::ofstream outFile;
};