#include <gui/guiHighScore.h>
#include <game/score.h>
#include <game/player.h>
#include <game/audio.h>
#include <system/util.h>
#include <game/particles.h>
#include <system/gameEvents.h>
#include <game/alertLevel.h>
#include "game/shipDecks.h"
#include "game/game.h"

int explosionDamage;

//-------------------------------------------------------------------------------------------------------------
//
// Start a new game
void gam_startNewGame()
//-------------------------------------------------------------------------------------------------------------
{
	renderer.clearTextures();
	gam_resetDroids ();
	playerDroid.currentMode = DROID_MODE_NORMAL;
	gam_setupPlayerDroid ();
	gam_initScoreValues();
	gam_setAlertLevel (ALERT_GREEN_TILE);

	gam_changeToDeck ("Staterooms", 0);
}

//-------------------------------------------------------------------------------------------------------------
//
// Process game over event
void gam_processGameOver()
//-------------------------------------------------------------------------------------------------------------
{
	if (playerDroid.sprite.animate ())      // Has the animdation finished playing
	{
		std::cout << "Player explosion animation is over" << std::endl;

		playerDroid.currentMode = DROID_MODE_DEAD;
		sys_setNewMode (MODE_END_PRE_LOST_SCREEN, false);

		std::cout << " Game Over " << std::endl;

	}
}