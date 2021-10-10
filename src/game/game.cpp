#include <gui/guiHighScore.h>
#include <game/score.h>
#include <game/player.h>
#include <game/audio.h>
#include <system/util.h>
#include <game/particles.h>
#include <system/gameEvents.h>
#include <game/alertLevel.h>
#include <ctime>
#include <classes/paraRandom.h>
#include <game/pathFind.h>
#include "game/shipDecks.h"
#include "game/game.h"

int explosionDamage;

//-------------------------------------------------------------------------------------------------------------
//
// Start on a random deck - lift 0
std::string gam_returnStartingDeck ()
//-------------------------------------------------------------------------------------------------------------
{
	std::vector<std::string> randomDeckNames;
	paraRandom               randDeckName;

	int randomIndex;

	randomDeckNames.emplace_back ("Stores");
	randomDeckNames.emplace_back ("Staterooms");
	randomDeckNames.emplace_back ("Repairs");
	randomDeckNames.emplace_back ("Quarters");
	randomDeckNames.emplace_back ("Research");

	randomIndex = randDeckName.get (0, static_cast<int>(randomDeckNames.size () - 1));
	return randomDeckNames[randomIndex];

}

//-------------------------------------------------------------------------------------------------------------
//
// Start a new game
void gam_startNewGame ()
//-------------------------------------------------------------------------------------------------------------
{
	renderer.clearTextures ();
	gam_resetDroids ();
	playerDroid.setCurrentMode (DROID_MODE_NORMAL);
	gam_setupPlayerDroid ();
	gam_initScoreValues ();
	gam_setAlertLevel (ALERT_GREEN_TILE);

	backgroundStarfield.init (renderer, 40, 7, 50, windowHeight, windowWidth);

	gam_changeToDeck (gam_returnStartingDeck (), 0);
}

//-------------------------------------------------------------------------------------------------------------
//
// Process game over event
void gam_processGameOver ()
//-------------------------------------------------------------------------------------------------------------
{
	if (playerDroid.sprite.animate ())      // Has the animation finished playing
	{
		playerDroid.setCurrentMode (DROID_MODE_DEAD);
		sys_setNewMode (MODE_END_PRE_LOST_SCREEN, false);
		gam_addAudioEvent (EVENT_ACTION_AUDIO_STOP_ALL, false, 0, 0, "");
	}

	gui.restartScrollBox ("introScreen.scrollbox");
	gui.restartScrollBox ("wonScreen.scrollbox");
}