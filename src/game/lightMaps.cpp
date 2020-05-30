#include <classes/paraLightmap.h>
#include "game/lightMaps.h"

std::vector<paraLightmap>       lightMaps;

//-----------------------------------------------------------------------------------------------------------
//
// Remove a lightmap - mark inUse to false - pass in bulletID to match
void gam_removeLightmap(Uint32 whichBullet)
//-----------------------------------------------------------------------------------------------------------
{
	for (auto &lightMapItr : lightMaps)
	{
		if (lightMapItr.getAttachedBullet() == whichBullet)
		{
			lightMapItr.setInUseState(false);
			return;
		}
	}
}

//-----------------------------------------------------------------------------------------------------------
//
// Add a new lightmap to the array
void gam_addNewLightmap(b2Vec2 newWorldPosition, int newType, Uint32 newWhichBullet)
//-----------------------------------------------------------------------------------------------------------
{
	paraLightmap    tempLightmap(newWorldPosition, newType, newWhichBullet);

	if (lightMaps.empty())
	{
		lightMaps.push_back(tempLightmap);
		return;
	}

	for (auto &lightMapItr : lightMaps)
	{
		if (!lightMapItr.inUse())
		{
			lightMapItr = tempLightmap;
			return;
		}
	}

	lightMaps.push_back(tempLightmap);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Render the lightmaps
void gam_renderLightmaps()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &lightMapItr : lightMaps)
	{
		if (lightMapItr.inUse())
			lightMapItr.render();
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Animate the lightmaps
void gam_animateLightmaps()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &lightMapItr : lightMaps)
	{
		if (lightMapItr.inUse())
			lightMapItr.animate ();
	}
}