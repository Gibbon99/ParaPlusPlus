#include <gui/guiHighScore.h>
#include <game/score.h>
#include "game/shipDecks.h"
#include "game/game.h"

int explosionDamage;

//-------------------------------------------------------------------------------------------------------------
//
// Start a new game
void gam_startNewGame()
//-------------------------------------------------------------------------------------------------------------
{
	sys_setupEnemyPhysics (gam_getCurrentDeckName());
}

//-------------------------------------------------------------------------------------------------------------
//
// Process game over event
void gam_processGameOver()
//-------------------------------------------------------------------------------------------------------------
{
	gui_insertNewScore(gam_getCurrentScore());

	std::cout << " Game Over " << std::endl;
}