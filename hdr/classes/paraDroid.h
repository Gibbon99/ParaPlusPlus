#pragma once

#include <cpVect.h>
#include <chipmunk.h>
#include "system/cpPhysics.h"
#include "classes/paraAI2.h"

extern int   collisionLimit;
extern float collisionCount;

class paraDroid
{
public:

	void ReleaseRef();

	void AddRef();

	int getCurrentMode();

	void setCurrentMode(int newCurrentMode);

	int getDroidType();

	void setDroidType(int newDroidType);

	int getWeaponCanFire();

	void setWeaponCanFire(int newWeaponCanFire);

	int getCurrentHealth();

	void setCurrentHealth(int newCurrentHealth);

	int getOverHealingTile();

	void setOverHealingTile(bool newOverHealingTile);

	int getOverTerminalTile();

	void setOverTerminalTile(int newOverTerminalTile);

	void setIndex(int newIndex);

	int getIndex();

	void setLiftIndex(int newLiftIndex);

	int getLiftIndex();

	void setWeaponDelay(float newWeaponDelay);

	float getWeaponDelay();

	void setInfluenceTimeLeft(float newInfluenceTimeLeft);

	float getInfluenceTimeLeft();

	void setInfluenceFade(float newInfluenceFade);

	float getInfluenceFade();

	void setLowInfluenceTimeLeft(float newlowInfluenceTimeLeft);

	bool getLowInfluenceTimeLeft();

	void setInTransferMode(float newInTransferMode);

	bool getInTransferMode();

	std::string getDroidName();

	void setDroidName(std::string newDroidName);

	cpVect getVelocity();

	void setVelocity(cpVect newVelocity);

	bool getOverLiftTile();

	void setOverLiftTile(int newOverLiftTile);

	void setWorldPosInPixels(cpVect newWorldPosInPixels);

	cpVect getWorldPosInPixels();

	// ----------------- Vars Public ----------------------

	bool visibleToPlayer {true};
	bool influenceFadeFlag {false};

	int visibleValue {255};
	int visibleState {VISIBLE_STATE_IS_VISIBLE};
	int transferTargetDroidIndex {};
	int transferTargetDroidType {};

	cpBody                *body {nullptr};
	cpShape               *shape {nullptr};
	shared_ptr<_userData> userData {};

	paraSprite sprite;
	paraAI2    ai2;
	//
	// Enemy droid fields


private:
	bool        lowInfluenceTimeleft {false};
	bool        weaponCanFire {true};
	bool        overHealingTile {false};
	bool        overTerminalTile {false};
	bool        inTransferMode {false};
	bool        overLiftTile {false};
	int         currentMode {};                // What is the droid doing; transfer, healing, terminal etc
	int         droidType {};
	int         currentHealth {};
	int         index {};                      // Used in physics callback
	int         liftIndex {-1};
	float       weaponDelay {};
	float       influenceTimeLeft {};
	float       influenceFade {};
	std::string droidName {};
};