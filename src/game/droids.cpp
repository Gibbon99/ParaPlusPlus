#include <game/database.h>
#include <game/shipDecks.h>
#include <system/util.h>
#include <game/player.h>
#include <game/audio.h>
#include <game/particles.h>
#include <game/pathFind.h>
#include <game/lineOfSight.h>
#include "game/droids.h"
#include "game/score.h"

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
	int        wayPointStartIndex = 0;
	int        droidIndex         = 0;
	droidClass tempDroid;

	if (!shipdecks.at (levelName).droid.empty ())
	{
		shipdecks.at (levelName).droid.clear ();
		shipdecks.at (levelName).droid.resize (0);
	}

	shipdecks.at (levelName).numEnemiesAlive = shipdecks.at (levelName).numDroids;
	for (auto &droidItr : shipdecks.at (levelName).droidTypes)
	{
		tempDroid.ai.initAI();
		tempDroid.ai.setArrayIndex (droidIndex);
		tempDroid.droidType          = droidItr;
		tempDroid.currentHealth      = dataBaseEntry[tempDroid.droidType].maxHealth;
		tempDroid.worldPosInPixels.x = shipdecks.at (levelName).wayPoints[wayPointStartIndex].x;
		tempDroid.worldPosInPixels.y = shipdecks.at (levelName).wayPoints[wayPointStartIndex++].y;
		tempDroid.ai.setWaypointIndex (wayPointStartIndex);
		if (wayPointStartIndex > shipdecks.at (levelName).numWaypoints)
			shipdecks.at (levelName).numWaypoints = 0;

		tempDroid.droidName = gam_getDroidName (tempDroid.droidType);

		tempDroid.ai.setAcceleration (dataBaseEntry[tempDroid.droidType].accelerate);
		tempDroid.ai.setMaxSpeed (dataBaseEntry[tempDroid.droidType].maxSpeed);

		tempDroid.currentMode = DROID_MODE_NORMAL;

		tempDroid.sprite.create (tempDroid.droidName, 9, 0.3);

		shipdecks.at (levelName).droid.push_back (tempDroid);

		droidIndex++;
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
		if ((droidItr.currentMode == DROID_MODE_EXPLODING) ||
		    (droidItr.currentMode == DROID_MODE_NORMAL) )
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
				droidItr.sprite.setTintColor(255,255,255);  // Full color for explosion

			if (droidItr.visibleToPlayer)
				droidItr.sprite.render (droidScreenPosition.x, droidScreenPosition.y, 1.0, static_cast<Uint8>(droidItr.visibleValue));

//			droidItr.ai.renderVelocity ();
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

		gam_checkLOS(droidItr);
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
		if (droidItr.currentMode == DROID_MODE_NORMAL)
		{
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
		if (g_shipDeckItr->second.droid[droidA].userData->ignoreCollision)
			return;

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
// Process damage to a droid
//
// damageSource can be either a bullet, explosion or a collision with player or another droid
void gam_damageToDroid (int targetDroid, int damageSource, int sourceDroid)
//-------------------------------------------------------------------------------------------------------------
{
	float newHealthPercent;

	if (g_shipDeckItr->second.droid[targetDroid].currentMode != DROID_MODE_NORMAL)
		return;

	switch (damageSource)
	{
		case PHYSIC_DAMAGE_BUMP:

			if (sourceDroid == -1)  // Hit from player
				g_shipDeckItr->second.droid[targetDroid].currentHealth -= dataBaseEntry[playerDroid.droidType].bounceDamage;
//			else
//				g_shipDeckItr->second.droid[targetDroid].currentHealth -= dataBaseEntry[g_shipDeckItr->second.droid[sourceDroid].droidType].bounceDamage;

			g_shipDeckItr->second.droid[targetDroid].currentHealth < 0 ? 0 : g_shipDeckItr->second.droid[targetDroid].currentHealth;
			break;

		case PHYSIC_DAMAGE_BULLET:

			g_shipDeckItr->second.droid[targetDroid].ai.setTargetDroid (sourceDroid);
//			g_shipDeckItr->second.droid[targetDroid].ai.modifyScore (AI_MODE_ATTACK, 40);

			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "damage");

			if (-1 == sourceDroid)      // Player shot this bullet
			{
				if (dataBaseEntry[playerDroid.droidType].canShoot)
					g_shipDeckItr->second.droid[targetDroid].currentHealth -= dataBaseEntry[playerDroid.droidType].bulletDamage;
				else
					g_shipDeckItr->second.droid[targetDroid].currentHealth -= dataBaseEntry[0].bulletDamage;

				//
				// See if droid is dead - set explosion sprite and change mode
				if (g_shipDeckItr->second.droid[targetDroid].currentHealth < 0)
				{
					g_shipDeckItr->second.droid[targetDroid].velocity = {0,0};
					g_shipDeckItr->second.droid[targetDroid].currentMode = DROID_MODE_EXPLODING;
					g_shipDeckItr->second.droid[targetDroid].sprite.create("explosion", 25, 1.4f);

					if (g_shipDeckItr->second.droid[targetDroid].droidType < 10)
						gam_addAudioEvent(EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "explode1");
					else
						gam_addAudioEvent(EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "explode2");
					gam_addEmitter(sys_convertToMeters(g_shipDeckItr->second.droid[targetDroid].worldPosInPixels), PARTICLE_TYPE_EXPLOSION, 0);
				}
			}
			else        // Droid shot this bullet
			{
				if (sourceDroid == targetDroid)
					return;

				if (targetDroid != -1)  // Not the player hit by a bullet
				{
					g_shipDeckItr->second.droid[targetDroid].currentHealth -= dataBaseEntry[g_shipDeckItr->second.droid[targetDroid].droidType].bulletDamage;
				}
				else
				{
					//
					// Player hit by bullet
					playerDroid.currentHealth -= dataBaseEntry[g_shipDeckItr->second.droid[targetDroid].droidType].bulletDamage;

					std::cout << "Player hit by bullet" << std::endl;
				}
			}
			break;
	}

	newHealthPercent = static_cast<float>(shipdecks.at (gam_getCurrentDeckName ()).droid[targetDroid].currentHealth) / static_cast<float>(dataBaseEntry[shipdecks.at (gam_getCurrentDeckName ()).droid[targetDroid].droidType].maxHealth);
	newHealthPercent *= 100.0;

	newHealthPercent = newHealthPercent > 100 ? 100 : newHealthPercent;
	newHealthPercent = newHealthPercent < 0 ? 0 : newHealthPercent;

	shipdecks.at (gam_getCurrentDeckName ()).droid[targetDroid].ai.setHealthPercent (newHealthPercent);
	shipdecks.at (gam_getCurrentDeckName ()).droid[targetDroid].ai.checkHealth ();
}

//-------------------------------------------------------------------------------------------------------------
//
// Check for any droids that need to be removed - died last tick
void gam_removeDroids()
//-------------------------------------------------------------------------------------------------------------
{
	for (auto &droidItr : g_shipDeckItr->second.droid)
	{
		if (droidItr.currentMode == DROID_MODE_FOR_REMOVAL)
		{
			g_shipDeckItr->second.numEnemiesAlive--;

			droidItr.currentMode = DROID_MODE_DEAD;
			//
			// Remove physics
			droidItr.body->GetWorld()->DestroyBody(droidItr.body);
			droidItr.body = nullptr;
			//
			// Free memory
			delete droidItr.userData;
			droidItr.userData = nullptr;
			//
			// Remove any path it may have been following
			gam_removeWhichDroidPath(droidItr.ai.getArrayIndex());

			if (droidItr.aStarIndex > -1)
			{
 				std::cout << "Removing aStar path from removed droid : " << droidItr.aStarIndex << std::endl;
				gam_AStarRemovePath (droidItr.aStarIndex);
			}
			//
			// add to score
			gam_modifyScore(dataBaseEntry[droidItr.droidType].score);
		}
	}

	//
	// See if the deck is empty
	if (g_shipDeckItr->second.numEnemiesAlive == 0)
	{
		// TODO - Change tile set to empty - play sound - add score
	}
}