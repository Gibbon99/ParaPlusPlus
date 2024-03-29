#include "game/database.h"
#include "game/shipDecks.h"
#include "system/util.h"
#include "game/player.h"
#include "game/audio.h"
#include "game/particles.h"
#include "game/lineOfSight.h"
#include "game/texture.h"
#include "game/game.h"
#include "classes/paraRandom.h"
#include "game/droids.h"
#include "game/score.h"
#include <sdl2_gfx/SDL2_gfxPrimitives.h>

float explosionAnimationSpeed;

#define MY_DEBUG 1

//-------------------------------------------------------------------------------------------------------------
//
// Return the name of a sprite based on its type
std::string gam_getDroidName(int droidType)
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
	return "Error : gam_getDroidName";
}

//-------------------------------------------------------------------------------------------------------------
//
// Create the droids for the level and init values
void gam_initDroids(const std::string &levelName)
//-------------------------------------------------------------------------------------------------------------
{
	int        wayPointStartIndex {};
	int        wayPointSpacing {};
	int        droidIndex {};
	int        whichDirection {};
	paraDroid  tempDroid {};
	paraRandom randomDirection {};

	if (!shipdecks.at (levelName).droid.empty ())
	{
		shipdecks.at (levelName).droid.clear ();
		shipdecks.at (levelName).droid.resize (0);
	}

	wayPointSpacing = shipdecks.at (levelName).numWaypoints / shipdecks.at (levelName).droidTypes.size () - 1;

	shipdecks.at (levelName).numEnemiesAlive = shipdecks.at (levelName).numDroids;
	shipdecks.at (levelName).deckIsDead      = false;

	wayPointStartIndex = 0;

	for (auto &droidItr: shipdecks.at (levelName).droidTypes)
	{
		tempDroid.ai2.initAI ();

		tempDroid.ai2.setArrayIndex (droidIndex);

		tempDroid.setDroidType (droidItr);

		tempDroid.setCurrentHealth (dataBaseEntry[droidItr].maxHealth);

		whichDirection = randomDirection.get (0, 10);
		tempDroid.ai2.setWaypointDirection (PATROL_WAYPOINT_DIRECTION::DOWN);
		if (whichDirection < 5)
			tempDroid.ai2.setWaypointDirection (PATROL_WAYPOINT_DIRECTION::UP);

		tempDroid.ai2.setWorldPosInPixels (cpVect {shipdecks.at (levelName).wayPoints[wayPointStartIndex].x, shipdecks.at (levelName).wayPoints[wayPointStartIndex].y});

		tempDroid.ai2.setDestinationInPixels (tempDroid.ai2.getWorldPosInPixels ());

		tempDroid.ai2.setWaypointIndex (wayPointStartIndex++);
		tempDroid.ai2.setAcceleration (dataBaseEntry[droidItr].accelerate);
		tempDroid.ai2.setMaxSpeed (dataBaseEntry[droidItr].maxSpeed);

		wayPointStartIndex += wayPointSpacing;

		tempDroid.setDroidName (gam_getDroidName (droidItr));

		tempDroid.setCurrentMode (DROID_MODE_NORMAL);
		tempDroid.setWeaponCanFire (true);
		tempDroid.setWeaponDelay (1.0f);

		tempDroid.setIndex (droidIndex);

		tempDroid.sprite.create (tempDroid.getDroidName (), 9, 1.0);

		shipdecks.at (levelName).droid.push_back (tempDroid);

		shipdecks.at (levelName).droidPhysicsCreated = false;

		droidIndex++;
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Reset the droid information for a new game
void gam_resetDroids()
//-------------------------------------------------------------------------------------------------------------
{
	for (auto &shipdeckItr: shipdecks)
	{
		gam_initDroids (shipdeckItr.first);
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Render the droids on the current level
void gam_renderDroids()
//-------------------------------------------------------------------------------------------------------------
{
	cpVect droidScreenPosition {};

	for (auto droidItr: g_shipDeckItr->second.droid)
	{
		if (droidItr.body != nullptr)
		{
			if ((droidItr.getCurrentMode () == DROID_MODE_EXPLODING) || (droidItr.getCurrentMode () == DROID_MODE_NORMAL))
			{
				droidItr.setWorldPosInPixels (cpBodyGetPosition (droidItr.body));

				droidScreenPosition = sys_worldToScreen (droidItr.getWorldPosInPixels (), 24);

				if (droidItr.visibleToPlayer)
				{
					droidItr.sprite.render (droidScreenPosition.x, droidScreenPosition.y, 1.0, static_cast<Uint8>(droidItr.visibleValue));

#ifdef MY_DEBUG_1
					cpVect debugDestinationPosition;

					debugDestinationPosition = droidItr.ai2.debugGetDestinationCoordsInPixels ();

					droidItr.ai2.debugShowDestination ();
					fontClass.use ("consoleFont");

					auto debugIndex = 0;

					if (droidItr.ai2.getCurrentAIMode () == AI2_MODE_PATROL)
						debugIndex = droidItr.ai2.getPatrolWaypointIndex ();
					else
						debugIndex = droidItr.ai2.aStar.getWayPointsIndex ();

					fontClass.render (renderer.renderer, droidScreenPosition.x, droidScreenPosition.y, 200, 200, 250, 255,
									  sys_getString ("%3.3f %3.3f", debugDestinationPosition.x, debugDestinationPosition.y) + " " + droidItr.ai2.getAIActionString (droidItr.ai2.getCurrentAIMode ()) + " Index: " +
									  sys_getString ("%i", debugIndex));
					gam_debugShowTarget (droidItr);
					droidItr.ai2.aStar.debugWayPoints ();
#endif
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Animate the droid sprites
void gam_animateDroids()
//-------------------------------------------------------------------------------------------------------------
{
	bool animationEnd {false};

	for (auto &droidItr: g_shipDeckItr->second.droid)
	{
		if ((droidItr.getCurrentMode () == DROID_MODE_EXPLODING) || (droidItr.getCurrentMode () == DROID_MODE_NORMAL))
			animationEnd = droidItr.sprite.animate ();

		if ((animationEnd) && (droidItr.getCurrentMode () == DROID_MODE_EXPLODING))
		{
			droidItr.setCurrentMode (DROID_MODE_FOR_REMOVAL);
		}

		droidItr.visibleState = VISIBLE_STATE_GO_VISIBLE;
		gam_checkLOS (droidItr);


		//
		// Set fade values
		if (droidItr.visibleState == VISIBLE_STATE_GO_VISIBLE)
		{
			droidItr.visibleValue += visibleFadeValue;
			if (droidItr.visibleValue > 255)
			{
				droidItr.visibleValue    = 255;
				droidItr.visibleToPlayer = true;
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
}

//-----------------------------------------------------------------------------
//
// Recharge a droids weapon fire rate
void gam_weaponRechargeDroid(paraDroid *whichDroid)
//-----------------------------------------------------------------------------
{
	if (!whichDroid->getWeaponCanFire ())
	{
		whichDroid->setWeaponDelay (whichDroid->getWeaponDelay () - 1.0f * 0.02f); //dataBaseEntry[whichDroid.droidType].rechargeTime;
		if (whichDroid->getWeaponDelay () < 0.0)
		{
			whichDroid->setWeaponDelay (1.0f);
			whichDroid->setWeaponCanFire (true);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Process the AI for each droid
void gam_processAI()
//-------------------------------------------------------------------------------------------------------------
{
	for (auto &droidItr: g_shipDeckItr->second.droid)
	{
		gam_checkDroidHealth (&droidItr);

		if (droidItr.getCurrentMode () == DROID_MODE_NORMAL)
		{
			if (dataBaseEntry[droidItr.getDroidType ()].canShoot)
				gam_weaponRechargeDroid (&droidItr);

			if (nullptr == droidItr.body)
				break;

			droidItr.ai2.process (cpBodyGetPosition (droidItr.body));

			droidItr.ai2.doMovement (cpBodyGetPosition (droidItr.body));

			//
			// Check if collisions should be ignored
			if (droidItr.ai2.collisionCounterDroid > 5)
				droidItr.userData->ignoreCollisionDroid = true;

			if (droidItr.ai2.collisionCounterPlayer > 5)
				droidItr.userData->ignoreCollisionPlayer = true;

			//
			// Slowly decrement the collision counter for other droids
			if (droidItr.userData->ignoreCollisionDroid)
			{
				droidItr.ai2.collisionCounterDelayDroid -= 1.0f;
				if (droidItr.ai2.collisionCounterDelayDroid < 0.0)
				{
					droidItr.ai2.collisionCounterDelayDroid = static_cast<float>(droidItr.getDroidType () / 2);
					droidItr.userData->ignoreCollisionDroid = false;
					droidItr.ai2.collisionCounterDroid      = 0;
				}
			}
			//
			// Slowly decrement the collision counter hitting the player
			if (droidItr.userData->ignoreCollisionPlayer)
			{
				droidItr.ai2.collisionCounterDelayPlayer -= 1.0f;
				if (droidItr.ai2.collisionCounterDelayPlayer < 0.0f)
				{
					droidItr.ai2.collisionCounterDelayPlayer = static_cast<float>(droidItr.getDroidType () / 2);
					droidItr.userData->ignoreCollisionPlayer = false;
					droidItr.ai2.collisionCounterPlayer      = 0;
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Process the counter to ignore collisions - called from physicsCollisions as a game event
void gam_processCollision(int droidA)
//-------------------------------------------------------------------------------------------------------------
{
	if (g_shipDeckItr->second.droid[droidA].getCurrentMode () == DROID_MODE_NORMAL)
	{
		g_shipDeckItr->second.droid[droidA].ai2.collisionCounterDroid++;
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Droid has lost all health - explode and remove
void gam_explodeDroid(paraDroid *whichDroid)
//-------------------------------------------------------------------------------------------------------------
{
	whichDroid->ai2.aStar.stopUsingPath ();

	whichDroid->setVelocity (cpVect {0, 0});
	whichDroid->setCurrentMode (DROID_MODE_EXPLODING);
	whichDroid->sprite.create ("explosion", 25, explosionAnimationSpeed);
	whichDroid->sprite.setAnimateSpeed (explosionAnimationSpeed);      // Set for explosion animation

	if (whichDroid->getDroidType () < 10)
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "explode1");
	else
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "explode2");

	gam_addEmitter (whichDroid->getWorldPosInPixels (), PARTICLE_TYPE_EXPLOSION, 0);
}

//-------------------------------------------------------------------------------------------------------------
//
// Get the current mode of the droid
int gam_getDroidCurrentMode(int targetDroid)
//-------------------------------------------------------------------------------------------------------------
{
	return g_shipDeckItr->second.droid[targetDroid].getCurrentMode ();
}

//-------------------------------------------------------------------------------------------------------------
//
// Set velocity to zero
void gam_setDroidVelocity(int targetDroid, cpVect newVelocity)
//-------------------------------------------------------------------------------------------------------------
{
	if (g_shipDeckItr->second.droid[targetDroid].getCurrentMode () != DROID_MODE_EXPLODING)
	{
		g_shipDeckItr->second.droid[targetDroid].setVelocity (newVelocity);
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Work out the droid animation speed based on health
void gam_setHealthAnimation(int targetDroid)
//-------------------------------------------------------------------------------------------------------------
{
	float newAnimationSpeed;

	if (g_shipDeckItr->second.droid[targetDroid].getCurrentMode () != DROID_MODE_EXPLODING)
	{
		newAnimationSpeed     = static_cast<float>(g_shipDeckItr->second.droid[targetDroid].getCurrentHealth ()) / static_cast<float>(dataBaseEntry[g_shipDeckItr->second.droid[targetDroid].getDroidType ()].maxHealth);
		if (newAnimationSpeed < 0.0f)
			newAnimationSpeed = 0.1f;

		if (newAnimationSpeed > 1.0f)
			newAnimationSpeed = 1.0f;

		g_shipDeckItr->second.droid[targetDroid].sprite.setAnimateSpeed (newAnimationSpeed);
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Calculate new droid health percentage and modify AI behaviour
void gam_checkDroidHealth(int targetDroid)
//-------------------------------------------------------------------------------------------------------------
{
	float newHealthPercent {};
//
// Set AI heal value
//
	newHealthPercent = static_cast<float>(g_shipDeckItr->second.droid[targetDroid].getCurrentHealth ()) / static_cast<float>(dataBaseEntry[g_shipDeckItr->second.droid[targetDroid].getDroidType ()].maxHealth);
	newHealthPercent *= 100.0;

	newHealthPercent = newHealthPercent > 100 ? 100 : newHealthPercent;
	newHealthPercent = newHealthPercent < 0 ? 0 : newHealthPercent;

	g_shipDeckItr->second.droid[targetDroid].ai2.setHealValue (newHealthPercent);

	gam_setHealthAnimation (targetDroid);
}

//-------------------------------------------------------------------------------------------------------------
//
// See if a droid sees another droid get shot, or get transferred into. Increase WITNESS value.
void gam_checkActionWitness(int sourceDroid)
//-------------------------------------------------------------------------------------------------------------
{
	for (auto &droidItr: g_shipDeckItr->second.droid)
	{
		if (droidItr.visibleToPlayer)
		{
			if (dataBaseEntry[droidItr.getDroidType ()].canShoot)
			{
				droidItr.ai2.modifyAIScore (AI2_MODE_WITNESS, 40 + difficultyValue);
				droidItr.ai2.setWitnessDroid (sourceDroid);
#ifdef MY_DEBUG
				logFile.write (sys_getString ("[ %i ] - [ %s ] Droid witness an action - increase WITNESS.", droidItr.ai2.getArrayIndex (), __func__));
#endif
			}
			else
				droidItr.ai2.modifyAIScore (AI2_MODE_FLEE, 40 + difficultyValue);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Process damage to a droid
//
// damageSource can be either a bullet, explosion or a collision with player or another droid
void gam_damageToDroid(int targetDroid, int damageSource, int sourceDroid)
//-------------------------------------------------------------------------------------------------------------
{
	logFile.write (sys_getString ("[ %s ] - target %i damageSource %i sourceDroid %i", __func__, targetDroid, damageSource, sourceDroid));

	gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 64, 127, "damage");

	switch (damageSource)
	{
		case PHYSIC_DAMAGE_BUMP:
#ifdef MY_DEBUG
			logFile.write (sys_getString ("[ %s ] - Damage to droid [ %i ] from BUMP", __func__, targetDroid));
#endif
			if ((sourceDroid != TARGET_PLAYER) && (targetDroid != TARGET_PLAYER))     // Two droids colliding
			{
				if (g_shipDeckItr->second.droid[targetDroid].getCurrentMode () == DROID_MODE_EXPLODING)
				{
					g_shipDeckItr->second.droid[sourceDroid].setCurrentHealth (g_shipDeckItr->second.droid[targetDroid].getCurrentHealth () - explosionDamage);
					gam_checkDroidHealth (sourceDroid);
				}

				if (g_shipDeckItr->second.droid[sourceDroid].getCurrentMode () == DROID_MODE_EXPLODING)
				{
					g_shipDeckItr->second.droid[sourceDroid].setCurrentHealth (g_shipDeckItr->second.droid[targetDroid].getCurrentHealth () - explosionDamage);
					gam_checkDroidHealth (targetDroid);
				}
				return;
			}

			if (sourceDroid == TARGET_PLAYER)  // Hit from player
			{
				if (targetDroid < 0)
					return;

				g_shipDeckItr->second.droid[targetDroid].ai2.modifyAIScore (AI2_MODE_ATTACK, 3 + difficultyValue);
				g_shipDeckItr->second.droid[targetDroid].ai2.setTargetDroid (sourceDroid);
				g_shipDeckItr->second.droid[targetDroid].setCurrentHealth (g_shipDeckItr->second.droid[targetDroid].getCurrentHealth () - dataBaseEntry[playerDroid.getDroidType ()].bounceDamage);
				gam_checkDroidHealth (targetDroid);

				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 100, 127, "collision1");
				//
				// Damage to player droid - bump and explosion damage done in this routine
				gam_damageToPlayer (PHYSIC_DAMAGE_BUMP, targetDroid);
				//
				// Damage to player from hitting exploding droid
				if (g_shipDeckItr->second.droid[targetDroid].getCurrentMode () == DROID_MODE_EXPLODING)
				{
					gam_damageToPlayer (PHYSIC_DAMAGE_EXPLOSION, targetDroid);
					return;
				}
			}
			break;

		case PHYSIC_DAMAGE_BULLET:

			//
			// BUG when target is -2 ??
			//
			logFile.write (sys_getString ("[ %s ] - Damage to droid [ %i ] from BULLET", __func__, targetDroid));

			if ((targetDroid != TARGET_PLAYER) && (sourceDroid != TARGET_PLAYER) && (targetDroid != NO_ATTACK_TARGET))     // Droids shot this at another droid
			{
				//
				// Set the target to the bullets source
				if (dataBaseEntry[g_shipDeckItr->second.droid[targetDroid].getDroidType ()].canShoot)
				{
					g_shipDeckItr->second.droid[targetDroid].ai2.setTargetDroid (sourceDroid);
					g_shipDeckItr->second.droid[targetDroid].ai2.modifyAIScore (AI2_MODE_ATTACK, 40 + difficultyValue);
				}
				else    // Can't shoot back
				{
					g_shipDeckItr->second.droid[targetDroid].ai2.modifyAIScore (AI2_MODE_FLEE, 30);
				}

				//
				// Do the bullet damage to target droid
				g_shipDeckItr->second.droid[targetDroid].setCurrentHealth (g_shipDeckItr->second.droid[targetDroid].getCurrentHealth () - dataBaseEntry[g_shipDeckItr->second.droid[sourceDroid].getDroidType ()].bulletDamage);
				gam_checkDroidHealth (targetDroid);
				return;
			}

			if ((targetDroid == TARGET_PLAYER) && (sourceDroid != TARGET_PLAYER))      // Droid hit player with bullet
			{
				gam_damageToPlayer (PHYSIC_DAMAGE_BULLET, sourceDroid);
				return;
			}

			if ((sourceDroid == TARGET_PLAYER) && (targetDroid != TARGET_PLAYER))      // Player shot this bullet at a droid
			{
				gam_checkActionWitness (sourceDroid);
				//
				// Bullet does damage according to current droid type if it has a weapon
				if (dataBaseEntry[playerDroid.getDroidType ()].canShoot)
					g_shipDeckItr->second.droid[targetDroid].setCurrentHealth (g_shipDeckItr->second.droid[targetDroid].getCurrentHealth () - dataBaseEntry[playerDroid.getDroidType ()].bulletDamage);
				else
					g_shipDeckItr->second.droid[targetDroid].setCurrentHealth (g_shipDeckItr->second.droid[targetDroid].getCurrentHealth () - dataBaseEntry[0].bulletDamage);

				gam_checkDroidHealth (targetDroid);
				//
				// Target player droid
				g_shipDeckItr->second.droid[targetDroid].ai2.setTargetDroid (sourceDroid);
				g_shipDeckItr->second.droid[targetDroid].ai2.modifyAIScore (AI2_MODE_ATTACK, 40 + difficultyValue);
			}
			break;
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Check for any droids that need to be removed - died last tick
void gam_removeDroids()
//-------------------------------------------------------------------------------------------------------------
{
	for (auto &droidItr: g_shipDeckItr->second.droid)
	{
		if (droidItr.getCurrentMode () == DROID_MODE_FOR_REMOVAL)
		{
			//
			// Remove this dead droid from any that may be targeting it
			for (auto &removeTargetItr: g_shipDeckItr->second.droid)
			{
				if (removeTargetItr.ai2.getTargetDroid () == droidItr.getIndex ())
				{
#ifdef MY_DEBUG
					logFile.write (sys_getString ("[ %i ] - Removing dead droid with target of [ %i ]", droidItr.getIndex (), removeTargetItr.getIndex ()));
#endif
					removeTargetItr.ai2.setTargetDroid (NO_ATTACK_TARGET);
					removeTargetItr.ai2.modifyAIScore (AI2_MODE_ATTACK, -55);
				}
			}

			g_shipDeckItr->second.numEnemiesAlive--;

			droidItr.setCurrentMode (DROID_MODE_DEAD);
			//
			// Remove physics
			if (droidItr.body != nullptr)
			{
				if (droidItr.shape != nullptr)
				{
					cpSpaceRemoveShape (sys_returnPhysicsWorld (), droidItr.shape);
					droidItr.shape = nullptr;
				}
				if (droidItr.body != nullptr)
				{
					cpSpaceRemoveBody (sys_returnPhysicsWorld (), droidItr.body);
					droidItr.body = nullptr;
				}
			}
			//
			// add to score
			gam_modifyScore (dataBaseEntry[droidItr.getDroidType ()].score);
		}
	}
	//
	// See if the deck is empty
	if (g_shipDeckItr->second.numEnemiesAlive == 0)
	{
		gam_setDeckIsDead ();
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Show the current droids attack target
void gam_debugShowTarget(paraDroid whichDroid)
//-------------------------------------------------------------------------------------------------------------
{
	cpVect endPos {};

	if (dataBaseEntry[whichDroid.getDroidType ()].canShoot)
	{
		if (whichDroid.ai2.getTargetDroid () != NO_ATTACK_TARGET)
		{
			auto startPos = whichDroid.getWorldPosInPixels ();

			if (whichDroid.ai2.getTargetDroid () == TARGET_PLAYER)
				endPos = playerDroid.getWorldPosInPixels ();
			else
				endPos = g_shipDeckItr->second.droid[whichDroid.ai2.getTargetDroid ()].getWorldPosInPixels ();

			startPos = sys_worldToScreen (startPos, 100);
			endPos   = sys_worldToScreen (endPos, 100);

			thickLineRGBA (renderer.renderer, static_cast<Sint16>(startPos.x), static_cast<Sint16>(startPos.y), static_cast<Sint16>(endPos.x), static_cast<Sint16>(endPos.y), 2, 255, 0, 0, 255);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// See if droid is dead - set explosion sprite and change mode
//
// gam_explodeDroid changes the currentMode
void gam_checkDroidHealth(paraDroid *whichDroid)
//-------------------------------------------------------------------------------------------------------------
{
	if (whichDroid->getCurrentMode () == DROID_MODE_NORMAL)
	{
		if (whichDroid->getCurrentHealth () < 0)
		{
			whichDroid->ai2.setTargetDroid (NO_ATTACK_TARGET);
			whichDroid->ai2.modifyAIScore (AI2_MODE_ATTACK, -100);

			gam_explodeDroid (whichDroid);
		}
	}
}