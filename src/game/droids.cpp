#include <game/database.h>
#include <game/shipDecks.h>
#include <system/util.h>
#include "game/droids.h"

droidClass  testDroid;

//-------------------------------------------------------------------------------------------------------------
//
// Test the aStar with a single droid
void createTestDroid()
//-------------------------------------------------------------------------------------------------------------
{
	testDroid.droidType = 0;
	testDroid.droidName = "001";
	testDroid.sprite.create (testDroid.droidName, 9, 0.3);
	testDroid.acceleration = dataBaseEntry[0].accelerate;
	testDroid.maxSpeed     = dataBaseEntry[0].maxSpeed;

	testDroid.worldPosInPixels.x = shipdecks.at(gam_getCurrentDeckName()).wayPoints[0].x;
	testDroid.worldPosInPixels.y = shipdecks.at(gam_getCurrentDeckName()).wayPoints[0].y;
}

//-------------------------------------------------------------------------------------------------------------
//
// Render the test droid
void renderTestDroid()
//-------------------------------------------------------------------------------------------------------------
{
	paraVec2d testDroidScreenPosition;

	testDroidScreenPosition.x = testDroid.worldPosInPixels.x;
	testDroidScreenPosition.y = testDroid.worldPosInPixels.y;

	testDroidScreenPosition = sys_worldToScreen(testDroidScreenPosition, 24);

	testDroid.sprite.render(testDroidScreenPosition.x, testDroidScreenPosition.y, 1.0);
}