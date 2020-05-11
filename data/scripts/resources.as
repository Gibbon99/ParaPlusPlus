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
	gam_loadTexture("splash.bmp", "splash");
	gam_loadTexture("planet.bmp", "planet");
	gam_loadTexture("hudNew.bmp", "hudNew");
	gam_loadTexture("screen.bmp", "screen");

	gam_loadTexture("db_999.bmp", "db_droid");

	gam_createCollisionMap("planet");
}