#include <game/database.h>
#include <game/shipDecks.h>
#include <system/util.h>
#include <game/player.h>
#include <game/audio.h>
#include <game/particles.h>
#include <game/pathFind.h>
#include <game/lineOfSight.h>
#include <game/texture.h>
#include <game/game.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <classes/paraRandom.h>
#include "game/droids.h"
#include "game/score.h"

float explosionAnimationSpeed;

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
	return "Error : gam_getDroidName";
}

//-------------------------------------------------------------------------------------------------------------
//
// Create the droids for the level and init values
void gam_initDroids (const std::string &levelName)
//-------------------------------------------------------------------------------------------------------------
{
	int        wayPointStartIndex = 0;
	int        wayPointSpacing    = 0;
	int        droidIndex         = 0;
	int        whichDirection;
	droidClass tempDroid;
	paraRandom randomDirection;

	if (!shipdecks.at (levelName).droid.empty ())
	{
		shipdecks.at (levelName).droid.clear ();
		shipdecks.at (levelName).droid.resize (0);
	}

	wayPointSpacing = shipdecks.at (levelName).numWaypoints / shipdecks.at (levelName).droidTypes.size () - 1;

	shipdecks.at (levelName).numEnemiesAlive = shipdecks.at (levelName).numDroids;
	shipdecks.at (levelName).deckIsDead      = false;

	for (auto &droidItr : shipdecks.at (levelName).droidTypes)
	{
		tempDroid.ai.initAI ();
		tempDroid.ai.setArrayIndex (droidIndex);
		tempDroid.droidType     = droidItr;
		tempDroid.currentHealth = dataBaseEntry[tempDroid.droidType].maxHealth;

		wayPointStartIndex = 0;
		whichDirection     = randomDirection.get (0, 10);
		if (whichDirection < 5)
			tempDroid.ai.setWaypointDirection (WAYPOINT_UP);
		else
			tempDroid.ai.setWaypointDirection (WAYPOINT_DOWN);

		tempDroid.worldPosInPixels.x = shipdecks.at (levelName).wayPoints[wayPointStartIndex].x;
		tempDroid.worldPosInPixels.y = shipdecks.at (levelName).wayPoints[wayPointStartIndex++].y;
		tempDroid.ai.setWaypointIndex (wayPointStartIndex);

		wayPointStartIndex += wayPointSpacing;

		tempDroid.droidName = gam_getDroidName (tempDroid.droidType);

		tempDroid.ai.setAcceleration (dataBaseEntry[tempDroid.droidType].accelerate);
		tempDroid.ai.setMaxSpeed (dataBaseEntry[tempDroid.droidType].maxSpeed);

		tempDroid.currentMode   = DROID_MODE_NORMAL;
		tempDroid.weaponCanFire = true;
		tempDroid.weaponDelay   = 1.0f;

		tempDroid.sprite.create (tempDroid.droidName, 9, 1.0);

		shipdecks.at (levelName).droid.push_back (tempDroid);

		shipdecks.at (levelName).droidPhysicsCreated = false;

		droidIndex++;
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Reset the droid information for a new game
void gam_resetDroids ()
//-------------------------------------------------------------------------------------------------------------
{
	for (auto &shipdeckItr : shipdecks)
	{
		gam_initDroids (shipdeckItr.first);
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Render the droids on the current level
void gam_renderDroids (std::string levelName)
//-------------------------------------------------------------------------------------------------------------
{
	b2Vec2 droidScreenPosition{};
	b2Vec2 worldPosInMeters;

	for (auto droidItr : g_shipDeckItr->second.droid)
	{
		if (droidItr.body != nullptr)
		{
			if ((droidItr.currentMode == DROID_MODE_EXPLODING) || (droidItr.currentMode == DROID_MODE_NORMAL))
			{
				worldPosInMeters = droidItr.body->GetPosition ();
				droidItr.worldPosInPixels.x = worldPosInMeters.x * pixelsPerMeter;
				droidItr.worldPosInPixels.y = worldPosInMeters.y * pixelsPerMeter;

				droidScreenPosition.x = droidItr.worldPosInPixels.x;
				droidScreenPosition.y = droidItr.worldPosInPixels.y;

				droidScreenPosition = sys_worldToScreen (droidScreenPosition, 24);
				if (droidItr.currentMode == DROID_MODE_NORMAL)
					droidItr.sprite.setTintColor (0, 0, 0);     // Draw in black color
				else
					droidItr.sprite.setTintColor (255, 255, 255);  // Full color for explosion

				if (droidItr.visibleToPlayer)
				{
					droidItr.sprite.render (droidScreenPosition.x, droidScreenPosition.y, 1.0, static_cast<Uint8>(droidItr.visibleValue));
#ifdef MY_DEBUG
					fontClass.use ("guiFont");
					fontClass.render (renderer.renderer, droidScreenPosition.x, droidScreenPosition.y, 0, 0, 0, 255, droidItr.droidName + " " + droidItr.ai.getString (droidItr.ai.getCurrentMode()));
#endif
				}
//				droidItr.ai.getCurrentMode()

//				droidItr.ai.renderVelocity ();

//				gam_debugShowTarget (droidItr);
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Animate the droid sprites
void gam_animateDroids ()
//-------------------------------------------------------------------------------------------------------------
{
	bool animationEnd = false;

	for (auto &droidItr : g_shipDeckItr->second.droid)
	{
		if ((droidItr.currentMode == DROID_MODE_EXPLODING) || (droidItr.currentMode == DROID_MODE_NORMAL))
			animationEnd = droidItr.sprite.animate ();

		if ((animationEnd) && (droidItr.currentMode == DROID_MODE_EXPLODING))
		{
			droidItr.currentMode = DROID_MODE_FOR_REMOVAL;
		}

		gam_checkLOS (droidItr);
	}
}

//-----------------------------------------------------------------------------
//
// Recharge a droids weapon fire rate
void gam_weaponRechargeDroid (droidClass *whichDroid)
//-----------------------------------------------------------------------------
{
	if (!whichDroid->weaponCanFire)
	{
		whichDroid->weaponDelay -= 1.0f * 0.02f; //dataBaseEntry[whichDroid.droidType].rechargeTime;
		if (whichDroid->weaponDelay < 0.0)
		{
			whichDroid->weaponDelay   = 1.0f;
			whichDroid->weaponCanFire = true;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Process the AI for each droid
void gam_processAI ()
//-------------------------------------------------------------------------------------------------------------
{
	for (auto &droidItr : g_shipDeckItr->second.droid)
	{
		gam_checkDroidHealth (&droidItr);

		if (droidItr.currentMode == DROID_MODE_NORMAL)
		{
			if (dataBaseEntry[droidItr.droidType].canShoot)
				gam_weaponRechargeDroid (&droidItr);

			if (nullptr == droidItr.body)
				break;

			droidItr.ai.process (droidItr.body->GetPosition ());
			droidItr.body->ApplyForce (droidItr.ai.getVelocity (), droidItr.body->GetWorldCenter (), true);
			//
			// Slowly decrement the collision counter
			if (droidItr.userData->ignoreCollision)
			{
				droidItr.collisionCounterDelay -= 0.3f;
				if (droidItr.collisionCounterDelay < 0.0)
				{
					droidItr.collisionCounterDelay     = collisionCount;
					droidItr.userData->ignoreCollision = false;
					droidItr.collisionCounter          = 0;
				}
			}
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Process the counter to ignore collisions
void gam_processCollision (int droidA)
//-------------------------------------------------------------------------------------------------------------
{
	if (g_shipDeckItr->second.droid[droidA].currentMode == DROID_MODE_NORMAL)
	{
		if (g_shipDeckItr->second.droid[droidA].userData == nullptr)
		{
			printf("Error: Userdata is set to null - droid [ %i ]\n", droidA);
			return;
		}
		if (g_shipDeckItr->second.droid[droidA].userData->ignoreCollision)
			return;

		g_shipDeckItr->second.droid[droidA].ai.swapWaypointDirection ();
		g_shipDeckItr->second.droid[droidA].collisionCounter++;
		if (g_shipDeckItr->second.droid[droidA].collisionCounter > collisionLimit)
		{
			g_shipDeckItr->second.droid[droidA].collisionCounterDelay     = collisionCount;
			g_shipDeckItr->second.droid[droidA].userData->ignoreCollision = true;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Droid has lost all health - explode and remove
void gam_explodeDroid (int droidIndex)
//-------------------------------------------------------------------------------------------------------------
{
	g_shipDeckItr->second.droid[droidIndex].velocity    = {0, 0};
	g_shipDeckItr->second.droid[droidIndex].currentMode = DROID_MODE_EXPLODING;
	g_shipDeckItr->second.droid[droidIndex].sprite.create ("explosion", 25, explosionAnimationSpeed);
	g_shipDeckItr->second.droid[droidIndex].sprite.setAnimateSpeed (explosionAnimationSpeed);      // Set for explosion animation

	if (g_shipDeckItr->second.droid[droidIndex].droidType < 10)
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "explode1");
	else
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "explode2");

	gam_addEmitter (sys_convertToMeters (g_shipDeckItr->second.droid[droidIndex].worldPosInPixels), PARTICLE_TYPE_EXPLOSION, 0);
}

//-------------------------------------------------------------------------------------------------------------
//
// Work out the droid animation speed based on health
void gam_setHealthAnimation (int targetDroid)
//-------------------------------------------------------------------------------------------------------------
{
	float newAnimationSpeed;

	if (g_shipDeckItr->second.droid[targetDroid].currentMode != DROID_MODE_EXPLODING)
	{
		newAnimationSpeed     = static_cast<float>(g_shipDeckItr->second.droid[targetDroid].currentHealth) / static_cast<float>(dataBaseEntry[g_shipDeckItr->second.droid[targetDroid].droidType].maxHealth);
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
	float newHealthPercent;

//
// Do AI to see if Droid needs to heal
//
	newHealthPercent = static_cast<float>(g_shipDeckItr->second.droid[targetDroid].currentHealth) / static_cast<float>(dataBaseEntry[g_shipDeckItr->second.droid[targetDroid].droidType].maxHealth);
	newHealthPercent *= 100.0;

	newHealthPercent = newHealthPercent > 100 ? 100 : newHealthPercent;
	newHealthPercent = newHealthPercent < 0 ? 0 : newHealthPercent;

	g_shipDeckItr->second.droid[targetDroid].ai.setHealthPercent (newHealthPercent);
	g_shipDeckItr->second.droid[targetDroid].ai.checkHealth ();

	gam_setHealthAnimation (targetDroid);
}

//-------------------------------------------------------------------------------------------------------------
//
// See if a droid sees another droid get shot, or get transferred into
void gam_checkActionWitness()
//-------------------------------------------------------------------------------------------------------------
{
	for (auto &droidItr : g_shipDeckItr->second.droid)
	{
		if (droidItr.visibleToPlayer)
		{
			if (dataBaseEntry[droidItr.droidType].canShoot)
			{
				droidItr.ai.setTargetDroid(-1);
				droidItr.ai.modifyScore (AI_MODE_ATTACK, +40);

				printf("Droid witness an action - increase ATTACK.\n");

			}
			else
				droidItr.ai.modifyScore(AI_MODE_FLEE, +40);
		}
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Process damage to a droid
//
// damageSource can be either a bullet, explosion or a collision with player or another droid
void gam_damageToDroid (int targetDroid, int damageSource, int sourceDroid)
//-------------------------------------------------------------------------------------------------------------
{

	printf ("gam_damageToDroid target %i damageSource %i sourceDroid %i\n", targetDroid, damageSource, sourceDroid);


//	if (g_shipDeckItr->second.droid[targetDroid].currentMode != DROID_MODE_NORMAL)
//		return;




	gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 64, 127, "damage");

	switch (damageSource)
	{
		case PHYSIC_DAMAGE_BUMP:
#ifdef MY_DEBUG
			std::cout << "Damage to Droid " << targetDroid << " from BUMP" << std::endl;
#endif
			if ((sourceDroid != -1) && (targetDroid != -1))     // Two droids colliding
			{
				if (g_shipDeckItr->second.droid[targetDroid].currentMode == DROID_MODE_EXPLODING)
				{
					g_shipDeckItr->second.droid[sourceDroid].currentHealth -= explosionDamage;
					gam_checkDroidHealth(sourceDroid);
				}

				if (g_shipDeckItr->second.droid[sourceDroid].currentMode == DROID_MODE_EXPLODING)
				{
					g_shipDeckItr->second.droid[targetDroid].currentHealth -= explosionDamage;
					gam_checkDroidHealth(targetDroid);
				}
				return;
			}

			if (sourceDroid == -1)  // Hit from player
			{
				if (targetDroid < 0)
					return;

				g_shipDeckItr->second.droid[targetDroid].ai.modifyScore (AI_MODE_ATTACK, 10);
				g_shipDeckItr->second.droid[targetDroid].ai.setTargetDroid (sourceDroid);
				g_shipDeckItr->second.droid[targetDroid].currentHealth -= dataBaseEntry[playerDroid.droidType].bounceDamage;
				gam_checkDroidHealth(targetDroid);

				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 100, 127, "collision1");
				//
				// Damage to player droid - bump and explosion damage done in this routine
				gam_damageToPlayer (PHYSIC_DAMAGE_BUMP, targetDroid);
				//
				// Damage to player from hitting exploding droid
				if (g_shipDeckItr->second.droid[targetDroid].currentMode == DROID_MODE_EXPLODING)
				{
					gam_damageToPlayer (PHYSIC_DAMAGE_EXPLOSION, targetDroid);
				}
			}
			break;

		case PHYSIC_DAMAGE_BULLET:

			//
			// BUG when target is -2 ??
			//
			std::cout << "Damage to Droid " << targetDroid << " from BULLET " << " from " << sourceDroid << std::endl;

			if ((targetDroid != TARGET_PLAYER) && (sourceDroid != TARGET_PLAYER))     // Droids shot this at another droid
			{
				//
				// Set the target to the bullets source
				if (dataBaseEntry[g_shipDeckItr->second.droid[targetDroid].droidType].canShoot)
				{
					g_shipDeckItr->second.droid[targetDroid].ai.setTargetDroid (sourceDroid);
					g_shipDeckItr->second.droid[targetDroid].ai.modifyScore (AI_MODE_ATTACK, 40);
				}
				else    // Cant shoot back
				{
					g_shipDeckItr->second.droid[targetDroid].ai.modifyScore (AI_MODE_FLEE, 30);
				}

				//
				// Do the bullet damage to target droid
				g_shipDeckItr->second.droid[targetDroid].currentHealth -= dataBaseEntry[g_shipDeckItr->second.droid[sourceDroid].droidType].bulletDamage;
				gam_checkDroidHealth(targetDroid);
			}

			if ((targetDroid == TARGET_PLAYER) && (sourceDroid != TARGET_PLAYER))      // Droid hit player with bullet
			{
				gam_damageToPlayer (PHYSIC_DAMAGE_BULLET, sourceDroid);
			}

			if ((sourceDroid == TARGET_PLAYER) && (targetDroid != TARGET_PLAYER))      // Player shot this bullet at a droid
			{
				gam_checkActionWitness ();
				//
				// Bullet does damage according to current droid type if it has a weapon
				if (dataBaseEntry[playerDroid.droidType].canShoot)
					g_shipDeckItr->second.droid[targetDroid].currentHealth -= dataBaseEntry[playerDroid.droidType].bulletDamage;
				else
					g_shipDeckItr->second.droid[targetDroid].currentHealth -= dataBaseEntry[0].bulletDamage;

				gam_checkDroidHealth(targetDroid);
				//
				// Target player droid
				g_shipDeckItr->second.droid[targetDroid].ai.setTargetDroid (sourceDroid);
				g_shipDeckItr->second.droid[targetDroid].ai.modifyScore (AI_MODE_ATTACK, 40);
			}
			break;
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Check for any droids that need to be removed - died last tick
void gam_removeDroids (bool clearAll)
//-------------------------------------------------------------------------------------------------------------
{
	for (auto &droidItr : g_shipDeckItr->second.droid)
	{
		if (droidItr.currentMode == DROID_MODE_FOR_REMOVAL)
		{
			//
			// Remove this dead droid from any that may be targeting it
			for (auto &removeTargetItr : g_shipDeckItr->second.droid)
			{
				if (removeTargetItr.ai.getTargetDroid () == droidItr.index)
				{
					std::cout << "Removing dead droid : " << droidItr.index << " as target for : " << removeTargetItr.index << std::endl;

					removeTargetItr.ai.setTargetDroid (NO_ATTACK_TARGET);
					removeTargetItr.ai.modifyScore (AI_MODE_ATTACK, -55);
				}
			}

			g_shipDeckItr->second.numEnemiesAlive--;

			droidItr.currentMode = DROID_MODE_DEAD;
			//
			// Remove physics
			if (droidItr.body != nullptr)
			{
				droidItr.body->GetWorld ()->DestroyBody (droidItr.body);
				droidItr.body = nullptr;
				//
				// Free memory
				delete droidItr.userData;
				droidItr.userData = nullptr;
			}
			//
			// Remove any path it may have been following
			gam_removeWhichDroidPath (droidItr.ai.getArrayIndex ());

			if (droidItr.aStarIndex > -1)
			{
				std::cout << "Removing aStar path from removed droid : " << droidItr.aStarIndex << std::endl;
				gam_AStarRemovePath (droidItr.aStarIndex);
			}
			//
			// add to score
			gam_modifyScore (dataBaseEntry[droidItr.droidType].score);
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
void gam_debugShowTarget (droidClass whichDroid)
//-------------------------------------------------------------------------------------------------------------
{
	b2Vec2 startPos;
	b2Vec2 endPos;

	if (dataBaseEntry[whichDroid.droidType].canShoot)
	{
		if (whichDroid.ai.getTargetDroid () != NO_ATTACK_TARGET)
		{
			startPos = whichDroid.worldPosInPixels;

			if (whichDroid.ai.getTargetDroid () == -1)
				endPos = playerDroid.worldPosInPixels;
			else
				endPos = g_shipDeckItr->second.droid[whichDroid.ai.getTargetDroid ()].worldPosInPixels;

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
void gam_checkDroidHealth (droidClass *whichDroid)
//-------------------------------------------------------------------------------------------------------------
{
	if (whichDroid->currentMode == DROID_MODE_NORMAL)
	{
		if (whichDroid->currentHealth < 0)
		{
			whichDroid->ai.setTargetDroid (NO_ATTACK_TARGET);
			whichDroid->ai.modifyScore (AI_MODE_ATTACK, -100);

			gam_explodeDroid (whichDroid->ai.getArrayIndex ());
		}
	}
}