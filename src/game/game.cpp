#include <gui/guiHighScore.h>
#include <game/score.h>
#include <game/player.h>
#include <game/audio.h>
#include <system/util.h>
#include <game/particles.h>
#include <system/gameEvents.h>
#include "game/shipDecks.h"
#include "game/game.h"

int explosionDamage;

//-------------------------------------------------------------------------------------------------------------
//
// Start a new game
void gam_startNewGame()
//-------------------------------------------------------------------------------------------------------------
{
	//
	// TODO: Clear screen back buffer

	gam_resetDroids ();
	sys_setupEnemyPhysics (gam_getCurrentDeckName());
	playerDroid.currentMode = DROID_MODE_NORMAL;
	gam_setupPlayerDroid ();
	gam_initScoreValues();
}

//-------------------------------------------------------------------------------------------------------------
//
// Process game over event
void gam_processGameOver()
//-------------------------------------------------------------------------------------------------------------
{
	bool animationEnd = false;

	animationEnd = playerDroid.sprite.animate ();

	std::cout << " Player is exploding " << animationEnd << " current mode : " << playerDroid.currentMode << std::endl;

	if (animationEnd)
	{

		std::cout << "Player explosion animation is over" << std::endl;

		playerDroid.currentMode = DROID_MODE_DEAD;
		sys_setNewMode (MODE_END_PRE_LOST_SCREEN, false);

		std::cout << " Game Over " << std::endl;

	}
}