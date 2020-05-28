#include "game/shipDecks.h"
#include "game/game.h"

//-------------------------------------------------------------------------------------------------------------
//
// Start a new game
void gam_startNewGame()
//-------------------------------------------------------------------------------------------------------------
{
	sys_setupEnemyPhysics (gam_getCurrentDeckName());
}