#pragma once

#include <string>

// Load a texture passing in filename from script
void gam_loadTexture(std::string &fileName, std::string &keyName);

// Create a collision map for passed in keyName
void gam_createCollisionMap(std::string &keyName);
