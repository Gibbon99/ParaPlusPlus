//----------------------------------------------------------------------------------------------------------------------
//
// Load all the Audio sound files
void as_loadAudioResources()
//----------------------------------------------------------------------------------------------------------------------
{
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "start1.wav");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load all the texture files
void as_loadTextureResources()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "splash.bmp|splash| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "planet.bmp|planet| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "hudNew.bmp|hudNew| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "screen.bmp|screen| ");

	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "db_999.bmp|db_droid| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "001.bmp|001| ");

	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_MAP, 0, "planet|planet| ");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Load all the deck information
void as_loadAllDecks()
//----------------------------------------------------------------------------------------------------------------------
{
	string fileName;

	for (int i = 0; i != 21; i++)
	{
		if (i != 7)
		{
			fileName = "116-newDeck" + formatInt (i, "l") + ".dat| ";
			sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_DECK, 0, fileName);
		}
	}
}