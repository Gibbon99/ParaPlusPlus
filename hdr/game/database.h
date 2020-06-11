#pragma once

#include "main.h"

typedef struct
{
	int         maxHealth;
	double      maxSpeed;
	double      accelerate;
	int         score;
	int         bounceDamage;
	bool        canShoot;
	int         bulletType;
	int         chanceToShoot;
	int         bulletDamage;
	bool        disrupterImmune;
	float       rechargeTime;
	int         tokenCount;
	int         imageIndex;
	std::string dbImageFileName;
	std::string height;
	std::string weight;

	std::string description;
	std::string className;
	std::string drive;
	std::string brain;
	std::string weapon;
	std::string sensor1;
	std::string sensor2;
	std::string sensor3;
	std::string notes;
} _dataBaseEntry;

extern std::vector<_dataBaseEntry> dataBaseEntry;

// Get and build up information for droid database files
void gam_getDBInformation ();

// Change to the previous droid information
void gam_previousDatabase();

// Change to the next droid information
void gam_nextDatabase();

// Setup the database screen
void gam_prepareDatabaseScreen(int whichDroidIndex);