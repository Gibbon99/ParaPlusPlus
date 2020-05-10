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
	as_texture.load("splash.bmp");
	as_texture.load("planet.bmp");
	as_texture.createMap("planet");
	as_texture.load("hudNew.bmp");
}