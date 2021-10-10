#include <string>
#include <SDL_rwops.h>
#include <io/fileSystem.h>
#include <io/console.h>
#include <system/util.h>
#include "game/texture.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Create a collision map for passed in keyName TODO Remove
void gam_createCollisionMap (std::string &keyName)
//----------------------------------------------------------------------------------------------------------------------
{
	try
	{
		textures.at (keyName).createMap ();
	}
	catch (std::out_of_range outOfRange)
	{
		sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_MAP, 1, "planet|planet| ");
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load a texture passing in filename from script - replace if it already exists
void gam_loadTexture (std::string &fileName, std::string &keyName)
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

