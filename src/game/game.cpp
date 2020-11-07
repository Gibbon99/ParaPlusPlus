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
	sys_setupEnemyPhysics (gam_getCurrentDeckName());
	playerDroid.currentMode = DROID_MODE_NORMAL;
	gam_setupPlayerDroid ();
	gam_resetDroids ();
}

//-------------------------------------------------------------------------------------------------------------
//
// Process game over event
void gam_processGameOver()
//-------------------------------------------------------------------------------------------------------------
{
	bool animationEnd = false;

	if ((playerDroid.currentMode != DROID_MODE_EXPLODING)
		&& (playerDroid.currentMode != DROID_MODE_DEAD))
	{
		gui_insertNewScore (gam_getCurrentScore ());

		playerDroid.velocity    = {0, 0};
		playerDroid.currentMode = DROID_MODE_EXPLODING;
		playerDroid.sprite.create ("explosion", 25, explosionAnimationSpeed);
		playerDroid.sprite.setAnimateSpeed (explosionAnimationSpeed);      // Set for explosion animation

		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "explode1");
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 1, 127, "explode2");

		gam_addEmitter (sys_convertToMeters (playerDroid.worldPosInPixels), PARTICLE_TYPE_EXPLOSION, 0);

		sys_setNewMode(MODE_GAME_OVER, false);
		return;
	}

	if (playerDroid.currentMode == DROID_MODE_EXPLODING)
		animationEnd = playerDroid.sprite.animate ();

	std::cout << " Player is exploding " << animationEnd << std::endl;

//	if ((animationEnd) && (playerDroid.currentMode == DROID_MODE_EXPLODING))
	if (animationEnd)
	{
		playerDroid.currentMode = DROID_MODE_DEAD;
//		gam_addEvent (MODE_END_PRE_LOST_SCREEN, 50, "");
		sys_setNewMode (MODE_END_PRE_LOST_SCREEN, false);
		std::cout << " Game Over " << std::endl;
	}
}