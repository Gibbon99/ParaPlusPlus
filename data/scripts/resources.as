//----------------------------------------------------------------------------------------------------------------------
//
// Load all the Audio sound files
void as_loadAudioResources()
//----------------------------------------------------------------------------------------------------------------------
{
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "collosion1.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "endTransmission1.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "greenAlert.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "lift1.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "scrollBeeps.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "transferdeadlock.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "yellowAlert.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "console1.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "endTransmission2.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "keypressBad.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "lift2.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "start1.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "transferMove.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "console2.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "energyHeal.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "keyPressGood.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "lift3.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "start2.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "transferStage1.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "damage.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "explode1.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "laser.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "lowEnergy.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "startAll.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "transferStage2.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "disruptor.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "explode2.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "levelShutdown.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "redAlert.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "transfer1.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "transferStart.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "doorOpen.wav");
	gam_addAudioEvent(EVENT_ACTION_AUDIO_LOAD, false, 0, 0, "doorClose.wav");
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
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "123.bmp|123| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "139.bmp|139| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "247.bmp|247| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "249.bmp|249| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "296.bmp|296| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "302.bmp|302| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "329.bmp|329| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "420.bmp|420| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "476.bmp|476| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "493.bmp|493| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "516.bmp|516| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "571.bmp|571| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "598.bmp|598| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "614.bmp|614| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "615.bmp|615| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "629.bmp|629| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "711.bmp|711| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "742.bmp|742| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "751.bmp|751| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "821.bmp|821| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "834.bmp|834| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "883.bmp|883| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "999.bmp|999| ");

	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "bullet_001.bmp|bullet_001| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "bullet_476.bmp|bullet_476| ");
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_LOAD_TEXTURE, 0, "bullet_821.bmp|bullet_821| ");

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