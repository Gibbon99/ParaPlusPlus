#include <string>
#include <SDL_rwops.h>
#include "io/fileSystem.h"
#include "io/console.h"
#include "system/util.h"
#include "game/texture.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Load a texture passing in filename from script - replace if it already exists
void gam_loadTexture(std::string &fileName, std::string &keyName)
//----------------------------------------------------------------------------------------------------------------------
{
	paraTexture tempTexture (con_addEvent, io_loadRawFile);

	if (tempTexture.load (fileName, keyName))
	{
		tempTexture.setFileName (fileName);

		auto textureItr = textures.find (keyName);
		if (textureItr != textures.end ())
		{
			textureItr->second.destroy ();
			textures.erase (textureItr);
		}

		tempTexture.setFileName (fileName);
		textures.insert (std::pair<std::string, paraTexture> (keyName, tempTexture));
	}
	else
		sys_shutdownWithError (sys_getString ("Unable to load texture [ %s ]", fileName.c_str ()));
}

