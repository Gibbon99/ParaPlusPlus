#include <game/database.h>
#include <game/player.h>
#include <game/shipDecks.h>
#include <game/audio.h>
#include <system/util.h>
#include "game/bullet.h"

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
// Create a new bullet - called from game event
paraBullet createBullet (int bulletSourceIndex, int arrayIndex)
//---------------------------------------------------------------------------------------------------------------
{
	paraBullet tempBullet;
	b2Vec2     tempPosition;
	b2Vec2     tempVelocity;
	b2Vec2     newVelocity;
	b2Vec2     newWorldPosInMeters;
	int        bulletType;

	if (-1 == bulletSourceIndex)        // Bullet from player
	{
		bulletType          = dataBaseEntry[playerDroid.droidType].bulletType;
		newVelocity         = playerDroid.velocity;
		newWorldPosInMeters = sys_convertToMeters (playerDroid.worldPosInPixels);
		//
		// Set its direction
		tempBullet.velocity = newVelocity;
		tempBullet.velocity.Normalize ();
		tempVelocity = tempBullet.velocity;
		tempVelocity *= 50.0f;
		tempBullet.worldDestInMeters = tempVelocity + sys_convertToMeters (playerDroid.worldPosInPixels);
	}
	else
	{
		bulletType = dataBaseEntry[shipdecks.at(gam_getCurrentDeckName()).droid[bulletSourceIndex].droidType].bulletType;
		
		if (g_shipDeckItr->second.droid[bulletSourceIndex].ai.getTargetDroid() == -1)
		{
			// Aim at player position
			newVelocity = shipdecks.at(gam_getCurrentDeckName()).droid[bulletSourceIndex].worldPosInPixels - playerDroid.worldPosInPixels;
		}
		else
		{
			// Aim at other droid
			newVelocity = shipdecks.at(gam_getCurrentDeckName()).droid[bulletSourceIndex].worldPosInPixels - 
				g_shipDeckItr->second.droid[g_shipDeckItr->second.droid[bulletSourceIndex].ai.getTargetDroid()].worldPosInPixels;
		}
		
		newWorldPosInMeters = sys_convertToMeters (shipdecks.at (gam_getCurrentDeckName ()).droid[bulletSourceIndex].worldPosInPixels);
		//
		// Set its direction
		tempBullet.velocity = newVelocity;
		tempBullet.velocity.Normalize ();
		tempBullet.velocity.operator *= (bulletMoveSpeed);
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
		tempBullet.bodyDef.angle = 0;
		tempBullet.bodyDef.bullet = true;
		tempBullet.body           = sys_getPhysicsWorld ()->CreateBody (&tempBullet.bodyDef);
		if (tempBullet.body == nullptr)
			sys_shutdownWithError ("Unable to create new physics body for bullet.");

		tempBullet.userData               = new _userData;
		if (bulletSourceIndex == -1)
			tempBullet.userData->userType = PHYSIC_TYPE_BULLET_PLAYER;
		else
			tempBullet.userData->userType = PHYSIC_TYPE_BULLET_ENEMY;

		tempBullet.userData->dataValue = arrayIndex;
		tempBullet.userData->wallIndexValue = -1;
		tempBullet.userData->ignoreCollision = false;
		tempBullet.body->SetUserData (tempBullet.userData);
	}

	switch (bulletType)
	{
		case BULLET_TYPE_NORMAL:
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "laser.wav");
			tempBullet.sprite.create ("bullet_001", 8, bulletAnimationSpeed);
			tempBullet.shape.SetAsBox(6.0f / pixelsPerMeter, 6.0f / pixelsPerMeter);
			tempBullet.fixtureDef.shape = &tempBullet.shape;
			break;

		case BULLET_TYPE_SINGLE:
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "laser.wav");
			tempBullet.sprite.create ("bullet_476", 8, bulletAnimationSpeed);
			tempBullet.shape.SetAsBox(9.0f / pixelsPerMeter, 3.0f / pixelsPerMeter);
			tempBullet.fixtureDef.shape = &tempBullet.shape;
			break;

		case BULLET_TYPE_DOUBLE:
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "laser.wav");
			tempBullet.sprite.create ("bullet_821", 8, bulletAnimationSpeed);
			tempBullet.shape.SetAsBox(12.0f / pixelsPerMeter, 6.0f / pixelsPerMeter);
			tempBullet.fixtureDef.shape = &tempBullet.shape;
			break;

		case BULLET_TYPE_DISRUPTER:
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "disrupter.wav");
			tempBullet.disrupterFadeAmount = disrupterFadeAmount / numDisrupterFrames;
			tempBullet.disrupterFade       = disrupterFadeAmount;
			// TODO - Disrupter damage
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

	return tempBullet;
}

//---------------------------------------------------------------------------------------------------------------
//
// Add a newly created bullet to the array
void gam_addBullet (int bulletSourceIndex)
//---------------------------------------------------------------------------------------------------------------
{
	if (bullets.size () == 0)
	{
		bullets.push_back (createBullet (bulletSourceIndex, 0));
		return;
	}

	for (auto i = 0; i != static_cast<int>(bullets.size ()); i++)
	{
		if (!bullets[i].inUse)
		{
			auto tempBullet = createBullet (bulletSourceIndex, i);
			if (tempBullet.type != -1)
				bullets[i] = tempBullet;
			return;
		}
	}

	bullets.push_back (createBullet (bulletSourceIndex, static_cast<int>(bullets.size ())));
}

//---------------------------------------------------------------------------------------------------------------
//
// Remove a bullet - called from Game Event loop - outside of Physics world step
void gam_removeBullet (int bulletIndex)
//---------------------------------------------------------------------------------------------------------------
{

//	std::cout << "Remove bullet " << bulletIndex << std::endl;

	try
	{
		bullets.at(bulletIndex).inUse = false;
		
		if (bullets.at (bulletIndex).body != nullptr)
		{
			bullets.at(bulletIndex).body->GetWorld()->DestroyBody(bullets.at(bulletIndex).body);
//			sys_getPhysicsWorld ()->DestroyBody (bullets.at (bulletIndex).body);
			bullets.at (bulletIndex).body = nullptr;
		}

		// TODO remove particle emitter

		if (bullets.at (bulletIndex).userData != nullptr)
		{
			free (bullets.at (bulletIndex).userData);
			bullets.at (bulletIndex).userData = nullptr;
		}

		bullets.at(bulletIndex).velocity = { 0,0 };
	}
	catch (const std::out_of_range &outOfRange)
	{
		sys_shutdownWithError (sys_getString ("Attempting to remove bullet with index [ %i ]. Out of range.", bulletIndex));
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Remove all the bullets
void gam_resetBullets ()
//---------------------------------------------------------------------------------------------------------------
{
	for (auto i = 0; i != static_cast<int>(bullets.size ()); i++)
	{
		gam_removeBullet (i);
	}
}

//---------------------------------------------------------------------------------------------------------------
//
// Process physics and animation for any active bullets
void gam_processBullets ()
//---------------------------------------------------------------------------------------------------------------
{
	for (auto &bulletItr : bullets)
	{
		if (bulletItr.inUse)
		{
			if (bulletItr.type != BULLET_TYPE_DISRUPTER)
			{
				bulletItr.velocity.Normalize ();
				bulletItr.velocity.operator*= (bulletMoveSpeed);
				bulletItr.body->ApplyLinearImpulseToCenter (bulletItr.velocity, true);
			}
			bulletItr.sprite.animate ();
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

	for (auto bulletItr : bullets)
	{
		if (bulletItr.type != BULLET_TYPE_DISRUPTER)
		{
			if (bulletItr.inUse)
			{
				tempPosition = bulletItr.body->GetPosition ();
				tempPosition = sys_convertToPixels (tempPosition);
				tempPosition = sys_worldToScreen (tempPosition, SPRITE_SIZE);
				bulletItr.sprite.render (tempPosition.x, tempPosition.y, 1.0f, bulletItr.angle);
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

	for (auto bulletItr : bullets)
	{
		if (bulletItr.inUse)
		{
			con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString("Index [ %i ] inUse [ %i ] Velocity [ %3.3f %3.3f ]", bulletItr.userData->dataValue, bulletItr.inUse,
				bulletItr.velocity.x, bulletItr.velocity.y));
			activeCounter++;
		}
	}

	con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString("Bullet array size [ %i ] - number active [ %i ]", bullets.size(), activeCounter));
}