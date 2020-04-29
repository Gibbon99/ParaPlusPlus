#pragma once

#include "../../hdr/main.h"

// Start the audio processing thread
void gam_initAudio();

// Put a new audio event onto the queue
void gam_addAudioEvent(int action, bool loop, int distance, int pan, std::string keyName);