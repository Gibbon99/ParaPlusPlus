#include <game/database.h>
#include <game/player.h>
#include <game/shipDecks.h>
#include <game/audio.h>
#include <system/util.h>
#include <game/particles.h>
#include <game/lightMaps.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include "classes/paraBullet.h"

//#define DEBUG_BULLET 1

#define MAX_NUM_BULLETS     32

float                   bulletDensity;
float                   bulletFriction;
float                   bulletAnimationSpeed;
float                   numDisrupterFrames;
float                   disrupterFadeAmount;
float                   bulletMoveSpeed;
std::vector<paraBullet> bullets;

//-----------------------------------------------------------------------------
//
// Get the angle in degrees of rotation for the bullet
auto getAngle (b2Vec2 sourcePos, b2Vec2 destPos) -> float
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
void gam_doDisrupterDamage (int sourceDroid)
//---------------------------------------------------------------------------------------------------------------
{
	for (auto &droidItr: g_shipDeckItr->second.droid)
	{
		if ((droidItr.visibleToPlayer) && (droidItr.getCurrentMode () == DROID_MODE_NORMAL) && (!dataBaseEntry[droidItr.getDroidType ()].disrupterImmune))
		{
			gam_damageToDroid (droidItr.userData->dataValue, PHYSIC_DAMAGE_BULLET, sourceDroid);
		}
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Create a new bullet - called from game event
paraBullet gam_createBullet (int bulletSourceIndex, Uint32 bulletID)
//---------------------------------------------------------------------------------------------------------------
{
	paraBullet tempBullet;
	b2Vec2     tempPosition;
	b2Vec2     tempVelocity;
	b2Vec2     newVelocity;
	b2Vec2     newWorldPosInMeters;
	int        bulletType;

#ifdef DEBUG_BULLET
	std::cout << "Creating a bullet with ID of : " << m_bulletID << std::endl;
#endif

	if (-1 == bulletSourceIndex)        // Bullet from player
	{
		bulletType     = dataBaseEntry[playerDroid.getDroidType ()].bulletType;
		if (!dataBaseEntry[playerDroid.getDroidType ()].canShoot)
			bulletType = BULLET_TYPE_NORMAL;

		newVelocity         = playerDroid.getVelocity ();
		newWorldPosInMeters = sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels ());
		//
		// Set its direction
		tempVelocity        = newVelocity;
		tempVelocity.Normalize ();
		tempVelocity *= 50.0f;
		tempBullet.velocity = tempVelocity;

		tempBullet.worldDestInMeters = tempVelocity + sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels ());
	}
	else        // Bullet from Droid
	{
		bulletType = dataBaseEntry[g_shipDeckItr->second.droid[bulletSourceIndex].getDroidType ()].bulletType;

		if (g_shipDeckItr->second.droid[bulletSourceIndex].ai2.getTargetDroid () == TARGET_PLAYER)
		{
			// Aim at player position
			newVelocity = playerDroid.getWorldPosInPixels () - g_shipDeckItr->second.droid[bulletSourceIndex].getWorldPosInPixels ();
			tempBullet.worldDestInMeters = sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels ());
		}
		else
		{
			// Aim at other droid
			newVelocity = g_shipDeckItr->second.droid[g_shipDeckItr->second.droid[bulletSourceIndex].ai2.getTargetDroid ()].getWorldPosInPixels () - g_shipDeckItr->second.droid[bulletSourceIndex].getWorldPosInPixels ();
			tempBullet.worldDestInMeters = sys_convertPixelsToMeters (g_shipDeckItr->second.droid[g_shipDeckItr->second.droid[bulletSourceIndex].ai2.getTargetDroid ()].getWorldPosInPixels ());
		}

		newWorldPosInMeters = sys_convertPixelsToMeters (g_shipDeckItr->second.droid[bulletSourceIndex].getWorldPosInPixels ());
		//
		// Set its direction
		tempVelocity        = newVelocity;
		tempVelocity.Normalize ();
		tempVelocity *= bulletMoveSpeed;
		tempBullet.velocity = tempVelocity;
	}

	if (bulletType != BULLET_TYPE_DISRUPTER)
	{
		//
		// Bullet starting position is outside sprite size to avoid collision
		tempPosition = newVelocity;
		tempPosition.Normalize ();
		tempPosition *= (SPRITE_SIZE / 2.0) / pixelsPerMeter;
		tempBullet.worldPosInMeters = newWorldPosInMeters + tempPosition;

		tempBullet.angle = getAngle (tempBullet.worldPosInMeters, tempBullet.worldDestInMeters);

		tempBullet.bodyDef.type = b2_dynamicBody;
		tempBullet.bodyDef.position.Set (tempBullet.worldPosInMeters.x, tempBullet.worldPosInMeters.y);
		tempBullet.bodyDef.angle  = 0;
		tempBullet.bodyDef.bullet = true;
		tempBullet.body           = sys_getPhysicsWorld ()->CreateBody (&tempBullet.bodyDef);
		if (tempBullet.body == nullptr)
			sys_shutdownWithError ("Unable to create new physics body for bullet.");

		tempBullet.userData               = reinterpret_cast<_userData *>(sys_malloc (sizeof (_userData), sys_getString ("%i", bulletID))); //  new _userData;
		if (bulletSourceIndex == -1)
			tempBullet.userData->userType = PHYSIC_TYPE_BULLET_PLAYER;
		else
			tempBullet.userData->userType = PHYSIC_TYPE_BULLET_ENEMY;

		tempBullet.userData->ID                   = bulletID;
		tempBullet.userData->dataValue            = bulletSourceIndex;
		tempBullet.userData->wallIndexValue       = -1;
		tempBullet.userData->ignoreCollisionDroid = false;
		tempBullet.body->SetUserData (tempBullet.userData);

#ifdef DEBUG_BULLET
		std::cout << "Bullet ID set to : " << tempBullet.userData->ID << std::endl;
#endif
	}

	switch (bulletType)
	{
		case BULLET_TYPE_NORMAL:
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "laser");
			tempBullet.sprite.create ("bullet_001", 8, bulletAnimationSpeed);
			tempBullet.shape.SetAsBox (6.0f / pixelsPerMeter, 6.0f / pixelsPerMeter);
			tempBullet.fixtureDef.shape = &tempBullet.shape;
			break;

		case BULLET_TYPE_SINGLE:
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "laser");
			tempBullet.sprite.create ("bullet_476", 8, bulletAnimationSpeed);
			tempBullet.shape.SetAsBox (9.0f / pixelsPerMeter, 3.0f / pixelsPerMeter);
			tempBullet.fixtureDef.shape = &tempBullet.shape;
			break;

		case BULLET_TYPE_DOUBLE:
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "laser");
			tempBullet.sprite.create ("bullet_821", 8, bulletAnimationSpeed);
			tempBullet.shape.SetAsBox (12.0f / pixelsPerMeter, 6.0f / pixelsPerMeter);
			tempBullet.fixtureDef.shape = &tempBullet.shape;
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
			tempBullet.fixtureDef.filter.categoryBits = PHYSIC_TYPE_BULLET_PLAYER;
			tempBullet.fixtureDef.filter.maskBits     = PHYSIC_TYPE_WALL | PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_DOOR_CLOSED;
		}
		else
		{
			tempBullet.fixtureDef.filter.categoryBits = PHYSIC_TYPE_BULLET_ENEMY;
			tempBullet.fixtureDef.filter.maskBits     = PHYSIC_TYPE_WALL | PHYSIC_TYPE_ENEMY | PHYSIC_TYPE_PLAYER | PHYSIC_TYPE_BULLET_PLAYER | PHYSIC_TYPE_BULLET_ENEMY | PHYSIC_TYPE_DOOR_CLOSED;
		}
		tempBullet.fixtureDef.density     = bulletDensity;
		tempBullet.fixtureDef.friction    = bulletFriction;
		tempBullet.fixtureDef.restitution = 0.01f;
		tempBullet.body->CreateFixture (&tempBullet.fixtureDef);
	}

	tempBullet.type  = bulletType;
	tempBullet.inUse = true;
	tempBullet.ID    = bulletID;

	return tempBullet;
}

//---------------------------------------------------------------------------------------------------------------
//
// Preallocate the bullet array - clear out on level change
void gam_initBulletArray ()
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
// Return the index of a bullet using its ID - return -1 if the bullet could not be found
int gam_getArrayIndex (Uint32 bulletID)
//---------------------------------------------------------------------------------------------------------------
{
	int indexCounter = 0;

	for (const auto &bulletItr: bullets)
	{
		if (bulletItr.ID == bulletID)
			return indexCounter;

		indexCounter++;
	}

	return -1;

	sys_shutdownWithError (sys_getString ("Unable to locate bullet index by ID [ %i ]", bulletID));
}

//---------------------------------------------------------------------------------------------------------------
//
// Add a newly created bullet to the array
void gam_addBullet (int bulletSourceIndex)
//---------------------------------------------------------------------------------------------------------------
{
	int    indexCounter {};
	Uint32 bulletID {};

	if (bulletSourceIndex == -1)
		playerDroid.setWeaponCanFire (false);
	else
		g_shipDeckItr->second.droid[bulletSourceIndex].setWeaponCanFire (false);

	bulletID = SDL_GetTicks ();

	for (auto &bulletItr: bullets)
	{
		if (!bulletItr.inUse)
		{
			bulletItr = gam_createBullet (bulletSourceIndex, bulletID);
			if (bulletSourceIndex < 0)  // Player bullet
			{
				gam_addEmitter (sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels ()), PARTICLE_TYPE_TRAIL, bulletID);
				gam_addNewLightmap (sys_convertPixelsToMeters (playerDroid.getWorldPosInPixels ()), LIGHTMAP_TYPE_BULLET, bulletID);
			}
			else
			{
				gam_addEmitter (sys_convertPixelsToMeters (g_shipDeckItr->second.droid[bulletSourceIndex].getWorldPosInPixels ()), PARTICLE_TYPE_TRAIL, bulletID);
				gam_addNewLightmap (sys_convertPixelsToMeters (g_shipDeckItr->second.droid[bulletSourceIndex].getWorldPosInPixels ()), LIGHTMAP_TYPE_BULLET, bulletID);
			}
#ifdef DEBUG_BULLET
			std::cout << "Bullet with ID : " << m_bulletID << " added to array position : " << indexCounter << std::endl;
#endif

			return;
		}
		indexCounter++;
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Remove a bullet - called from Game Event loop - outside of Physics world step
void gam_removeBullet (Uint32 bulletID)
//---------------------------------------------------------------------------------------------------------------
{
	int bulletIndex = -1;

	bulletIndex = gam_getArrayIndex (bulletID);
#ifdef DEBUG_BULLET
	std::cout << "Remove a bullet : " << m_bulletID << " with position in array : " << bulletIndex << std::endl;
#endif

	try
	{
		bullets.at (bulletIndex).inUse = false;

		if (bullets.at (bulletIndex).body != nullptr)
		{
			bullets.at (bulletIndex).body->GetWorld ()->DestroyBody (bullets.at (bulletIndex).body);
			bullets.at (bulletIndex).body = nullptr;
		}

		if (bullets.at (bulletIndex).userData != nullptr)
		{
			sys_freeMemory (sys_getString ("%i", bulletID));
			bullets.at (bulletIndex).userData = nullptr;
		}

		bullets.at (bulletIndex).velocity = {0, 0};
		bullets.at (bulletIndex).inUse    = false;

		gam_removeEmitter (bulletID);
		gam_removeLightmap (bulletID);
	}

	catch (const std::out_of_range &outOfRange)
	{
//		sys_shutdownWithError (sys_getString ("Attempting to remove bullet with index [ %i ]. Out of range.", bulletIndex));
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Process physics and animation for any active bullets
void gam_processBullets ()
//---------------------------------------------------------------------------------------------------------------
{
	for (auto &bulletItr: bullets)
	{
		if (bulletItr.inUse)
		{
			if (bulletItr.type != BULLET_TYPE_DISRUPTER)
			{
				bulletItr.velocity.Normalize ();
				bulletItr.velocity.operator*= (bulletMoveSpeed);
				bulletItr.body->ApplyLinearImpulseToCenter (bulletItr.velocity, true);
				bulletItr.sprite.animate ();
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
void gam_renderBullets ()
//---------------------------------------------------------------------------------------------------------------
{
	b2Vec2 tempPosition;

	for (auto &bulletItr: bullets)
	{
		if (bulletItr.inUse)
		{
			if (bulletItr.type != BULLET_TYPE_DISRUPTER)
			{
				bulletItr.worldPosInMeters = bulletItr.body->GetPosition ();
				tempPosition = sys_convertMetersToPixels (bulletItr.worldPosInMeters);
				tempPosition = sys_worldToScreen (tempPosition, SPRITE_SIZE);
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
void gam_debugBullets ()
//---------------------------------------------------------------------------------------------------------------
{
	int activeCounter = 0;

	for (const auto &bulletItr: bullets)
	{
		if (bulletItr.inUse)
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("Index [ %i ] inUse [ %i ] Velocity [ %3.3f %3.3f ]", bulletItr.userData->dataValue, bulletItr.inUse, bulletItr.velocity.x, bulletItr.velocity.y));
			activeCounter++;
		}
	}

	con_addEvent (EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString ("Bullet array size [ %i ] - number active [ %i ]", bullets.size (), activeCounter));
}
