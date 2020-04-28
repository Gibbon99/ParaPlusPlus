#pragma once

#include "../../hdr/main.h"

// Load function callback for the audio class - this function loads a file and returns a pointer to memory
SDL_RWops * io_loadAudioFile(std::string fileName);