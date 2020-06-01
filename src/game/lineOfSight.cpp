#include "game/player.h"
#include "classes/paraLOS.h"
#include "system/util.h"
#include "game/lineOfSight.h"

RayCastAnyCallback LOSCallback;
int                visibleFadeValue;

//----------------------------------------------------------------------------------------------------------------------
//
// Check each droid to see if the player can see them or not - set visibleToPlayer flag to reflect
void gam_checkLOS (droidClass &droidItr)
//----------------------------------------------------------------------------------------------------------------------
{
	sys_getPhysicsWorld ()->RayCast (&LOSCallback, sys_convertToMeters (droidItr.worldPosInPixels), sys_convertToMeters (playerDroid.worldPosInPixels));

	if (LOSCallback.m_hit)
	{
		droidItr.visibleState = VISIBLE_STATE_GO_NOT_VISIBLE;
	}
	else
	{
		droidItr.visibleState = VISIBLE_STATE_GO_VISIBLE;
	}

	if (droidItr.visibleState == VISIBLE_STATE_GO_VISIBLE)
	{
		if (droidItr.visibleValue < 255)
		{
			droidItr.visibleValue += visibleFadeValue;
			if (droidItr.visibleValue > 30)
			{
				droidItr.visibleValue    = 255;
				droidItr.visibleState    = VISIBLE_STATE_IS_VISIBLE;
				droidItr.visibleToPlayer = true;
			}
		}
	}

	if (droidItr.visibleState == VISIBLE_STATE_GO_NOT_VISIBLE)
	{
		droidItr.visibleValue -= visibleFadeValue;
		if (droidItr.visibleValue < 0)
		{
			droidItr.visibleValue    = 0;
			droidItr.visibleToPlayer = false;
		}
	}
}