#include "system/util.h"
#include "gui/guiLanguage.h"
#include "game/player.h"
#include "game/audio.h"
#include "game/texture.h"
#include "game/database.h"

std::vector<_dataBaseEntry> dataBaseEntry {};
CSimpleIniA                 databaseFile {};
int                         currentDatabaseRecord {};

//-----------------------------------------------------------------------------------------------------
//
// Load a droid database file.
bool io_getDBDroidInfo(const std::string &fileName)
//-----------------------------------------------------------------------------------------------------
{
	_dataBaseEntry tempDataBaseEntry;
	std::string    fileInMem;

	fileInMem = fileSystem.getString (fileName);

	auto returnCode = databaseFile.LoadData (fileInMem.c_str (), fileInMem.size ());
	if (returnCode < 0)
		sys_shutdownWithError (sys_getString ("Unable to open database file [ %s ].", fileName.c_str ()));

	if (dataBaseEntry.empty ())    // 001 base droid stats
	{
		tempDataBaseEntry.maxSpeed = databaseFile.GetDoubleValue ("droidInfo", "max_speed", -1.0);
		if (tempDataBaseEntry.maxSpeed == -1.0)
			sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "maxSpeed"));

		tempDataBaseEntry.maxSpeed /= baseGameSpeed;

		tempDataBaseEntry.accelerate = databaseFile.GetDoubleValue ("droidInfo", "accelerate", -1.0);
		if (tempDataBaseEntry.accelerate == -1.0)
			sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "accelerate"));

		tempDataBaseEntry.accelerate /= baseGameSpeed;

		tempDataBaseEntry.maxHealth = static_cast<int>(databaseFile.GetLongValue ("droidInfo", "max_health", 0));
		if (tempDataBaseEntry.maxHealth == 0)
			sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "max_health"));

		tempDataBaseEntry.rechargeTime = databaseFile.GetDoubleValue ("droidInfo", "rechargeTime", -1.0);
		if (tempDataBaseEntry.rechargeTime == -1.0)
			sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "rechargeTime"));
	}
	else
	{
		//
		// All other droid speeds and acceleration are based on player droid values
		// Easy to change overall speed of the game
		//
		double tempVar;

		tempVar = databaseFile.GetDoubleValue ("droidInfo", "max_speed", -1.0);
		if (tempVar == -1.0)
			sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "max_speed"));

		tempDataBaseEntry.maxSpeed = dataBaseEntry[0].maxSpeed + tempVar;

		tempVar = databaseFile.GetDoubleValue ("droidInfo", "accelerate", -1.0);
		if (tempVar == -1.0)
			sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "accelerate"));

		tempDataBaseEntry.accelerate = dataBaseEntry[0].accelerate + tempVar;

		tempDataBaseEntry.maxSpeed /= baseGameSpeed;
		tempDataBaseEntry.accelerate /= baseGameSpeed;

		tempVar = databaseFile.GetDoubleValue ("droidInfo", "rechargeTime", -1.0);
		if (tempVar == -1.0)
			sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "rechargeTime"));

		tempDataBaseEntry.rechargeTime = dataBaseEntry[0].rechargeTime + tempVar;


		tempDataBaseEntry.maxHealth = static_cast<int>(databaseFile.GetLongValue ("droidInfo", "max_health", 0));
		if (tempDataBaseEntry.maxHealth == 0)
			sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "max_health"));
		tempDataBaseEntry.maxHealth += dataBaseEntry[0].maxHealth;
	}

	tempDataBaseEntry.score = static_cast<int>(databaseFile.GetLongValue ("droidInfo", "score", -1));
	if (tempDataBaseEntry.score == -1.0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "score"));

	tempDataBaseEntry.bounceDamage = static_cast<int>(databaseFile.GetLongValue ("droidInfo", "bounce_damage", -1));
	if (tempDataBaseEntry.bounceDamage == -1.0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "bounce_damage"));

	tempDataBaseEntry.canShoot = databaseFile.GetBoolValue ("droidInfo", "can_shoot", true);

	tempDataBaseEntry.bulletType = static_cast<int>(databaseFile.GetLongValue ("droidInfo", "bullet_type", -2));
	if (tempDataBaseEntry.bulletType == -2.0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "bullet_type"));

	//
	// Map the type of bullet to the sprite image for it
	//
	// Set to -1 if can_shoot == 0
	switch (tempDataBaseEntry.bulletType)
	{
		case 0: // small double laser
			tempDataBaseEntry.bulletType = BULLET_TYPE_NORMAL;
			break;

		case 1: // Large single laser
			tempDataBaseEntry.bulletType = BULLET_TYPE_SINGLE;
			break;

		case 2: // Large double laser
			tempDataBaseEntry.bulletType = BULLET_TYPE_DOUBLE;
			break;

		case 3: // Disrupter
			tempDataBaseEntry.bulletType = BULLET_TYPE_DISRUPTER;
			break;

		default:
			tempDataBaseEntry.bulletType = -1;
			break;
	}

	tempDataBaseEntry.chanceToShoot = static_cast<int>(databaseFile.GetLongValue ("droidInfo", "chance_to_shoot", -1));
	if (tempDataBaseEntry.chanceToShoot == -1.0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "chance_to_shoot"));

	tempDataBaseEntry.bulletDamage = static_cast<int>(databaseFile.GetLongValue ("droidInfo", "bullet_damage", -1));
	if (tempDataBaseEntry.bulletDamage == -1.0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "bullet_damage"));

	tempDataBaseEntry.disrupterImmune = databaseFile.GetBoolValue ("droidInfo", "disrupter_immune", true);

	tempDataBaseEntry.tokenCount = static_cast<int>(databaseFile.GetLongValue ("droidInfo", "token_count", -1));
	if (tempDataBaseEntry.tokenCount == -1.0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "token_count"));

	auto returnValue = databaseFile.GetValue ("droidInfo", "height", "default");
	if (strcmp (returnValue, "default") == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "height"));
	tempDataBaseEntry.height = returnValue;

	returnValue = databaseFile.GetValue ("droidInfo", "weight", "default");
	if (strcmp (returnValue, "default") == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "weight"));
	tempDataBaseEntry.weight = returnValue;

	returnValue = databaseFile.GetValue ("droidInfo", "dbImageFileName", "default");
	if (strcmp (returnValue, "default") == 0)
		sys_shutdownWithError (sys_getString ("Unable to locate value [ %s ] in database file.", "dbImageFileName"));
	tempDataBaseEntry.dbImageFileName = returnValue;

	tempDataBaseEntry.notes = gui_getString (tempDataBaseEntry.dbImageFileName);

	dataBaseEntry.push_back (tempDataBaseEntry);

	return true;
}

//------------------------------------------------------------------------------------------------------------------------------------
//
// Get and build up information for droid database files
void gam_getDBInformation()
//------------------------------------------------------------------------------------------------------------------------------------
{
	io_getDBDroidInfo ("001.txt");
	io_getDBDroidInfo ("123.txt");
	io_getDBDroidInfo ("139.txt");
	io_getDBDroidInfo ("247.txt");
	io_getDBDroidInfo ("249.txt");
	io_getDBDroidInfo ("296.txt");
	io_getDBDroidInfo ("302.txt");
	io_getDBDroidInfo ("329.txt");
	io_getDBDroidInfo ("420.txt");
	io_getDBDroidInfo ("476.txt");
	io_getDBDroidInfo ("493.txt");
	io_getDBDroidInfo ("516.txt");
	io_getDBDroidInfo ("571.txt");
	io_getDBDroidInfo ("598.txt");
	io_getDBDroidInfo ("614.txt");
	io_getDBDroidInfo ("615.txt");
	io_getDBDroidInfo ("629.txt");
	io_getDBDroidInfo ("711.txt");
	io_getDBDroidInfo ("742.txt");
	io_getDBDroidInfo ("751.txt");
	io_getDBDroidInfo ("821.txt");
	io_getDBDroidInfo ("834.txt");
	io_getDBDroidInfo ("883.txt");
	io_getDBDroidInfo ("999.txt");
}

//------------------------------------------------------------------------------------------------------------------------------------
//
// Change to the previous droid information
void gam_previousDatabase()
//------------------------------------------------------------------------------------------------------------------------------------
{
	std::string newFileName;
	std::string newKeyName;

	newKeyName = "db_droid";

	if (currentDatabaseRecord > 0)
		currentDatabaseRecord--;

	newFileName = dataBaseEntry[currentDatabaseRecord].dbImageFileName + ".bmp";
	gam_loadTexture (newFileName, newKeyName);

	gui.restartScrollBox ("databaseScreen.scrollbox");
	gui.setLabel (GUI_OBJECT_SCROLLBOX, "databaseScreen.scrollbox", 10, GUI_LABEL_LEFT, dataBaseEntry[currentDatabaseRecord].notes);
}

//------------------------------------------------------------------------------------------------------------------------------------
//
// Change to the next droid information
void gam_nextDatabase()
//------------------------------------------------------------------------------------------------------------------------------------
{
	std::string newFileName;
	std::string newKeyName;

	newKeyName = "db_droid";

	if (currentDatabaseRecord < static_cast<int>(dataBaseEntry.size () - 1))
		currentDatabaseRecord++;

	if (currentDatabaseRecord - 1 == playerDroid.getDroidType ())
	{
		currentDatabaseRecord--;
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
		return;
	}

	newFileName = dataBaseEntry[currentDatabaseRecord].dbImageFileName + ".bmp";
	gam_loadTexture (newFileName, newKeyName);

	gui.restartScrollBox ("databaseScreen.scrollbox");
	gui.setLabel (GUI_OBJECT_SCROLLBOX, "databaseScreen.scrollbox", 10, GUI_LABEL_LEFT, dataBaseEntry[currentDatabaseRecord].notes);
}

//------------------------------------------------------------------------------------------------------------------------------------
//
// Setup the database screen
void gam_prepareDatabaseScreen(int whichDroidIndex)
//------------------------------------------------------------------------------------------------------------------------------------
{
	std::string newFileName;
	std::string newKeyName;

	newKeyName = "db_droid";

	currentDatabaseRecord = whichDroidIndex;

	newFileName = dataBaseEntry[currentDatabaseRecord].dbImageFileName + ".bmp";
	gam_loadTexture (newFileName, newKeyName);

	gui.setLabel (GUI_OBJECT_SCROLLBOX, "databaseScreen.scrollbox", 10, GUI_LABEL_LEFT, dataBaseEntry[currentDatabaseRecord].notes);

}
