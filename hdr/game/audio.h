#pragma once

#include "../../hdr/main.h"

// Load function callback for the audio class - this function loads a file and returns a pointer to memory
SDL_RWops * io_loadAudioFile(std::string fileName);

// Start the audio processing thread
void gam_initAudio();

// Put a new audio event onto the queue
void gam_addAudioEvent(int action, bool loop, int distance, int pan, std::string keyName);