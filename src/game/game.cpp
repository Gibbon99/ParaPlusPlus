#include <game/shipDecks.h>
#include "game/game.h"

//-------------------------------------------------------------------------------------------------------------
//
// Clear previous level - remove physics objects from droid before changing level name
void gam_clearDroidPhysics(std::string levelName)
//-------------------------------------------------------------------------------------------------------------
{
	if (levelName.empty())      // First run - no level set as yet
		return;

	for (auto &droidItr : shipdecks.at(levelName).droid)
	{
		if (droidItr.userData != nullptr)
			delete (droidItr.userData);

		if (droidItr.body != nullptr)
			sys_getPhysicsWorld ()->DestroyBody (droidItr.body);
	}
}

//-------------------------------------------------------------------------------------------------------------
//
// Start a new game
void gam_startNewGame()
//-------------------------------------------------------------------------------------------------------------
{
	for (auto &deckItr : shipdecks)
	{
		gam_initDroids(deckItr.first);
	}

	sys_setupEnemyPhysics (gam_getCurrentDeckName());
}