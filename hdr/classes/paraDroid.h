#ifndef PARA_PARADROID_H
#define PARA_PARADROID_H

#include <box2d/b2_body.h>
#include <box2d/box2d.h>
#include "system/physics.h"
#include "classes/paraAI2.h"

extern int   collisionLimit;
extern float collisionCount;

class paraDroid
{
public:

	void ReleaseRef ();

	void AddRef ();

	int getCurrentMode ();

	void setCurrentMode (int newCurrentMode);

	int getDroidType ();

	void setDroidType (int newDroidType);

	int getWeaponCanFire ();

	void setWeaponCanFire (int newWeaponCanFire);

	int getCurrentHealth ();

	void setCurrentHealth (int newCurrentHealth);

	int getOverHealingTile ();

	void setOverHealingTile (int newOverHealingTile);

	int getOverTerminalTile ();

	void setOverTerminalTile (int newOverTerminalTile);

	void setIndex (int newIndex);

	int getIndex ();

	void setLiftIndex (int newLiftIndex);

	int getLiftIndex ();

	void setWeaponDelay (float newWeaponDelay);

	float getWeaponDelay ();

	void setInfluenceTimeLeft (float newInfluenceTimeLeft);

	float getInfluenceTimeLeft ();

	void setInfluenceFade (float newInfluenceFade);

	float getInfluenceFade ();

	void setLowInfluenceTimeLeft (float newlowInfluenceTimeLeft);

	bool getLowInfluenceTimeLeft ();

	void setInTransferMode (float newInTransferMode);

	bool getInTransferMode ();

	std::string getDroidName ();

	void setDroidName (std::string newDroidName);

	b2Vec2 getVelocity ();

	void setVelocity (b2Vec2 newVelocity);

	bool getOverLiftTile ();

	void setOverLiftTile (int newOverLiftTile);

	b2BodyDef *getBodyDef ();

	void setBodyDefType (b2BodyType newBodyType);


	void setBodyPosition (b2Vec2 newBodyPosition);

	void setBodyAngle (float newBodyAngle);

	void setShapeRadius (float newShapeRadius);

	void setShapePosition (b2Vec2 newShapePosition);

	void setFixtureDefShape (b2Shape *newShape);

	void setFixtureDefDensity (float newDensity);

	void setFixtureDefFriction (float newFriction);

	void setFixtureDefFilterCategoryBits (uint16 newFilterBits);

	void setFixtureDefMaskBits (uint16 newMaskBits);

	void setFixtureRestitution (float newRestitution);

	b2FixtureDef *getFixtureDef ();

	b2Shape *getShape ();

	void setWorldPosInPixels (b2Vec2 newWorldPosInPixels);

	b2Vec2 getWorldPosInPixels ();

	// ----------------- Vars Public ----------------------

	bool visibleToPlayer {true};
	bool influenceFadeFlag {false};

	int visibleValue {255};
	int visibleState {VISIBLE_STATE_IS_VISIBLE};
	int transferTargetDroidIndex {};
	int transferTargetDroidType {};

	b2Body    *body {nullptr};
	_userData *userData {nullptr};

	b2Vec2 previousWorldPosInPixels {};

	paraSprite sprite;
	paraAI2    ai2;
	//
	// Enemy droid fields
	int        collisionCounterDroid {0};
	int        collisionCounterPlayer {0};
	float      collisionCounterDelayDroid {0.0};
	float      collisionCounterDelayPlayer {0.0};

private:
	bool          lowInfluenceTimeleft {false};
	bool          weaponCanFire {true};
	bool          overHealingTile {false};
	bool          overTerminalTile {false};
	bool          inTransferMode {false};
	bool          overLiftTile {false};
	int           currentMode {};                // What is the droid doing; transfer, healing, terminal etc
	int           droidType {};
	int           currentHealth {};
	int           index {};                      // Used in physics callback
	int           liftIndex {-1};
	float         weaponDelay {};
	float         influenceTimeLeft {};
	float         influenceFade {};
	std::string   droidName {};
	b2Vec2        velocity {};
	b2Vec2        worldPosInPixels {};
	b2BodyDef     bodyDef;                      // Used for physics and collisions
	b2CircleShape shape;
	b2FixtureDef  fixtureDef;
};

#endif //PARA_PARADROID_H
