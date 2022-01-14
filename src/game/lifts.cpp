#include <string>
#include "io/logFile.h"
#include "gui/guiSideview.h"
#include "system/util.h"
#include "game/audio.h"
#include "game/player.h"
#include "game/alertLevel.h"
#include "game/shipDecks.h"
#include "game/lifts.h"

std::vector<__tileSensor> lifts;
int                       currentTunnel;

struct __TUNNEL_LINKS
{
	std::vector<int> linkedDecks;
};

__TUNNEL_LINKS tunnelLinks[8];

static int currentTunnelDeckIndex;

//----------------------------------------------------------------------------------------------------------------------------
//
// Return the current tunnel in use
int gam_getCurrentTunnel()
//----------------------------------------------------------------------------------------------------------------------------
{
	return currentTunnel;
}

//----------------------------------------------------------------------------------------------------------------------------
//
// Set the deck index
void gam_setCurrentTunnelDeckIndex()
//----------------------------------------------------------------------------------------------------------------------------
{
	int tempTunnel = 0;
	currentTunnelDeckIndex = 0;

	for (auto tunnelItr: shipdecks.at (gam_getCurrentDeckName ()).liftClass.tunnels)
	{
		if (tunnelItr.second == playerDroid.getLiftIndex ())
		{
			tempTunnel = tunnelItr.first;
			break;
		}
	}

	for (auto i = 0; i != static_cast<int>(tunnelLinks[tempTunnel].linkedDecks.size ()); i++)
	{
		if (tunnelLinks[tempTunnel].linkedDecks[i] == currentDeckNumber)
		{
			currentTunnelDeckIndex = i;
			return;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------------
//
// Create lookup from a tunnel to decks it connects to
void gam_setupTunnelLinks()
//----------------------------------------------------------------------------------------------------------------------------
{
	tunnelLinks[0].linkedDecks.push_back (19);
	tunnelLinks[0].linkedDecks.push_back (16);
	tunnelLinks[0].linkedDecks.push_back (15);
	tunnelLinks[0].linkedDecks.push_back (4);
	tunnelLinks[0].linkedDecks.push_back (3);

	tunnelLinks[1].linkedDecks.push_back (6);
	tunnelLinks[1].linkedDecks.push_back (5);
	tunnelLinks[1].linkedDecks.push_back (0);

	tunnelLinks[2].linkedDecks.push_back (17);
	tunnelLinks[2].linkedDecks.push_back (15);

	tunnelLinks[3].linkedDecks.push_back (20);
	tunnelLinks[3].linkedDecks.push_back (17);
	tunnelLinks[3].linkedDecks.push_back (14);
	tunnelLinks[3].linkedDecks.push_back (13);
	tunnelLinks[3].linkedDecks.push_back (12);

	tunnelLinks[4].linkedDecks.push_back (12);
	tunnelLinks[4].linkedDecks.push_back (6);
	tunnelLinks[4].linkedDecks.push_back (5);
	tunnelLinks[4].linkedDecks.push_back (1);

	tunnelLinks[5].linkedDecks.push_back (9);
	tunnelLinks[5].linkedDecks.push_back (8);
	tunnelLinks[5].linkedDecks.push_back (2);

	tunnelLinks[6].linkedDecks.push_back (14);
	tunnelLinks[6].linkedDecks.push_back (13);
	tunnelLinks[6].linkedDecks.push_back (12);
	tunnelLinks[6].linkedDecks.push_back (11);
	tunnelLinks[6].linkedDecks.push_back (10);
	tunnelLinks[6].linkedDecks.push_back (9);

	tunnelLinks[7].linkedDecks.push_back (18);
	tunnelLinks[7].linkedDecks.push_back (14);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Move the lift position
void gam_moveLift(int direction)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (direction)
	{
		case 2: //DOWN
			if (currentTunnelDeckIndex != static_cast<int>(tunnelLinks[currentTunnel].linkedDecks.size () - 1))
			{
				currentTunnelDeckIndex++;
				currentDeckNumber = tunnelLinks[currentTunnel].linkedDecks[currentTunnelDeckIndex];
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "liftButton");
			}
			else
			{
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
			}

			break;

		case 1: //UP
			if (currentTunnelDeckIndex != 0)
			{
				currentTunnelDeckIndex--;
				currentDeckNumber = tunnelLinks[currentTunnel].linkedDecks[currentTunnelDeckIndex];
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "liftButton");
			}
			else
			{
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
			}

			break;

		case 3: // ACTION

			if (currentTunnelDeckIndex > 6)
			{
				currentTunnelDeckIndex = 0;
			}

			gam_changeToDeck (gam_returnLevelNameFromDeck (currentDeckNumber), shipdecks.at (gam_returnLevelNameFromDeck (currentDeckNumber)).liftClass.getLiftIndex (currentTunnel));

			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");

			playerDroid.setVelocity (cpvzero);

			sys_setNewMode (MODE_GAME, true);

			break;

		default:
			break;
	}
}


//----------------------------------------------------------------------------------------------------------------------
//
// Clear out memory and free bodies
void gam_clearLifts()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &liftItr: lifts)
	{
		if (liftItr.shape != nullptr)
		{
			cpSpaceRemoveShape (sys_returnPhysicsWorld (), liftItr.shape);
			cpShapeFree (liftItr.shape);
			liftItr.shape = nullptr;
		}
		if (liftItr.body != nullptr)
		{
			liftItr.body = nullptr;
		}
	}
	lifts.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a lift sensor
void gam_createLiftSensor(int whichLift)
//----------------------------------------------------------------------------------------------------------------------
{
	lifts[whichLift].body = cpBodyNewStatic (); // Need to add body to world??

	cpBodySetPosition (lifts[whichLift].body, lifts[whichLift].worldPosition);

	lifts[whichLift].shape = cpBoxShapeNew (lifts[whichLift].body, lifts[whichLift].width, lifts[whichLift].height, 1.0f);  // Check radis
	cpSpaceAddShape (sys_returnPhysicsWorld (), lifts[whichLift].shape);
	cpShapeSetCollisionType (lifts[whichLift].shape, PHYSIC_TYPE_LIFT);
	cpShapeSetSensor (lifts[whichLift].shape, cpTrue);

	lifts[whichLift].userData            = std::make_shared<_userData> ();
	lifts[whichLift].userData->userType  = cpShapeGetCollisionType (lifts[whichLift].shape);
	lifts[whichLift].userData->dataValue = whichLift;
	cpShapeSetUserData (lifts[whichLift].shape, lifts[whichLift].userData.get ());
}

//---------------------------------------------------------
//
// Get the positions of lifts
void gam_findLiftPositions(const std::string &levelName)
//---------------------------------------------------------
{
	__tileSensor tempLift;

	int countX      = 0;
	int countY      = 0;
	int currentTile = 0;
	int countLift   = 0;

	countLift = 0;
	countX    = 0;
	countY    = 0;

	if (!lifts.empty ())
		gam_clearLifts ();

	for (int index = 0; index < g_shipDeckItr->second.levelDimensions.x * g_shipDeckItr->second.levelDimensions.y; index++)
	{
		currentTile = g_shipDeckItr->second.tiles[((countY * (g_shipDeckItr->second.levelDimensions.x)) + countX)];

		if (LIFT_TILE == currentTile)
		{
			tempLift.worldPosition.x = (countX * tileSize) + (tileSize * 0.5f);
			tempLift.worldPosition.y = (countY * tileSize) + (tileSize * 0.5f);

			tempLift.width  = tileSize / 8;
			tempLift.height = tileSize / 8;

			lifts.push_back (tempLift);

			gam_createLiftSensor (lifts.size () - 1);

			countLift++;
		}

		countX++;

		if (countX == shipdecks.at (levelName).levelDimensions.x)
		{
			countX = 0;
			countY++;
		}
	}
}

// ----------------------------------------------------------------------------
//
// Position the player on the requested lift on the new level
cpVect gam_getLiftWorldPosition(int whichLift)
// ----------------------------------------------------------------------------
{
	int    whichTile, countY, countX, liftCounter;
	int    tilePosX, tilePosY;
	double pixelX, pixelY;

	cpVect returnPosition{};

	liftCounter = 0;
	for (countY = 0; countY != g_shipDeckItr->second.levelDimensions.y; countY++)
	{
		for (countX = 0; countX != g_shipDeckItr->second.levelDimensions.x; countX++)
		{
			whichTile = g_shipDeckItr->second.tiles[(countY * g_shipDeckItr->second.levelDimensions.x) + countX];

			if (LIFT_TILE == whichTile)
			{
				if (liftCounter == whichLift)
				{
					tilePosX = countX * tileSize;
					tilePosY = countY * tileSize;
					tilePosY += tileSize;

					pixelX = static_cast<double>(tileSize) * 0.5;
					pixelY = static_cast<double>(-tileSize) * 0.5;

					returnPosition.x = static_cast<float>(tilePosX + pixelX);
					returnPosition.y = static_cast<float>(tilePosY + pixelY);

					return returnPosition;
				}
				else
				{
					liftCounter++;
				}
			}
		}
	}
	sys_shutdownWithError (sys_getString ("Unable to find lift tile on level [ %s ]", g_shipDeckItr->second.levelName));
	return returnPosition;
}

//---------------------------------------------------------
//
// Setup lifts
void gam_setupLifts()
//---------------------------------------------------------
{
	shipdecks.at (gam_returnLevelNameFromDeck (0)).liftClass.createTunnel (1, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (1)).liftClass.createTunnel (4, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (2)).liftClass.createTunnel (5, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (3)).liftClass.createTunnel (0, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (4)).liftClass.createTunnel (0, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (5)).liftClass.createTunnel (1, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (5)).liftClass.createTunnel (4, 1);
	shipdecks.at (gam_returnLevelNameFromDeck (6)).liftClass.createTunnel (1, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (6)).liftClass.createTunnel (4, 1);
	shipdecks.at (gam_returnLevelNameFromDeck (8)).liftClass.createTunnel (5, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (9)).liftClass.createTunnel (5, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (9)).liftClass.createTunnel (6, 1);
	shipdecks.at (gam_returnLevelNameFromDeck (10)).liftClass.createTunnel (6, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (11)).liftClass.createTunnel (6, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (12)).liftClass.createTunnel (3, 1);
	shipdecks.at (gam_returnLevelNameFromDeck (12)).liftClass.createTunnel (4, 2);
	shipdecks.at (gam_returnLevelNameFromDeck (12)).liftClass.createTunnel (6, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (13)).liftClass.createTunnel (3, 1);
	shipdecks.at (gam_returnLevelNameFromDeck (13)).liftClass.createTunnel (6, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (14)).liftClass.createTunnel (3, 1);
	shipdecks.at (gam_returnLevelNameFromDeck (14)).liftClass.createTunnel (6, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (14)).liftClass.createTunnel (7, 2);
	shipdecks.at (gam_returnLevelNameFromDeck (15)).liftClass.createTunnel (2, 1);
	shipdecks.at (gam_returnLevelNameFromDeck (15)).liftClass.createTunnel (0, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (16)).liftClass.createTunnel (0, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (17)).liftClass.createTunnel (2, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (17)).liftClass.createTunnel (3, 1);
	shipdecks.at (gam_returnLevelNameFromDeck (18)).liftClass.createTunnel (7, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (19)).liftClass.createTunnel (0, 0);
	shipdecks.at (gam_returnLevelNameFromDeck (20)).liftClass.createTunnel (3, 0);

	gam_setupTunnelLinks ();
}

//----------------------------------------------------------------------------
//
// Activate the lift - called from action down event on keyboard
void gam_performLiftAction()
//----------------------------------------------------------------------------
{
	currentTunnel = shipdecks.at (gam_getCurrentDeckName ()).liftClass.getTunnelIndex (playerDroid.getLiftIndex ());
	gam_stopAlertLevelSound (gam_getCurrentAlertLevel ());
	gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP, true, 0, 127, "lowEnergy");

	sys_setNewMode (MODE_GUI_LIFTVIEW, true);
}