#include "game/player.h"
#include "classes/paraLOS.h"
#include "game/lineOfSight.h"

// #define SHOW_ALL_DROIDS 1

int visibleFadeValue;

//----------------------------------------------------------------------------------------------------------------------
//
// Function callback when checking worldSpace for shapes
void cpSpaceSegmentQueryFirstCallback(cpShape *shape, [[maybe_unused]]cpFloat t, [[maybe_unused]]cpVect n, [[maybe_unused]]void *data)
//----------------------------------------------------------------------------------------------------------------------
{
	cpDataPointer shapePointer {};

	if (nullptr != shape)
	{
		shapePointer = cpShapeGetUserData (shape);
		if (nullptr != shapePointer)
		{
			auto *userData = reinterpret_cast<_userData *>(shapePointer);
			switch (userData->userType)
			{
				case PHYSIC_TYPE_ENEMY:
					printf ("Query found PHYSIC_TYPE_ENEMY\n");
					break;

				case PHYSIC_TYPE_WALL:
					printf ("Query found PHYSIC_TYPE_WALL\n");
					break;

				case PHYSIC_TYPE_DOOR_CLOSED:
					printf ("Query found PHYSIC_TYPE_DOOR_CLOSED\n");
					break;

				case PHYSIC_TYPE_DOOR_OPEN:
					printf ("Query found PHYSIC_TYPE_DOOR_OPEN\n");
					break;

				case PHYSIC_TYPE_LIFT:
					printf ("Query found PHYSIC_TYPE_LIFT\n");
					break;

				default:
					printf ("Query found [ %lu ]\n", userData->userType);
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Check each droid to see if the player can see them or not - set visibleToPlayer flag to reflect
void gam_checkLOS(paraDroid &droidItr)
//----------------------------------------------------------------------------------------------------------------------
{

	cpSegmentQueryInfo segmentQueryResult {};
	cpShapeFilter      shapeFilter {};
	cpDataPointer      shapePointer {};
	cpBool             isVisible {};

	shapeFilter.group      = CP_NO_GROUP;
	shapeFilter.mask       = PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_WALL | PHYSIC_TYPE_DOOR_CLOSED;      // Check for enemy's and walls - don't look through closed doors
	shapeFilter.categories = PHYSIC_TYPE_PLAYER;

//	cpSpaceSegmentQuery (sys_returnPhysicsWorld (), playerDroid.getWorldPosInPixels (), droidItr.getWorldPosInPixels (), 2.0f, FILTER_CAT_PLAYER, reinterpret_cast<cpSpaceSegmentQueryFunc>(cpSpaceSegmentQueryFirstCallback), nullptr);


	auto result = cpSpaceSegmentQueryFirst (sys_returnPhysicsWorld (), playerDroid.getWorldPosInPixels (), droidItr.getWorldPosInPixels (), 2.0f, shapeFilter, &segmentQueryResult);

	isVisible = false;

	if (nullptr != result)
	{
		shapePointer = cpShapeGetUserData (result);
		if (nullptr != shapePointer)
		{
			auto *userDataA = reinterpret_cast<_userData *>(shapePointer);

			if (userDataA->userType == PHYSIC_TYPE_ENEMY)
				isVisible = cpTrue;
			else
				isVisible = cpFalse;
		}
	}

	if (cpFalse == isVisible)
	{

#ifdef SHOW_ALL_DROIDS
		droidItr.visibleState = VISIBLE_STATE_GO_VISIBLE;
#else
		droidItr.visibleState = VISIBLE_STATE_GO_NOT_VISIBLE;
#endif

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