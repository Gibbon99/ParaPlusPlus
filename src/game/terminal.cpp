#include "game/shipDecks.h"
#include "game/lifts.h"
#include "game/terminal.h"

std::vector<__tileSensor> terminals;

//----------------------------------------------------------------------------------------------------------------------
//
// Create a terminal sensor
void gam_createTerminalSensor(int whichTerminal)
//----------------------------------------------------------------------------------------------------------------------
{
	terminals[whichTerminal].body = cpBodyNewStatic ();
	cpBodySetPosition (terminals[whichTerminal].body, terminals[whichTerminal].worldPosition);

	terminals[whichTerminal].shape = cpBoxShapeNew (terminals[whichTerminal].body, terminals[whichTerminal].width, terminals[whichTerminal].height, 1.0);   // Check Radius
	cpSpaceAddBody (sys_returnPhysicsWorld(), terminals[whichTerminal].body);
	cpSpaceAddShape (sys_returnPhysicsWorld (), terminals[whichTerminal].shape);
	cpShapeSetCollisionType (terminals[whichTerminal].shape, PHYSIC_TYPE_TERMINAL);
	cpShapeSetSensor (terminals[whichTerminal].shape, cpTrue);

	terminals[whichTerminal].userData            = std::make_shared<userData_> ();
	terminals[whichTerminal].userData->userType  = cpShapeGetCollisionType (terminals[whichTerminal].shape);
	terminals[whichTerminal].userData->dataValue = whichTerminal;
	cpShapeSetUserData (terminals[whichTerminal].shape, terminals[whichTerminal].userData.get ());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Clear out memory and free bodies
void gam_clearTerminals()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &terminalItr: terminals)
	{
		if (terminalItr.shape != nullptr)
		{
			if (cpSpaceContainsShape (sys_returnPhysicsWorld(), terminalItr.shape))
				cpSpaceRemoveShape (sys_returnPhysicsWorld (), terminalItr.shape);

			cpShapeFree(terminalItr.shape);
			terminalItr.shape = nullptr;
		}
		if (terminalItr.body != nullptr)
		{
			if (cpSpaceContainsBody (sys_returnPhysicsWorld(), terminalItr.body))
				cpSpaceRemoveBody (sys_returnPhysicsWorld(), terminalItr.body);

			cpBodyFree(terminalItr.body);
			terminalItr.body = nullptr;
		}
	}
	terminals.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the positions of terminals on the current level
void gam_findTerminalPositions(const std::string &levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	__tileSensor tempTerminal {};

	int countX {0};
	int countY {0};
	int currentTile {0};
	int countTerminal {0};

	if (!terminals.empty ())
	{
		gam_clearTerminals ();
	}

	for (int index = 0; index < shipdecks.at (levelName).levelDimensions.x * shipdecks.at (levelName).levelDimensions.y; index++)
	{
		currentTile = shipdecks.at (levelName).tiles[((countY * (shipdecks.at (levelName).levelDimensions.x)) + countX)];

		if ((TERMINAL_BOTTOM == currentTile) || (TERMINAL_LEFT == currentTile) || (TERMINAL_RIGHT == currentTile) || (TERMINAL_TOP == currentTile))
		{
			tempTerminal.worldPosition.x = (countX * tileSize) + (tileSize * 0.5f);
			tempTerminal.worldPosition.y = (countY * tileSize) + (tileSize * 0.5f);

			tempTerminal.width  = tileSize / 8;
			tempTerminal.height = tileSize / 8;

			terminals.push_back (tempTerminal);

			gam_createTerminalSensor (terminals.size () - 1);

			countTerminal++;
		}

		countX++;

		if (countX == shipdecks.at (levelName).levelDimensions.x)
		{
			countX = 0;
			countY++;
		}
	}
}
