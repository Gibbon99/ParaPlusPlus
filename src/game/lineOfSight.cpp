#include <game/shipDecks.h>
#include <system/util.h>
#include <SDL2_gfxPrimitives.h>
#include "game/player.h"
#include "game/lineOfSight.h"

int visibleFadeValue;

//----------------------------------------------------------------------------------------------------------------------
//
// Check each droid to see if the player can see them or not - set visibleToPlayer flag to reflect
void gam_checkLOS(paraDroid &droidItr)
//----------------------------------------------------------------------------------------------------------------------
{
#ifdef SHOW_ALL_DROIDS
	droidItr.visibleState = VISIBLE_STATE_GO_VISIBLE;
	return;
#endif
	cpShapeFilter      shapeFilter {};
	cpSegmentQueryInfo segmentQueryResult {};

	shapeFilter.group      = CP_NO_GROUP;
	shapeFilter.mask       = PHYSIC_TYPE_WALL | PHYSIC_TYPE_DOOR_CLOSED;      // Check for walls
	shapeFilter.categories = PHYSIC_TYPE_PLAYER;

	//
	// Only run if the droid is on the visible screen
	if (sys_visibleOnScreen (droidItr.getWorldPosInPixels (), SPRITE_SIZE / 2))
	{
		auto raycastResult = cpSpaceSegmentQueryFirst (sys_returnPhysicsWorld (), playerDroid.getWorldPosInPixels (), droidItr.getWorldPosInPixels (), 2.0f, shapeFilter, &segmentQueryResult);

		if (nullptr != raycastResult)       // No objects in the way
		{
			droidItr.visibleState = VISIBLE_STATE_GO_NOT_VISIBLE;
		}
	}
	else
	{
		droidItr.visibleState = VISIBLE_STATE_GO_NOT_VISIBLE;
	}
}