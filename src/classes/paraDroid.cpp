#include "classes/paraDroid.h"

#include <utility>

int   collisionLimit;
float collisionCount;

void paraDroid::AddRef()
{

}

void paraDroid::ReleaseRef()
{

}

//-------------------------------------------------------------------------------------------------------------
//
// Return the name of a sprite based on its type
int paraDroid::getCurrentMode()
//-------------------------------------------------------------------------------------------------------------
{
	return currentMode;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return the name of a sprite based on its type
void paraDroid::setCurrentMode(int newCurrentMode)
//-------------------------------------------------------------------------------------------------------------
{
	currentMode = newCurrentMode;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return the name of a sprite based on its type
int paraDroid::getDroidType()
//-------------------------------------------------------------------------------------------------------------
{
	return droidType;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return the name of a sprite based on its type
void paraDroid::setDroidType(int newDroidType)
//-------------------------------------------------------------------------------------------------------------
{
	droidType = newDroidType;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return if the droid can fire
int paraDroid::getWeaponCanFire()
//-------------------------------------------------------------------------------------------------------------
{
	return weaponCanFire;
}

//-------------------------------------------------------------------------------------------------------------
//
// Set the weapon can fire state
void paraDroid::setWeaponCanFire(int newWeaponCanFire)
//-------------------------------------------------------------------------------------------------------------
{
	weaponCanFire = newWeaponCanFire;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return the droid current health value
int paraDroid::getCurrentHealth()
//-------------------------------------------------------------------------------------------------------------
{
	return currentHealth;
}

//-------------------------------------------------------------------------------------------------------------
//
// Set the new health value
void paraDroid::setCurrentHealth(int newCurrentHealth)
//-------------------------------------------------------------------------------------------------------------
{
	currentHealth = newCurrentHealth;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return if the droid is over a healing tile flag
int paraDroid::getOverHealingTile()
//-------------------------------------------------------------------------------------------------------------
{
	return overHealingTile;
}

//-------------------------------------------------------------------------------------------------------------
//
// Set the over a healing tile flag
void paraDroid::setOverHealingTile(bool newOverHealingTile)
//-------------------------------------------------------------------------------------------------------------
{
	overHealingTile = newOverHealingTile;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return if the droid is over a terminal tile flag
int paraDroid::getOverTerminalTile()
//-------------------------------------------------------------------------------------------------------------
{
	return overTerminalTile;
}

//-------------------------------------------------------------------------------------------------------------
//
// Set the over a terminal tile flag
void paraDroid::setOverTerminalTile(int newOverTerminalTile)
//-------------------------------------------------------------------------------------------------------------
{
	overTerminalTile = newOverTerminalTile;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return droid index ( ID )
int paraDroid::getIndex()
//-------------------------------------------------------------------------------------------------------------
{
	return index;
}

//-------------------------------------------------------------------------------------------------------------
//
// Set the droid index value
void paraDroid::setIndex(int newIndex)
//-------------------------------------------------------------------------------------------------------------
{
	index = newIndex;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return droid Liftindex ( ID )
int paraDroid::getLiftIndex()
//-------------------------------------------------------------------------------------------------------------
{
	return liftIndex;
}

//-------------------------------------------------------------------------------------------------------------
//
// Set the droid Liftindex value
void paraDroid::setLiftIndex(int newLiftIndex)
//-------------------------------------------------------------------------------------------------------------
{
	liftIndex = newLiftIndex;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return droid weapon delay value
float paraDroid::getWeaponDelay()
//-------------------------------------------------------------------------------------------------------------
{
	return weaponDelay;
}

//-------------------------------------------------------------------------------------------------------------
//
// Set the droid Liftindex value
void paraDroid::setWeaponDelay(float newWeaponDelay)
//-------------------------------------------------------------------------------------------------------------
{
	weaponDelay = newWeaponDelay;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return droid weapon delay value
float paraDroid::getInfluenceTimeLeft()
//-------------------------------------------------------------------------------------------------------------
{
	return influenceTimeLeft;
}

//-------------------------------------------------------------------------------------------------------------
//
// Set the droid Liftindex value
void paraDroid::setInfluenceTimeLeft(float newInfluenceTimeLeft)
//-------------------------------------------------------------------------------------------------------------
{
	influenceTimeLeft = newInfluenceTimeLeft;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return droid influence fade value
float paraDroid::getInfluenceFade()
//-------------------------------------------------------------------------------------------------------------
{
	return influenceFade;
}

//-------------------------------------------------------------------------------------------------------------
//
// Set the droid influence fade
void paraDroid::setInfluenceFade(float newInfluenceFade)
//-------------------------------------------------------------------------------------------------------------
{
	influenceFade = newInfluenceFade;
}

//-------------------------------------------------------------------------------------------------------------
//
// Set influence time left flag
void paraDroid::setLowInfluenceTimeLeft(float newLowInfluenceTimeLeft)
//-------------------------------------------------------------------------------------------------------------
{
	lowInfluenceTimeleft = newLowInfluenceTimeLeft;
}

//-------------------------------------------------------------------------------------------------------------
//
// Get low influence time left
bool paraDroid::getLowInfluenceTimeLeft()
//-------------------------------------------------------------------------------------------------------------
{
	return lowInfluenceTimeleft;
}

//-------------------------------------------------------------------------------------------------------------
//
// Set the flag to indicate player is in transfer mode
void paraDroid::setInTransferMode(float newInTransferMode)
//-------------------------------------------------------------------------------------------------------------
{
	inTransferMode = newInTransferMode;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return if the player is in transfer mode
bool paraDroid::getInTransferMode()
//-------------------------------------------------------------------------------------------------------------
{
	return inTransferMode;
}

//-------------------------------------------------------------------------------------------------------------
//
// Return the droids text name
std::string paraDroid::getDroidName()
{
	return droidName;
}

//-------------------------------------------------------------------------------------------------------------
//
// Set the text name for the droid
void paraDroid::setDroidName(std::string newDroidName)
//-------------------------------------------------------------------------------------------------------------
{
	droidName = std::move (newDroidName);
}

//-------------------------------------------------------------------------------------------------------------
//
// Return current velocity value
cpVect paraDroid::getVelocity()
//-------------------------------------------------------------------------------------------------------------
{
	return this->ai2.getVelocity ();
}

//-------------------------------------------------------------------------------------------------------------
//
// Set the current velocity value
void paraDroid::setVelocity(cpVect newVelocity)
//-------------------------------------------------------------------------------------------------------------
{
	this->ai2.setVelocity (newVelocity);
}

//-------------------------------------------------------------------------------------------------------------
//
// Return if the player is over a lift
bool paraDroid::getOverLiftTile()
//-------------------------------------------------------------------------------------------------------------
{
	return overLiftTile;
}

//-------------------------------------------------------------------------------------------------------------
//
// Set flag to indicate player is over a lift tile
void paraDroid::setOverLiftTile(int newOverLiftTile)
//-------------------------------------------------------------------------------------------------------------
{
	overLiftTile = newOverLiftTile;
}

//-------------------------------------------------------------------------------------------------------------
//
// Work with world position at the droid class level
void paraDroid::setWorldPosInPixels(cpVect newWorldPosInPixels)
//-------------------------------------------------------------------------------------------------------------
{
	this->ai2.setWorldPosInPixels (newWorldPosInPixels);
}

//-------------------------------------------------------------------------------------------------------------
//
// Work with world position at the droid class level
cpVect paraDroid::getWorldPosInPixels()
//-------------------------------------------------------------------------------------------------------------
{
	return this->ai2.getWorldPosInPixels ();
}

