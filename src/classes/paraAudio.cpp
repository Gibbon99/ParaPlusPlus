#include <vector>
#include <iostream>
#include "classes/paraAudio.h"

//-----------------------------------------------------------------------------------------------------------------------
//
// Constructor
paraAudio::paraAudio ()
//-----------------------------------------------------------------------------------------------------------------------
{
	Mix_Init (0);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Deconstructor
paraAudio::~paraAudio ()
//-----------------------------------------------------------------------------------------------------------------------
{
	Mix_Quit ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Pass in string and parameters to format and return a string
// https://stackoverflow.com/questions/19009094/c-variable-arguments-with-stdstring-only
std::string paraAudio::int_getString (std::string format, ...)
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

	// return a formatted string without risking memory mismanagement and without assuming any compiler
	// or platform specific behavior
	std::vector<char> zc (iLen + 1);
	std::vsnprintf (zc.data (), zc.size (), zcFormat, vaArgs);
	va_end(vaArgs);

	return std::string (zc.data ());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set a function to call when displaying any output
void paraAudio::setOutputFunction(functionPtrStr outputFunction)
//----------------------------------------------------------------------------------------------------------------------
{
	paraAudio::funcOutput = outputFunction;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Init the audio system
int paraAudio::init (functionPtrStr outputFunction, functionPtrLoad loadFunction)
//-----------------------------------------------------------------------------------------------------------------------
{
	setOutputFunction(outputFunction);
	paraAudio::funcLoad = loadFunction;

	// Open 44.1KHz, signed 16bit, system byte order,
	// stereo audio, using 1024 byte chunks
	if (Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		funcOutput(int_getString("Unable to open audio - [ %s ]", Mix_GetError ()));
		return -1;
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Play a sound file - record the channel it is playing on - -1 for error
int paraAudio::play (std::string keyName, bool loop)
//-----------------------------------------------------------------------------------------------------------------------
{
	if (audio[keyName].keyName == keyName)
	{
		if (!audio[keyName].loaded)
			return -1;
	}

	audio[keyName].playingOnChannel = Mix_PlayChannel(-1, audio[keyName].audio, loop ? -1 : 0);

	std::cout << "Playing : " << keyName << std::endl;;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Load a pointer to memory as a MIX_Chunk
bool paraAudio::load (std::string keyName, std::string fileName)
//-----------------------------------------------------------------------------------------------------------------------
{
	__audio     tempAudio;

	if ((keyName.empty()) || (fileName.empty()))
	{
		funcOutput(int_getString("Missing information for parameters [ %s ] [ %s ]", keyName.c_str(), fileName.c_str()));
		return -1;
	}

	tempAudio.fileName = fileName;
	tempAudio.keyName = keyName;
	tempAudio.audio = Mix_LoadWAV_RW(funcLoad(fileName), 0);
	if (nullptr != tempAudio.audio)
	{
		tempAudio.loaded = true;
		audio.insert(std::pair<std::string,__audio>(keyName, tempAudio));
#ifdef MY_DEBUG
		std::cout << "Loaded file : " << fileName << std::endl;
#endif
		return true;
	}
	return false;
}