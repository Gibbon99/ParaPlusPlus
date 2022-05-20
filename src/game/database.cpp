#include "system/util.h"
#include "gui/guiLanguage.h"
#include "game/player.h"
#include "game/audio.h"
#include "game/texture.h"
#include "game/database.h"
#include "io/libfmemopen.h"

tinyxml2::XMLDocument *xmlFileSave {};
tinyxml2::XMLDocument *xmlFileLoad {};
tinyxml2::XMLNode     *rootNode {};


std::vector<dataBaseEntry_> dataBaseEntry {};
CSimpleIniA                 databaseFile {};
int                         currentDatabaseRecord {};

//-----------------------------------------------------------------------------------------------------
//
// Load a droid database file.
bool io_getDBDroidInfo(const std::string &fileName)
//-----------------------------------------------------------------------------------------------------
{
	tinyxml2::XMLElement *elementPtr;
	dataBaseEntry_       tempDataBaseEntry;

	xmlFileLoad = new tinyxml2::XMLDocument;
	//
	// Load file from packfile into memory for reading by tinyxml
	std::string memFile = fileSystem.getString (fileName);
	if (memFile.empty ())
	{
		logFile.write (sys_getString ("Unable to load file [ %s ] into memory.", fileName.c_str ()));
		return false;
	}
	auto memFilePtr = fmemopen ((void *) memFile.data (), memFile.size (), "rb");

	tinyxml2::XMLError eResult = xmlFileLoad->LoadFile (memFilePtr);
	gam_checkXMLReturnCode (eResult);

	rootNode = xmlFileLoad->FirstChild ();
	if (nullptr == rootNode)
	{
		logFile.write (sys_getString ("Unable to load XML file [ %s ]", fileName.c_str ()));
		return false;
	}
	//
	// Start getting all the info from the database droid file
	if (dataBaseEntry.empty ())    // 001 base droid stats
	{
		//
		// maxHealth - INT
		elementPtr = rootNode->FirstChildElement ("maxHealth");
		if (nullptr == elementPtr)
		{
			logFile.write (sys_getString ("Unable to find node 'mapVersion' in file [ %s ]", fileName.c_str ()));
			return false;
		}
		eResult = elementPtr->QueryIntText (&tempDataBaseEntry.maxHealth);
		gam_checkXMLReturnCode (eResult);
		//
		// rechargeTime - FLOAT
		elementPtr = rootNode->FirstChildElement ("rechargeTime");
		if (nullptr == elementPtr)
		{
			logFile.write (sys_getString ("Unable to find node 'rechargeTime' in file [ %s ]", fileName.c_str ()));
			return false;
		}
		eResult = elementPtr->QueryFloatText (&tempDataBaseEntry.rechargeTime);
		gam_checkXMLReturnCode (eResult);
		//
		// maxSpeed - FLOAT
		elementPtr = rootNode->FirstChildElement ("maxSpeed");
		if (nullptr == elementPtr)
		{
			logFile.write (sys_getString ("Unable to find node 'maxSpeed' in file [ %s ]", fileName.c_str ()));
			return false;
		}
		eResult = elementPtr->QueryFloatText (&tempDataBaseEntry.maxSpeed);
		gam_checkXMLReturnCode (eResult);
		tempDataBaseEntry.maxSpeed /= baseGameSpeed;
		//
		// accelerate - FLOAT
		elementPtr = rootNode->FirstChildElement ("accelerate");
		if (nullptr == elementPtr)
		{
			logFile.write (sys_getString ("Unable to find node 'accelerate' in file [ %s ]", fileName.c_str ()));
			return false;
		}
		eResult = elementPtr->QueryFloatText (&tempDataBaseEntry.accelerate);
		gam_checkXMLReturnCode (eResult);
		tempDataBaseEntry.accelerate /= baseGameSpeed;
	}
	else
	{
		//
		// Base other values on the 001 initial value to make it easier to change all game variables.
		float tempVarFloat {};
		int   tempVarInt {};
		//
		// maxHealth - INT
		elementPtr = rootNode->FirstChildElement ("maxHealth");
		if (nullptr == elementPtr)
		{
			logFile.write (sys_getString ("Unable to find node 'maxHealth' in file [ %s ]", fileName.c_str ()));
			return false;
		}
		eResult = elementPtr->QueryIntText (&tempVarInt);
		gam_checkXMLReturnCode (eResult);
		tempDataBaseEntry.maxHealth = dataBaseEntry[0].maxHealth + tempVarInt;
		//
		// rechargeTime - FLOAT
		elementPtr = rootNode->FirstChildElement ("rechargeTime");
		if (nullptr == elementPtr)
		{
			logFile.write (sys_getString ("Unable to find node 'rechargeTime' in file [ %s ]", fileName.c_str ()));
			return false;
		}
		eResult = elementPtr->QueryFloatText (&tempVarFloat);
		gam_checkXMLReturnCode (eResult);
		tempDataBaseEntry.rechargeTime = dataBaseEntry[0].rechargeTime + tempVarFloat;
		//
		// maxSpeed - FLOAT
		elementPtr = rootNode->FirstChildElement ("maxSpeed");
		if (nullptr == elementPtr)
		{
			logFile.write (sys_getString ("Unable to find node 'maxSpeed' in file [ %s ]", fileName.c_str ()));
			return false;
		}
		eResult = elementPtr->QueryFloatText (&tempVarFloat);
		gam_checkXMLReturnCode (eResult);
		tempDataBaseEntry.maxSpeed = dataBaseEntry[0].maxSpeed + tempVarFloat;
		//
		// accelerate - FLOAT
		elementPtr = rootNode->FirstChildElement ("accelerate");
		if (nullptr == elementPtr)
		{
			logFile.write (sys_getString ("Unable to find node 'accelerate' in file [ %s ]", fileName.c_str ()));
			return false;
		}
		eResult = elementPtr->QueryFloatText (&tempVarFloat);
		gam_checkXMLReturnCode (eResult);
		tempDataBaseEntry.accelerate = dataBaseEntry[0].accelerate + tempVarFloat;
	}
	//
	// Now do all the variables that are not changed.
	//
	// score - INT
	elementPtr = rootNode->FirstChildElement ("score");
	if (nullptr == elementPtr)
	{
		logFile.write (sys_getString ("Unable to find node 'score' in file [ %s ]", fileName.c_str ()));
		return false;
	}
	eResult = elementPtr->QueryIntText (&tempDataBaseEntry.score);
	gam_checkXMLReturnCode (eResult);
	//
	// bounceDamage - INT
	elementPtr = rootNode->FirstChildElement ("bounceDamage");
	if (nullptr == elementPtr)
	{
		logFile.write (sys_getString ("Unable to find node 'bounceDamage' in file [ %s ]", fileName.c_str ()));
		return false;
	}
	eResult = elementPtr->QueryIntText (&tempDataBaseEntry.bounceDamage);
	gam_checkXMLReturnCode (eResult);
	//
	// canShoot - BOOL
	elementPtr = rootNode->FirstChildElement ("canShoot");
	if (nullptr == elementPtr)
	{
		logFile.write (sys_getString ("Unable to find node 'canShoot' in file [ %s ]", fileName.c_str ()));
		return false;
	}
	eResult = elementPtr->QueryBoolText (&tempDataBaseEntry.canShoot);
	gam_checkXMLReturnCode (eResult);
	//
	// bulletType - INT
	elementPtr = rootNode->FirstChildElement ("bulletType");
	if (nullptr == elementPtr)
	{
		logFile.write (sys_getString ("Unable to find node 'bulletType' in file [ %s ]", fileName.c_str ()));
		return false;
	}
	eResult = elementPtr->QueryIntText (&tempDataBaseEntry.bulletType);
	gam_checkXMLReturnCode (eResult);

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
	//
	// bulletDamage - INT
	elementPtr = rootNode->FirstChildElement ("bulletDamage");
	if (nullptr == elementPtr)
	{
		logFile.write (sys_getString ("Unable to find node 'bulletDamage' in file [ %s ]", fileName.c_str ()));
		return false;
	}
	eResult = elementPtr->QueryIntText (&tempDataBaseEntry.bulletDamage);
	gam_checkXMLReturnCode (eResult);
	//
	// disrupterImmune - BOOL
	elementPtr = rootNode->FirstChildElement ("disrupterImmune");
	if (nullptr == elementPtr)
	{
		logFile.write (sys_getString ("Unable to find node 'disrupterImmune' in file [ %s ]", fileName.c_str ()));
		return false;
	}
	eResult = elementPtr->QueryBoolText (&tempDataBaseEntry.disrupterImmune);
	gam_checkXMLReturnCode (eResult);

	//
	// tokenCount - INT
	elementPtr = rootNode->FirstChildElement ("tokenCount");
	if (nullptr == elementPtr)
	{
		logFile.write (sys_getString ("Unable to find node 'tokenCount' in file [ %s ]", fileName.c_str ()));
		return false;
	}
	eResult = elementPtr->QueryIntText (&tempDataBaseEntry.tokenCount);
	gam_checkXMLReturnCode (eResult);
	//
	// height - STRING
	elementPtr = rootNode->FirstChildElement ("height");
	if (nullptr == elementPtr)
	{
		logFile.write (sys_getString ("Unable to find node 'height' in file [ %s ]", fileName.c_str ()));
		return false;
	}
	tempDataBaseEntry.height = elementPtr->GetText ();
	if (tempDataBaseEntry.height.empty ())
		gam_checkXMLReturnCode (tinyxml2::XML_NO_TEXT_NODE);
	//
	// weight - STRING
	elementPtr = rootNode->FirstChildElement ("weight");
	if (nullptr == elementPtr)
	{
		logFile.write (sys_getString ("Unable to find node 'weight' in file [ %s ]", fileName.c_str ()));
		return false;
	}
	tempDataBaseEntry.weight = elementPtr->GetText ();
	if (tempDataBaseEntry.weight.empty ())
		gam_checkXMLReturnCode (tinyxml2::XML_NO_TEXT_NODE);
	//
	// height - STRING
	elementPtr = rootNode->FirstChildElement ("height");
	if (nullptr == elementPtr)
	{
		logFile.write (sys_getString ("Unable to find node 'height' in file [ %s ]", fileName.c_str ()));
		return false;
	}
	tempDataBaseEntry.height = elementPtr->GetText ();
	if (tempDataBaseEntry.height.empty ())
		gam_checkXMLReturnCode (tinyxml2::XML_NO_TEXT_NODE);
	//
	// dbEntry - STRING
	elementPtr = rootNode->FirstChildElement ("dbEntry");
	if (nullptr == elementPtr)
	{
		logFile.write (sys_getString ("Unable to find node 'dbEntry' in file [ %s ]", fileName.c_str ()));
		return false;
	}
	tempDataBaseEntry.dbImageFileName = elementPtr->GetText ();
	if (tempDataBaseEntry.dbImageFileName.empty ())
		gam_checkXMLReturnCode (tinyxml2::XML_NO_TEXT_NODE);

	tempDataBaseEntry.notes = gui_getString (tempDataBaseEntry.dbImageFileName);

	dataBaseEntry.push_back (tempDataBaseEntry);

	delete xmlFileLoad;

	return true;
}

//------------------------------------------------------------------------------------------------------------------------------------
//
// Get and build up information for droid database files
void gam_getDBInformation()
//------------------------------------------------------------------------------------------------------------------------------------
{
	dataBaseEntry.clear();

	io_getDBDroidInfo ("001.data");
	io_getDBDroidInfo ("123.data");
	io_getDBDroidInfo ("139.data");
	io_getDBDroidInfo ("247.data");
	io_getDBDroidInfo ("249.data");
	io_getDBDroidInfo ("296.data");
	io_getDBDroidInfo ("302.data");
	io_getDBDroidInfo ("329.data");
	io_getDBDroidInfo ("420.data");
	io_getDBDroidInfo ("476.data");
	io_getDBDroidInfo ("493.data");
	io_getDBDroidInfo ("516.data");
	io_getDBDroidInfo ("571.data");
	io_getDBDroidInfo ("598.data");
	io_getDBDroidInfo ("614.data");
	io_getDBDroidInfo ("615.data");
	io_getDBDroidInfo ("629.data");
	io_getDBDroidInfo ("711.data");
	io_getDBDroidInfo ("742.data");
	io_getDBDroidInfo ("751.data");
	io_getDBDroidInfo ("821.data");
	io_getDBDroidInfo ("834.data");
	io_getDBDroidInfo ("883.data");
	io_getDBDroidInfo ("999.data");
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
