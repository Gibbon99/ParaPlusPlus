#include <game/shipDecks.h>
#include <game/lifts.h>
#include "game/terminal.h"

std::vector<__liftSensor> terminals;

//----------------------------------------------------------------------------------------------------------------------
//
// Create a terminal sensor
void gam_createTerminalSensor (unsigned long whichTerminal, int index)
//----------------------------------------------------------------------------------------------------------------------
{
	terminals[whichTerminal].bodyDef.type = b2_staticBody;
	terminals[whichTerminal].bodyDef.position.Set (terminals[whichTerminal].worldPosition.x / pixelsPerMeter, terminals[whichTerminal].worldPosition.y / pixelsPerMeter);
	terminals[whichTerminal].body = sys_getPhysicsWorld ()->CreateBody (&terminals[whichTerminal].bodyDef);

	terminals[whichTerminal].userData            = new _userData;
	terminals[whichTerminal].userData->userType  = PHYSIC_TYPE_TERMINAL;
	terminals[whichTerminal].userData->dataValue = (int) index;
	terminals[whichTerminal].body->SetUserData (terminals[whichTerminal].userData);

	terminals[whichTerminal].shape.SetAsBox ((terminals[whichTerminal].height) / pixelsPerMeter, (terminals[whichTerminal].width) / pixelsPerMeter);
	terminals[whichTerminal].fixtureDef.shape    = &terminals[whichTerminal].shape;
	terminals[whichTerminal].fixtureDef.isSensor = true;
	terminals[whichTerminal].body->CreateFixture (&terminals[whichTerminal].fixtureDef);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Clear out memory and free bodies
void gam_clearTerminals ()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto &terminalItr : terminals)
	{
		if (terminalItr.userData != nullptr)
			delete (terminalItr.userData);
		if (terminalItr.body != nullptr)
			sys_getPhysicsWorld ()->DestroyBody (terminalItr.body);
	}
	terminals.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the positions of terminals on the current level
void gam_findTerminalPositions (const std::string &levelName)
//----------------------------------------------------------------------------------------------------------------------
{
	__liftSensor tempTerminal;

	int countX        = 0;
	int countY        = 0;
	int currentTile   = 0;
	int countTerminal = 0;

	countTerminal = 0;
	countX        = 0;
	countY        = 0;

	if (!terminals.empty ())
	{
		gam_clearTerminals ();
	}

	for (int index = 0; index < shipdecks.at (levelName).levelDimensions.x * shipdecks.at (levelName).levelDimensions.y; index++)
	{
		currentTile = shipdecks.at (levelName).tiles[((countY * (shipdecks.at (levelName).levelDimensions.x)) + countX)];

		if ((TERMINAL_BOTTOM == currentTile) ||
		    (TERMINAL_LEFT == currentTile) ||
		    (TERMINAL_RIGHT == currentTile) ||
		    (TERMINAL_TOP == currentTile))
		{
			tempTerminal.worldPosition.x = (countX * tileSize) + (tileSize * 0.5f);
			tempTerminal.worldPosition.y = (countY * tileSize) + (tileSize * 0.5f);

			tempTerminal.width  = tileSize / 8;
			tempTerminal.height = tileSize / 8;

			terminals.push_back (tempTerminal);

			gam_createTerminalSensor (terminals.size () - 1, countTerminal);

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

//----------------------------------------------------------------------------------------------------------------------
//
// Pressed the action key while over a terminal tile
void gam_performTerminalAction ()
//----------------------------------------------------------------------------------------------------------------------
{
/*
	evt_pushEvent (0, PARA_EVENT_AUDIO, GAME_EVENT_STOP_AUDIO, 0, 0, "greenAlert"); // TODO: Change to current alert
	gui_changeToGUIScreen (gui_findIndex (GUI_OBJECT_SCREEN, "mainTerminalScreen"));
	sys_changeMode (MODE_GUI_TERMINAL, true);
	gui_hostSetObjectFocus ("terminalLogoffButton");
	hud_setText (false, "hudTerminal");
	*/
}