
//----------------------------------------------------------------------------------------------------------------------
//
// Display the specifications of the opened Audio device
void as_getAudioSpecs()
//----------------------------------------------------------------------------------------------------------------------
{
	sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, "Audio device");
	as_audio.deviceInfo();
}

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
// Set the master volume to a new level
void as_setVolume(int &in newVolume)
//----------------------------------------------------------------------------------------------------------------------
{
	int currentVolume;

	as_audio.setMasterVolume(newVolume);
	currentVolume = as_audio.getMasterVolume();
	sys_printConInt("Volume level : ", currentVolume);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the current master volume
int as_getVolume()
//----------------------------------------------------------------------------------------------------------------------
{
	int currentVolume;
	string currentVolumeStr;

	currentVolume = as_audio.getMasterVolume();

	currentVolumeStr = "Volume : ";
	currentVolumeStr += formatInt(currentVolume, "l");

	sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, currentVolumeStr);

	sys_printConInt("Volume level : ", currentVolume);

	return -1;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Change to using a new renderer - called from host program
//
// Pass in the index of the new renderer
void as_useNewRenderer(int &in newRenderer)
//----------------------------------------------------------------------------------------------------------------------
{
//	as_renderer.getRendererInfo();
//	as_renderer.d_getAllRenderers();
//sys_printConInt(newIndex, 99);

	string newIndex;

	newIndex = formatInt(newRenderer, "l");   // Put the int parameter into the string, (l) left justified
	sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_GAME_USE_NEW_RENDERER, 0, newIndex);
}

void as_testFunction()
{
	sys_addEvent (EVENT_TYPE_CONSOLE, EVENT_ACTION_CONSOLE_ADD_LINE, 0, "Text from inside the script");

}

