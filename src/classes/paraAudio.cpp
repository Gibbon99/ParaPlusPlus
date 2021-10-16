#include <vector>
#include <iostream>
#include <system/startup.h>
#include <system/util.h>
#include "classes/paraAudio.h"

//#define AUDIO_DEBUG 1

#define MAX_NUM_CHANNELS    64
#define DEFAULT_NUM_CHANNELS    16
#define numDoorSounds 5

//-----------------------------------------------------------------------------------------------------------------------
//
// Constructor
paraAudio::paraAudio()
//-----------------------------------------------------------------------------------------------------------------------
{
	Mix_Init (0);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Deconstructor
paraAudio::~paraAudio()
//-----------------------------------------------------------------------------------------------------------------------
{
	Mix_Quit ();

	auto audioItr = audio.begin ();

	while (audioItr != audio.end ())
	{
		free (audioItr->second.audio);

		audioItr++;
	}

}

void paraAudio::AddRef()
{
}

void paraAudio::ReleaseRef()
{

}

//----------------------------------------------------------------------------------------------------------------------
//
// Pass in string and parameters to format and return a string
// https://stackoverflow.com/questions/19009094/c-variable-arguments-with-stdstring-only
std::string paraAudio::int_getString(std::string format, ...)
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

//-----------------------------------------------------------------------------------------------------------------------
//
// Init the audio system
int paraAudio::init(int numMaxActiveChannels, audioFunctionPtrStr outputFunction, audioFunctionPtrLoad loadFunction)
//-----------------------------------------------------------------------------------------------------------------------
{
	__audioActiveSounds tempActiveSounds;

	paraAudio::funcOutput = outputFunction;
	paraAudio::funcLoad   = loadFunction;

	audioDeviceOpened = false;

	// Open 44.1KHz, signed 16bit, system byte order, stereo audio, using 1024 byte chunks
	if (Mix_OpenAudio (44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
	{
		funcOutput (-1, int_getString ("Unable to open audio - [ %s ]", Mix_GetError ()));
		return -1;
	}

	for (auto i = 0; i < SDL_GetNumAudioDrivers (); ++i)
	{
		logFile.write (sys_getString ("%i: %s", i, SDL_GetAudioDriver (i)));
	}

	logFile.write (sys_getString ("Using audio driver: %s\n", SDL_GetCurrentAudioDriver ()));

	if ((numMaxActiveChannels < 0) || (numMaxActiveChannels > MAX_NUM_CHANNELS))
	{
		funcOutput (-1, int_getString ("Invalid number [ %i ] of max channels specified. Setting to %i.", numMaxActiveChannels, DEFAULT_NUM_CHANNELS));
		numMaxActiveChannels = DEFAULT_NUM_CHANNELS;  // set to reasonable default
	}

	maxNumActiveChannels = numMaxActiveChannels;
	Mix_AllocateChannels (maxNumActiveChannels);
	//
	// Fill up the active sounds with a channel number
	for (auto i = 0; i != maxNumActiveChannels; i++)
	{
		tempActiveSounds.whichChannel = i;
		tempActiveSounds.keyName      = "";

		activeSounds.push_back (tempActiveSounds);
	}
	audioDeviceOpened = true;

	return 0;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the number of channels that are currently playing
int paraAudio::getNumPlayingChannels()
//-----------------------------------------------------------------------------------------------------------------------
{
	return Mix_Playing (-1);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Pause all sounds from playing
void paraAudio::pauseAllChannels()
//-----------------------------------------------------------------------------------------------------------------------
{
	Mix_Pause (-1);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Resume playing all paused sounds
void paraAudio::resumeAllChannels()
//-----------------------------------------------------------------------------------------------------------------------
{
	Mix_Resume (-1);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Stop all currently playing channels
void paraAudio::stopAllChannels()
//-----------------------------------------------------------------------------------------------------------------------
{
	Mix_HaltChannel (-1);
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Returns true if current sound is playing
bool paraAudio::isPlaying(const std::string &keyName)
//-----------------------------------------------------------------------------------------------------------------------
{
	for (const auto &audioItr: activeSounds)
	{
		if (keyName == audioItr.keyName)
		{
			if (Mix_Playing (audioItr.whichChannel) == 1)
				return true;
		}
	}
	return false;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Return the current master volume level - to be saved in config file
int paraAudio::getMasterVolume()
//-----------------------------------------------------------------------------------------------------------------------
{
	return currentVolumeLevel;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Set the overall volume
// Use scale of 0 to 10 and map to 0 - MIX_MAX_VOLUME ( 128 )
void paraAudio::setMasterVolume(int volume)
//-----------------------------------------------------------------------------------------------------------------------
{
	if ((volume < 0) || (volume > PARA_MAX_VOLUME))
	{
		funcOutput (-1, int_getString ("Invalid volume level [ %i ]. Must be between 0 and %i", volume, PARA_MAX_VOLUME));
		return;
	}

	currentVolumeLevel = volume * (MIX_MAX_VOLUME / PARA_MAX_VOLUME);
	Mix_Volume (-1, currentVolumeLevel);
	g_volumeLevel = volume;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Stop playing a sound based on name
void paraAudio::stop(std::string keyName)
//-----------------------------------------------------------------------------------------------------------------------
{
	for (auto &audioItr: activeSounds)
	{
		if (keyName == audioItr.keyName)
		{
			Mix_HaltChannel (audioItr.whichChannel);
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Play a sound file - record the channel it is playing on - -1 for error
//
// Distance: 0 (close / loud ) to 255 (far / quiet)
//
// Panning: 254 for all left, 127 for center, 0 for all right
int paraAudio::play(std::string keyName, bool loop, int distance, int pan)
//-----------------------------------------------------------------------------------------------------------------------
{
	static int numDoorsPlaying = 0;

	if ((distance < 0) || (distance > 255))
		distance = 128;

	if ((pan < 0) || (pan > 255))
		pan = 127;

	auto audioItr = audio.find (keyName);

	if (audioItr != audio.end ())
	{
		if (!audio[keyName].loaded)
		{
			funcOutput (-1, int_getString ("File [ %s ] is not loaded", keyName.c_str ()));
			return -1;
		}

		for (auto &activeItr: activeSounds)
		{
			if (Mix_Playing (activeItr.whichChannel) == 0)   // Channel is not playing
			{
				if (activeItr.doorSound)        // If it was a door sound - decrement counter TODO - Check this. Other sounds are triggering this
					numDoorsPlaying--;

				Mix_UnregisterAllEffects (activeItr.whichChannel);       // Reset any effects from previous sounds

				if (!Mix_SetPanning (activeItr.whichChannel, pan, 254 - pan))
					funcOutput (-1, int_getString ("Failed to set panning for [ %s ] - [ %s ]", keyName.c_str (), Mix_GetError ()));

				if (!Mix_SetDistance (activeItr.whichChannel, distance))
					funcOutput (-1, int_getString ("Failed to set attenuation for [ %s ] - [ %s ]", keyName.c_str (), Mix_GetError ()));

				if ((keyName == "doorOpen") || (keyName == "doorClose"))
				{
					if (numDoorsPlaying > numDoorSounds)
					{
						activeItr.doorSound = false;
						return -1;
					}

					numDoorsPlaying++;
					activeItr.doorSound = true;

					activeItr.whichChannel = Mix_PlayChannel(activeItr.whichChannel, audio[keyName].audio, loop ? -1 : 0);
					activeItr.keyName      = keyName;
#ifdef AUDIO_DEBUG
					std::cout << "Playing : " << keyName << " on channel : " << activeItr.whichChannel << std::endl;
#endif
					return activeItr.whichChannel;
				}
				else
				{
					activeItr.doorSound    = false;
					activeItr.whichChannel = Mix_PlayChannel(activeItr.whichChannel, audio[keyName].audio, loop ? -1 : 0);
					activeItr.keyName      = keyName;
#ifdef AUDIO_DEBUG
					std::cout << "Playing : " << keyName << " on channel : " << activeItr.whichChannel << std::endl;
#endif
					return activeItr.whichChannel;
				}
			}
		}
		funcOutput (-1, int_getString ("Unable to play sound [ %s ]. No active channels left.", keyName.c_str ()));
		return -1;
	}
	funcOutput (-1, int_getString ("File [ %s ] has not been loaded.", keyName.c_str ()));
	return -1;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Load a pointer to memory as a MIX_Chunk
//
// Any extension on the filename is removed, and the remainder becomes the keyName index into the array
bool paraAudio::load(std::string fileName)
//-----------------------------------------------------------------------------------------------------------------------
{
	__audio tempAudio;

	if (fileName.empty ())
	{
		funcOutput (-1, int_getString ("Audio load - missing filename."));
		return false;
	}

	tempAudio.fileName = fileName;
	tempAudio.keyName  = fileName.erase (fileName.find_last_of ("."), std::string::npos);;
	tempAudio.audio = Mix_LoadWAV_RW (funcLoad (tempAudio.fileName), 0);
	if (nullptr == tempAudio.audio)
	{
		funcOutput (-1, int_getString ("Unable to load audio file [ %s ]", tempAudio.fileName.c_str ()));
		return false;
	}
	tempAudio.loaded = true;
	audio.insert (std::pair<std::string, __audio> (tempAudio.keyName, tempAudio));

#ifdef MY_DEBUG
	funcOutput (-1, int_getString ("Loaded [ %s ] with key [ %s ]", tempAudio.fileName.c_str (), tempAudio.keyName.c_str ()));
	std::cout << "Loaded file : " << tempAudio.fileName << std::endl;
#endif

	return true;
}

//-----------------------------------------------------------------------------------------------------------------------
//
// Get the actual details used by the sound device
void paraAudio::deviceInfo()
//-----------------------------------------------------------------------------------------------------------------------
{
	int         numTimesOpened = 0;
	int         frequency      = 0;
	int         numChannels    = 0;
	Uint16      format         = 0;
	std::string formatStr;

	if (!audioDeviceOpened)
	{
		funcOutput (-1, int_getString ("The Audio device has not been opened yet. Unable to query specs."));
		return;
	}

	numTimesOpened = Mix_QuerySpec (&frequency, &format, &numChannels);
	if (!numTimesOpened)
	{
		funcOutput (-1, int_getString ("Error getting audio device specs [ %s ]", Mix_GetError ()));
		return;
	}

	switch (format)
	{
		case AUDIO_U8:
			formatStr = "U8";
			break;
		case AUDIO_S8:
			formatStr = "S8";
			break;
		case AUDIO_U16LSB:
			formatStr = "U16LSB";
			break;
		case AUDIO_S16LSB:
			formatStr = "S16LSB";
			break;
		case AUDIO_U16MSB:
			formatStr = "U16MSB";
			break;
		case AUDIO_S16MSB:
			formatStr = "S16MSB";
			break;
		default:
			formatStr = "Unknown";
			break;
	}

	funcOutput (-1, int_getString ("Opened [ %i ] times. Frequency [ %i Hz ] Format [ %s ] Channels [ %i ]", numTimesOpened, frequency, formatStr.c_str (), numChannels));
}