#include <game/database.h>
#include <game/shipDecks.h>
#include <system/util.h>
#include "game/droids.h"

droidClass testDroid;

//-------------------------------------------------------------------------------------------------------------
//
// Return the name of a sprite based on its type
std::string gam_getDroidName (int droidType)
//-------------------------------------------------------------------------------------------------------------
{
	switch (droidType)
	{
		case 1:
			return "123";
			break;
		case 2:
			return "139";
			break;
		case 3:
			return "247";
			break;
		case 4:
			return "249";
			break;
		case 5:
			return "296";
			break;
		case 6:
			return "302";
			break;
		case 7:
			return "329";
			break;
		case 8:
			return "420";
			break;
		case 9:
			return "476";
			break;
		case 10:
			return "493";
			break;
		case 11:
			return "516";
			break;
		case 12:
			return "571";
			break;
		case 13:
			return "598";
			break;
		case 14:
			return "614";
			break;
		case 15:
			return "615";
			break;
		case 16:
			return "629";
			break;
		case 17:
			return "711";
			break;
		case 18:
			return "742";
			break;
		case 19:
			return "751";
			break;
		case 20:
			return "821";
			break;
		case 21:
			return "834";
			break;
		case 22:
			return "883";
			break;
		case 23:
			return "999";
			break;
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Create the droids for the level and init values
void gam_initDroids (std::string levelName)
//-------------------------------------------------------------------------------------------------------------
{
	int        wayPointStartIndex   = 0;
	droidClass tempDroid;

	if (!shipdecks.at (levelName).droid.empty ())
	{
		shipdecks.at (levelName).droid.clear ();
		shipdecks.at (levelName).droid.resize (0);
	}

	shipdecks.at (levelName).numEnemiesAlive = shipdecks.at (levelName).numDroids;
	for (auto droidItr : shipdecks.at (levelName).droidTypes)
	{
		tempDroid.droidType                       = droidItr;
		tempDroid.worldPosInPixels.x              = shipdecks.at (levelName).wayPoints[wayPointStartIndex++].x;
		tempDroid.worldPosInPixels.y              = shipdecks.at (levelName).wayPoints[wayPointStartIndex++].y;
		if (wayPointStartIndex > shipdecks.at (levelName).numWaypoints)
			shipdecks.at (levelName).numWaypoints = 0;

		tempDroid.droidName    = gam_getDroidName (tempDroid.droidType);
		tempDroid.acceleration = dataBaseEntry[tempDroid.droidType].accelerate;
		tempDroid.maxSpeed     = dataBaseEntry[tempDroid.droidType].maxSpeed;

		tempDroid.currentMode = DROID_MODE_NORMAL;

		tempDroid.sprite.create (tempDroid.droidName, 9, 0.3);

		shipdecks.at (levelName).droid.push_back (tempDroid);
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Render the droids on the current level
void gam_renderDroids (std::string levelName)
//-------------------------------------------------------------------------------------------------------------
{
	paraVec2d droidScreenPosition{};

	for (auto droidItr : g_shipDeckItr->second.droid)
	{
		droidScreenPosition.x = droidItr.worldPosInPixels.x;
		droidScreenPosition.y = droidItr.worldPosInPixels.y;

		droidScreenPosition = sys_worldToScreen (droidScreenPosition, 24);
		droidItr.sprite.setTintColor(0, 0, 0);
		droidItr.sprite.render (droidScreenPosition.x, droidScreenPosition.y, 1.0);
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Animate the droid sprites
void gam_animateDroids (std::string levelName)
//-------------------------------------------------------------------------------------------------------------
{
	for (auto &droidItr : g_shipDeckItr->second.droid)
	{
		droidItr.sprite.animate ();
	}
}
