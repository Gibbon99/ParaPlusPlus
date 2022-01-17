#pragma once

#include <string>

// Load a texture passing in filename from script
void gam_loadTexture(std::string &fileName, std::string &keyName);

// Free all the textures
void gam_freeTextures();