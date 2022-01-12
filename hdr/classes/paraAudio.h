#pragma once

#include <map>
#include <SDL_mixer.h>

#define PARA_MAX_VOLUME 10

typedef void      (*audioFunctionPtrStr)(int, std::string);

typedef SDL_RWops *(*audioFunctionPtrLoad)(std::string);

struct __audio
{
	bool        loaded           = false;
	std::string fileName         = "";
	std::string keyName          = "";
	Mix_Chunk   *audio           = nullptr;
	int         playingOnChannel = -2;   // The Channel this sound is playing on
};

struct __audioActiveSounds
{
	bool        doorSound;
	int         whichChannel;
	std::string keyName;
};

class paraAudio
{
public:

	paraAudio();

	~paraAudio();

	void AddRef();

	void ReleaseRef();

	int init(int numMaxActiveChannels, audioFunctionPtrStr outputFunction, audioFunctionPtrLoad loadFunction);

	bool load(std::string fileName);

	int getMasterVolume();

	void stopAllChannels();

	int getNumPlayingChannels();

	// Returns true if current sound is playing
	bool isPlaying(const std::string &keyName);

	void setMasterVolume(int volume);

	void pauseAllChannels();

	void resumeAllChannels();

	void deviceInfo();

//	bool pause (std::string keyName);

	std::string int_getString(std::string format, ...);

	int play(std::string keyName, bool loop, int distance, int pan);

	void stop(std::string keyName);

	void setOutputFunction(audioFunctionPtrStr outputFunction);

	// Special case - handle number of door sounds playing
	bool handleDoorSounds(int distance, int pan);

private:
	std::vector<__audioActiveSounds> activeSounds;
	std::map<std::string, __audio>   audio;
	audioFunctionPtrStr              funcOutput {};
	audioFunctionPtrLoad             funcLoad {};
	bool                             audioDeviceOpened    = false;
	int                              currentVolumeLevel   = 0;
	int                              maxNumActiveChannels = 0;
};