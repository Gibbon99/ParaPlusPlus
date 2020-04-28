//
// Created by dberry on 28/4/20.
//

#ifndef PARA_PARAAUDIO_H
#define PARA_PARAAUDIO_H

#include <SDL_mixer.h>
#include "io/resources.h"

typedef void (*functionPtrStr) (std::string);
typedef SDL_RWops *(*functionPtrLoad) (std::string);

struct __audio
{
	bool        loaded   = false;
	std::string fileName = "";
	std::string keyName  = "";
	Mix_Chunk   *audio   = nullptr;
	int         playingOnChannel = -2;   // The Channel this sound is playing on
};

class paraAudio
{
public:

	paraAudio ();

	~paraAudio ();

	int init (functionPtrStr outputFunction, functionPtrLoad loadFunction);

	bool load (std::string keyName, std::string fileName);

//	bool pause (std::string keyName);

	std::string int_getString (std::string format, ...);

	int play (std::string keyName, bool loop);

	void setOutputFunction (functionPtrStr outputFunction);

private:
	std::map<std::string, __audio> audio;
	functionPtrStr                 funcOutput{};
	functionPtrLoad                funcLoad{};
};

#endif //PARA_PARAAUDIO_H
