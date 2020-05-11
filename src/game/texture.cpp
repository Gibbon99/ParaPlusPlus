#include <string>
#include <SDL_rwops.h>
#include <io/fileSystem.h>
#include <io/console.h>
#include "game/texture.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Create a collision map for passed in keyName
void gam_createCollisionMap(std::string &keyName)
//----------------------------------------------------------------------------------------------------------------------
{
	textures.at(keyName).createMap();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load a texture passing in filename from script - replace if it already exists
void gam_loadTexture(std::string &fileName, std::string &keyName)
//----------------------------------------------------------------------------------------------------------------------
{
	paraTexture tempTexture(con_addEvent, io_loadRawFile);

	if (tempTexture.load(fileName, keyName))
	{
		auto textureItr = textures.find(keyName);
		if (textureItr != textures.end())
			textures.erase(textureItr);

		tempTexture.setFileName(fileName);
		textures.insert (std::pair<std::string, paraTexture> (keyName, tempTexture));
	}
	else
		sys_shutdownWithError(sys_getString("Unable to load texture [ %s ]", fileName.c_str()));
}

