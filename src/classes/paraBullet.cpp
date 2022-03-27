#include <game/database.h>
#include <game/player.h>
#include <game/shipDecks.h>
#include <game/audio.h>
#include <system/util.h>
#include <game/particles.h>
#include <game/lightMaps.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include "classes/paraBullet.h"

#define MAX_NUM_BULLETS     32

float                   bulletDensity {};
float                   bulletFriction {};
float                   bulletAnimationSpeed {};
float                   numDisrupterFrames {};
float                   disrupterFadeAmount {};
float                   bulletMoveSpeed {};
float                   bullet1Mass {0.009f};
float                   bullet2Mass {0.007f};
float                   bullet3Mass {0.005f};
std::vector<paraBullet> bullets;

//-----------------------------------------------------------------------------
//
// Get the angle in degrees of rotation for the bullet
auto getAngle(cpVect sourcePos, cpVect destPos) -> float
//-----------------------------------------------------------------------------
{
	auto localAngle = static_cast<float>((atan2 (destPos.y - sourcePos.y, destPos.x - sourcePos.x) * (180 / MY_PI)));

	if (localAngle < 0)
		localAngle = 360 - (-localAngle);

	return localAngle;
}

//---------------------------------------------------------------------------------------------------------------
//
// Do damage from disrupter fire
void gam_doDisrupterDamage(int sourceDroid)
//---------------------------------------------------------------------------------------------------------------
{
	for (auto &droidItr: g_shipDeckItr->second.droid)
	{
		if ((droidItr.visibleToPlayer) && (droidItr.getCurrentMode () == DROID_MODE_NORMAL) && (!dataBaseEntry[droidItr.getDroidType ()].disrupterImmune))
		{
			gam_damageToDroid (droidItr.userData->dataValue, PHYSIC_DAMAGE_BULLET, sourceDroid);
		}
	}

	if (!dataBaseEntry[playerDroid.getDroidType ()].disrupterImmune)
		gam_damageToPlayer (PHYSIC_DAMAGE_BULLET, sourceDroid);
}

//---------------------------------------------------------------------------------------------------------------
//
// Create a new bullet - called from game event
paraBullet gam_createBullet(int bulletSourceIndex, Uint32 bulletID)
//---------------------------------------------------------------------------------------------------------------
{
	paraBullet tempBullet;
	cpVect     tempPosition;
	cpVect     tempVelocity;
	cpVect     newVelocity;
	cpVect     newWorldPosInPixels;
	int        bulletType;
	double     moment;

#ifdef DEBUG_BULLET
	logFile.write (sys_getString ("[ %s ] Creating a bullet with ID of [ %i ]", __func__, bulletID));
#endif

	if (-1 == bulletSourceIndex)        // Bullet from player
	{
#ifdef DEBUG_BULLET
		logFile.write (sys_getString ("[ %s ] Player fired bullet [ %i ]", __func__, bulletID));
#endif

		bulletType     = dataBaseEntry[playerDroid.getDroidType ()].bulletType;
		if (!dataBaseEntry[playerDroid.getDroidType ()].canShoot)
			bulletType = BULLET_TYPE_NORMAL;

		newVelocity         = playerDroid.getVelocity ();
		newWorldPosInPixels = playerDroid.getWorldPosInPixels ();
		//
		// Set its direction
		tempVelocity        = newVelocity;
		tempVelocity        = cpvnormalize (tempVelocity);
		tempVelocity        = cpvmult (tempVelocity, bulletMoveSpeed);
		tempBullet.velocity = tempVelocity;

		tempBullet.worldDestInPixels = cpvadd (newWorldPosInPixels, tempVelocity);
	}
	else        // Bullet from Droid
	{
#ifdef DEBUG_BULLET
		logFile.write (sys_getString ("[ %s ] Enemy fired bullet [ %i ]", __func__, bulletID));
#endif
		bulletType = dataBaseEntry[g_shipDeckItr->second.droid[bulletSourceIndex].getDroidType ()].bulletType;

		if (g_shipDeckItr->second.droid[bulletSourceIndex].ai2.getTargetDroid () == TARGET_PLAYER)
		{
			// Aim at player position
			newVelocity = cpvsub (playerDroid.getWorldPosInPixels (), g_shipDeckItr->second.droid[bulletSourceIndex].getWorldPosInPixels ());
			tempBullet.worldDestInPixels = playerDroid.getWorldPosInPixels ();
		}
		else
		{
			// Aim at other droid
			newVelocity = cpvsub (g_shipDeckItr->second.droid[g_shipDeckItr->second.droid[bulletSourceIndex].ai2.getTargetDroid ()].getWorldPosInPixels (), g_shipDeckItr->second.droid[bulletSourceIndex].getWorldPosInPixels ());
			tempBullet.worldDestInPixels = g_shipDeckItr->second.droid[g_shipDeckItr->second.droid[bulletSourceIndex].ai2.getTargetDroid ()].getWorldPosInPixels ();
		}

		newWorldPosInPixels = g_shipDeckItr->second.droid[bulletSourceIndex].getWorldPosInPixels ();
		//
		// Set its direction
		tempVelocity        = cpvnormalize (newVelocity);
		tempVelocity        = cpvmult (tempVelocity, bulletMoveSpeed);
		tempBullet.velocity = tempVelocity;
	}

	if (bulletType != BULLET_TYPE_DISRUPTER)
	{
		//
		// Bullet starting position is outside sprite size to avoid collision
		tempPosition = tempBullet.velocity;
		tempPosition = cpvnormalize (tempPosition);
		tempPosition = cpvmult (tempPosition, (SPRITE_SIZE / 2.0));
		tempBullet.worldPosInPixels = cpvadd (newWorldPosInPixels, tempPosition);

		tempBullet.angle = getAngle (tempBullet.worldPosInPixels, tempBullet.worldDestInPixels);

		tempBullet.userData               = std::make_shared<userData_> ();
		if (bulletSourceIndex == -1)
			tempBullet.userData->userType = PHYSIC_TYPE_BULLET_PLAYER;
		else
			tempBullet.userData->userType = PHYSIC_TYPE_BULLET_ENEMY;

		tempBullet.userData->bulletID             = bulletID;
		tempBullet.userData->dataValue            = bulletSourceIndex;
		tempBullet.userData->ignoreCollisionDroid = false;
	}

	switch (bulletType)
	{
		case BULLET_TYPE_NORMAL:
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "laser");
			tempBullet.sprite.create ("bullet_001", 8, bulletAnimationSpeed);

			moment = cpMomentForBox (bullet1Mass, 6.0, 6.0);
			tempBullet.body = cpSpaceAddBody (sys_returnPhysicsWorld (), cpBodyNew (0.5, moment));
			cpBodySetMass (tempBullet.body, bullet1Mass);
			tempBullet.shape = cpBoxShapeNew (tempBullet.body, 6.0, 6.0, 1.0);
			break;

		case BULLET_TYPE_SINGLE:
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "laser");
			tempBullet.sprite.create ("bullet_476", 8, bulletAnimationSpeed);

			moment = cpMomentForBox (bullet2Mass, 9.0, 9.0);
			tempBullet.body = cpSpaceAddBody (sys_returnPhysicsWorld (), cpBodyNew (0.5, moment));
			cpBodySetMass (tempBullet.body, bullet2Mass);
			tempBullet.shape = cpBoxShapeNew (tempBullet.body, 9.0, 9.0, 1.0);
			break;

		case BULLET_TYPE_DOUBLE:
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "laser");
			tempBullet.sprite.create ("bullet_821", 8, bulletAnimationSpeed);

			moment = cpMomentForBox (bullet3Mass, 12.0, 6.0);
			tempBullet.body = cpSpaceAddBody (sys_returnPhysicsWorld (), cpBodyNew (0.5, moment));
			cpBodySetMass (tempBullet.body, bullet3Mass);
			tempBullet.shape = cpBoxShapeNew (tempBullet.body, 12.0, 6.0, 1.0);
			break;

		case BULLET_TYPE_DISRUPTER:
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "disruptor");
			tempBullet.disrupterFadeAmount = disrupterFadeAmount;
			tempBullet.disrupterFade       = 250;
			gam_doDisrupterDamage (bulletSourceIndex);
			break;

		default:
			sys_shutdownWithError ("Unknown bullet type passed to create bullet.");
			break;
	}

	if (bulletType != BULLET_TYPE_DISRUPTER)
	{
		if (bulletSourceIndex == -1)
		{
			cpShapeSetCollisionType (tempBullet.shape, PHYSIC_TYPE_BULLET_PLAYER);
			cpShapeSetFilter (tempBullet.shape, FILTER_CAT_PLAYER_BULLET);
		}
		else
		{
			cpShapeSetCollisionType (tempBullet.shape, PHYSIC_TYPE_BULLET_ENEMY);
			cpShapeSetFilter (tempBullet.shape, FILTER_CAT_ENEMY_BULLET);
		}

		cpShapeSetUserData (tempBullet.shape, tempBullet.userData.get ());
		cpBodySetPosition (tempBullet.body, tempBullet.worldPosInPixels);
		cpSpaceAddShape (sys_returnPhysicsWorld (), tempBullet.shape);
	}

	tempBullet.type  = bulletType;
	tempBullet.inUse = true;
	tempBullet.ID    = bulletID;

	return tempBullet;
}

//---------------------------------------------------------------------------------------------------------------
//
// Preallocate the bullet array - clear out on level change
void gam_initBulletArray()
//---------------------------------------------------------------------------------------------------------------
{
	paraBullet tempBullet;

	if (bullets.size () > 0)
	{
		for (auto &bulletItr: bullets)
		{
			if (bulletItr.inUse)
			{
				gam_removeBullet (bulletItr.ID);
				gam_removeEmitter (bulletItr.ID);
				gam_removeLightmap (bulletItr.ID);
			}

			bulletItr.inUse = false;
			bulletItr.ID    = 0;
		}
	}

	for (auto i = 0; i != MAX_NUM_BULLETS; i++)
	{
		tempBullet.inUse = false;
		tempBullet.ID    = 0;
		bullets.push_back (tempBullet);
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Return the position of the bullet in pixels
cpVect gam_getBulletWorldPosition(Uint32 bulletID)
//---------------------------------------------------------------------------------------------------------------
{

	auto bulletIndex = gam_getArrayIndex (bulletID);
	if (-1 == bulletIndex)
		return cpVect{0, 0};

	return bullets[bulletIndex].worldPosInPixels;
}

//---------------------------------------------------------------------------------------------------------------
//
// Return the index of a bullet using its ID - return -1 if the bullet could not be found
int gam_getArrayIndex(Uint32 bulletID)
//---------------------------------------------------------------------------------------------------------------
{
	int indexCounter = 0;

	for (const auto &bulletItr: bullets)
	{
		if (bulletItr.ID == bulletID)
			return indexCounter;

		indexCounter++;
	}

	printf ("%s - Unable to find bulletID [ %i ].\n", __func__, bulletID);

	return -1;
}

//---------------------------------------------------------------------------------------------------------------
//
// Get a unique bullet ID
int gam_getBulletID()
//---------------------------------------------------------------------------------------------------------------
{
	static int newBulletID {100};

	return ++newBulletID;
}

//---------------------------------------------------------------------------------------------------------------
//
// Add a newly created bullet to the array
void gam_addBullet(int bulletSourceIndex)
//---------------------------------------------------------------------------------------------------------------
{
	int    indexCounter {};
	Uint32 bulletID {};

	if (bulletSourceIndex == -1)
		playerDroid.setWeaponCanFire (false);
	else
		g_shipDeckItr->second.droid[bulletSourceIndex].setWeaponCanFire (false);

	bulletID = gam_getBulletID ();

	for (auto &bulletItr: bullets)
	{
		if (!bulletItr.inUse)
		{
			bulletItr = gam_createBullet (bulletSourceIndex, bulletID);
			//
			// Only add particles and lightmaps for non disrupter bullets
			if (bulletItr.type != BULLET_TYPE_DISRUPTER)
			{
				if (bulletSourceIndex < 0)  // Player bullet
				{
					gam_addEmitter (playerDroid.getWorldPosInPixels (), PARTICLE_TYPE_TRAIL, bulletID);
					gam_addNewLightmap (playerDroid.getWorldPosInPixels (), LIGHTMAP_TYPE_BULLET, bulletID);
				}
				else
				{
					gam_addEmitter (g_shipDeckItr->second.droid[bulletSourceIndex].getWorldPosInPixels (), PARTICLE_TYPE_TRAIL, bulletID);
					gam_addNewLightmap (g_shipDeckItr->second.droid[bulletSourceIndex].getWorldPosInPixels (), LIGHTMAP_TYPE_BULLET, bulletID);
				}
			}
#ifdef DEBUG_BULLET
			logFile.write (sys_getString ("[ %s ] Bullet with ID [ %i ] added to bullet array at [ %i ]", __func__, bulletID, indexCounter));
#endif
			return;
		}
		indexCounter++;
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Remove a bullet - called from Game Event loop - outside of Physics world step
void gam_removeBullet(Uint32 bulletID)
//---------------------------------------------------------------------------------------------------------------
{
	int bulletIndex = -1;

	bulletIndex = gam_getArrayIndex (bulletID);
#ifdef DEBUG_BULLET
	logFile.write (sys_getString ("[ %s ] Remove a bullet [ %i ] with position [ %i ] in array.", __func__, bulletID, bulletIndex));
#endif

	try
	{
		bullets.at (bulletIndex).inUse    = false;
		bullets.at (bulletIndex).velocity = {0, 0};

		if (bullets.at (bulletIndex).type == BULLET_TYPE_DISRUPTER)
			return;

		if (cpSpaceContainsBody (sys_returnPhysicsWorld (), bullets.at (bulletIndex).body))
			cpSpaceRemoveBody (sys_returnPhysicsWorld (), bullets.at (bulletIndex).body);

		if (cpSpaceContainsShape (sys_returnPhysicsWorld (), bullets.at (bulletIndex).shape))
			cpSpaceRemoveShape (sys_returnPhysicsWorld (), bullets.at (bulletIndex).shape);

		gam_removeEmitter (bulletID);
		gam_removeLightmap (bulletID);

	}

	catch (const std::out_of_range &outOfRange)
	{
		sys_shutdownWithError (sys_getString ("Attempting to remove bullet with index [ %i ]. Out of range.", bulletIndex));
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Process physics and animation for any active bullets
void gam_processBullets()
//---------------------------------------------------------------------------------------------------------------
{
	for (auto &bulletItr: bullets)
	{
		if (bulletItr.inUse)
		{
			if (bulletItr.type != BULLET_TYPE_DISRUPTER)
			{
				if (cpSpaceContainsBody (sys_returnPhysicsWorld (), bulletItr.body))
				{
					bulletItr.velocity = cpvnormalize (bulletItr.velocity);
					bulletItr.velocity = cpvmult (bulletItr.velocity, bulletMoveSpeed);
					cpBodyApplyForceAtLocalPoint (bulletItr.body, bulletItr.velocity, cpvzero);
					bulletItr.sprite.animate ();
					bulletItr.worldPosInPixels = cpBodyGetPosition (bulletItr.body);
				}
			}
			else
			{
				bulletItr.disrupterFade -= bulletItr.disrupterFadeAmount;
				if (bulletItr.disrupterFade < 0)
				{
					gam_removeBullet (bulletItr.ID);
				}
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Render any visible bullets to screen
void gam_renderBullets()
//---------------------------------------------------------------------------------------------------------------
{
	cpVect tempPosition;

	for (auto &bulletItr: bullets)
	{
		if (bulletItr.inUse)
		{
			if (bulletItr.type != BULLET_TYPE_DISRUPTER)
			{
				tempPosition = sys_worldToScreen (cpBodyGetPosition (bulletItr.body), SPRITE_SIZE);
				bulletItr.sprite.render (tempPosition.x, tempPosition.y, 1.0f, bulletItr.angle);
			}
			else
			{
				boxRGBA (renderer.renderer, 0, 0, hiresVirtualWidth, hiresVirtualHeight, 255, 255, 255, static_cast<Uint8>(bulletItr.disrupterFade));
			}
		}
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Debug bullets
void gam_debugBullets()
//---------------------------------------------------------------------------------------------------------------
{
	int activeCounter = 0;

	for (const auto &bulletItr: bullets)
	{
		if (bulletItr.inUse)
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("Source [ %i ] inUse [ %i ] Velocity [ %3.3f %3.3f ]", bulletItr.userData->dataValue, bulletItr.inUse, bulletItr.velocity.x, bulletItr.velocity.y));
			activeCounter++;
		}
	}

	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("Bullet array size [ %i ] - number active [ %i ]", bullets.size (), activeCounter));
}
