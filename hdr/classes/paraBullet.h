#pragma once

#include <system/cpPhysics.h>
#include "main.h"

#define MY_PI 3.1415

class paraBullet
{
public:

	int                        type {};
	bool                       inUse {false};
	float                      angle {0.0};
	float                      disrupterFadeAmount {};
	float                      disrupterFade {};
	Uint32                     ID {};                              // Unique ID for each bullet created
	cpVect                     worldPosInPixels {};
	cpVect                     worldDestInPixels {};
	cpVect                     velocity {};
	cpBody                     *body {};
	cpShape                    *shape {};
	std::shared_ptr<userData_> userData {};

	paraSprite sprite {};
private:

};

extern float                   bulletDensity;
extern float                   bulletFriction;
extern float                   bulletAnimationSpeed;
extern float                   numDisrupterFrames;
extern float                   disrupterFadeAmount;
extern float                   bulletMoveSpeed;
extern std::vector<paraBullet> bullets;

// Add a newly created bullet to the array
void gam_addBullet(int bulletSourceIndex);

// Remove a bullet - called from Game Event loop - outside of Physics world step
void gam_removeBullet(Uint32 bulletIndex);

// Process physics and animation for any active bullets
void gam_processBullets();

// Render any visible bullets to screen
void gam_renderBullets();

// Debug bullets
void gam_debugBullets();

// Preallocate the bullet array - clear out on level change
void gam_initBulletArray();

int gam_getArrayIndex(Uint32 bulletID);        // Return the index of a bullet using its ID