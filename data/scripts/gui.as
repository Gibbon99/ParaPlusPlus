int buttonWidth   = 40;
int buttonHeight  = 10;
int buttonSpacing = 13;
int buttonStartX  = 10;

//----------------------------------------------------------------------------------------------------------------------
//
// Handle the actions from the main menu
void as_handleMainMenu ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "mainMenu.startGameButton"))
	{

		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "mainMenu.optionsButton"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "optionsMenu"));
		// Set active object
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "optionsMenu.backButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "mainMenu.quitButton"))
	{
		quitProgram = true;
		return;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Handle the actions from the options menu
void as_handleOptionsMenu ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "optionsMenu.videoButton"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "videoMenu"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "videoMenu.backButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "optionsMenu.audioButton"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "audioOptions"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "audioOptions.backButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "optionsMenu.controlsButton"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "controlsMenu"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "controlsMenu.backButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "optionsMenu.gameButton"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "gameMenu"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "gameMenu.backButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "optionsMenu.backButton"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "mainMenu"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "mainMenu.optionsButton");
		return;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Process the elements on the Audio menu
void as_handleAudioMenu ()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	int enableSoundInt;

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "audioOptions.backButton"))
	{
		g_volumeLevel = parseInt(as_paraGui.getSliderValue("audioOptions.volume"));
		as_audio.setMasterVolume(g_volumeLevel);
		io_updateConfigValueInt("g_volumeLevel", g_volumeLevel);

		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "optionsMenu"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "optionsMenu.audioButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex (GUI_OBJECT_CHECKBOX, "audioOptions.enableSound"))
	{
		enableSound = !enableSound;
		as_paraGui.setTickedStatus ("audioOptions.enableSound", -1, enableSound);
		enableSoundInt = enableSound ? 1: 0;
		io_updateConfigValueInt("enableSound", enableSoundInt);
		if (enableSound == true)
		{
			as_paraGui.setReady (GUI_OBJECT_SLIDER, "audioOptions.volume", true);
			as_paraGui.setReady (GUI_OBJECT_SLIDER, "audioOptions.speakers", true);
		}
		else
		{
			as_paraGui.setReady (GUI_OBJECT_SLIDER, "audioOptions.volume", false);
			as_paraGui.setReady (GUI_OBJECT_SLIDER, "audioOptions.speakers", false);
		}
		return;
	}

	if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex (GUI_OBJECT_SLIDER, "audioOptions.volume"))
	{
		return;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Process the elements on the Control menu
void as_handleControlsMenu ()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "controlsMenu.backButton"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "optionsMenu"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "optionsMenu.controlsButton");
		return;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Process the elements on the Control menu
void as_handleGameMenu ()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "gameMenu.backButton"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "optionsMenu"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "optionsMenu.gameButton");
		return;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Process the elements on the Control menu
void as_handleVideoMenu ()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "videoMenu.backButton"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "optionsMenu"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "optionsMenu.videoButton");
		return;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Create the Video options menu. Window Setting; Which Renderer; Use VSync Checkbox
void createVideoMenu ()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	int numRenderers;
	string tempNum;
	string currentRendererStr;

	as_paraGui.create (GUI_OBJECT_SCREEN, "videoMenu");

	as_paraGui.create (GUI_OBJECT_LABEL, "videoOptions.heading");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "videoOptions.heading", "videoMenu");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "videoOptions.heading", 0, GUI_COORD_ABSOLUTE, 3, 3, 2, 2);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "videoOptions.heading", 0, GUI_LABEL_LEFT, gui_getString("videoOptions.heading"));
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "videoOptions.heading", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "videoOptions.heading", true);
	//
	// Renderers
	//
	numRenderers = as_renderer.getNumRenderers();
	as_paraGui.create (GUI_OBJECT_SLIDER, "videoOptions.renderers");
	as_paraGui.addToScreen (GUI_OBJECT_SLIDER, "videoOptions.renderers", "videoMenu");
	as_paraGui.setPosition (GUI_OBJECT_SLIDER, "videoOptions.renderers", 8, GUI_COORD_PERCENT, 5, 20, 30, 2);
	as_paraGui.setLabel (GUI_OBJECT_SLIDER, "videoOptions.renderers", 12, GUI_LABEL_CENTER, gui_getString ("videoOptions.renderers"));
	as_paraGui.setAction (GUI_OBJECT_SLIDER, "videoOptions.renderers", "as_handleVideoMenu");

	for (int i = 0; i != numRenderers; i++)
	{
		tempNum = formatInt (i, "l");
		as_paraGui.addNewElement ("videoOptions.renderers", tempNum, as_renderer.getRendererByIndex(i), SLIDER_TYPE_INT);
	}

	currentRendererStr = formatInt(as_renderer.getCurrentRendererIndex(), "l");
	as_paraGui.setSliderValue ("videoOptions.renderers", currentRendererStr);
	as_paraGui.setFontName (GUI_OBJECT_SLIDER, "videoOptions.renderers", "guiFont28");
	as_paraGui.setReady (GUI_OBJECT_SLIDER, "videoOptions.renderers", true);
	//
	// Display
	//
	as_paraGui.create (GUI_OBJECT_SLIDER, "videoOptions.display");
	as_paraGui.addToScreen (GUI_OBJECT_SLIDER, "videoOptions.display", "videoMenu");
	as_paraGui.setPosition (GUI_OBJECT_SLIDER, "videoOptions.display", 8, GUI_COORD_PERCENT, 5, 52, 30, 2);
	as_paraGui.setLabel (GUI_OBJECT_SLIDER, "videoOptions.display", 12, GUI_LABEL_CENTER, gui_getString ("videoOptions.display"));
	as_paraGui.setAction (GUI_OBJECT_SLIDER, "videoOptions.display", "as_handleVideoMenu");

	as_paraGui.addNewElement ("videoOptions.display", "0", gui_getString("videoOptions.windowed"), SLIDER_TYPE_INT);
	as_paraGui.addNewElement ("videoOptions.display", "1", gui_getString("videoOptions.fullscreen"), SLIDER_TYPE_INT);
	as_paraGui.addNewElement ("videoOptions.display", "2", gui_getString("videoOptions.desktop"), SLIDER_TYPE_INT);

//	currentRendererStr = formatInt(as_renderer.getCurrentRendererIndex(), "l");
//	as_paraGui.setSliderValue ("videoOptions.display", currentRendererStr);
	as_paraGui.setFontName (GUI_OBJECT_SLIDER, "videoOptions.display", "guiFont28");
	as_paraGui.setReady (GUI_OBJECT_SLIDER, "videoOptions.display", true);
	//
	// Scale quality
	//
	as_paraGui.create (GUI_OBJECT_SLIDER, "videoOptions.scalequality");
	as_paraGui.addToScreen (GUI_OBJECT_SLIDER, "videoOptions.scalequality", "videoMenu");
	as_paraGui.setPosition (GUI_OBJECT_SLIDER, "videoOptions.scalequality", 8, GUI_COORD_PERCENT, 5, 84, 30, 2);
	as_paraGui.setLabel (GUI_OBJECT_SLIDER, "videoOptions.scalequality", 12, GUI_LABEL_CENTER, gui_getString ("videoOptions.scalequality"));
	as_paraGui.setAction (GUI_OBJECT_SLIDER, "videoOptions.scalequality", "as_handleVideoMenu");

	as_paraGui.addNewElement ("videoOptions.scalequality", "0", gui_getString("videoOptions.nearest"), SLIDER_TYPE_INT);
	as_paraGui.addNewElement ("videoOptions.scalequality", "1", gui_getString("videoOptions.linear"), SLIDER_TYPE_INT);
	as_paraGui.addNewElement ("videoOptions.scalequality", "2", gui_getString("videoOptions.best"), SLIDER_TYPE_INT);

//	currentRendererStr = formatInt(as_renderer.getCurrentRendererIndex(), "l");
//	as_paraGui.setSliderValue ("videoOptions.display", currentRendererStr);
	as_paraGui.setFontName (GUI_OBJECT_SLIDER, "videoOptions.scalequality", "guiFont28");
	as_paraGui.setReady (GUI_OBJECT_SLIDER, "videoOptions.scalequality", true);
	//
	// Use VSync
	//
	as_paraGui.create (GUI_OBJECT_CHECKBOX, "videoOptions.usevsync");
	as_paraGui.addToScreen (GUI_OBJECT_CHECKBOX, "videoOptions.usevsync", "videoMenu");
	as_paraGui.setPosition (GUI_OBJECT_CHECKBOX, "videoOptions.usevsync", 3, GUI_COORD_PERCENT, 60, 15, 5, 7);
	as_paraGui.setLabel (GUI_OBJECT_CHECKBOX, "videoOptions.usevsync", 6, GUI_LABEL_LEFT, gui_getString("videoOptions.usevsync"));
	as_paraGui.setFontName (GUI_OBJECT_CHECKBOX, "videoOptions.usevsync", "guiFont28");
	as_paraGui.setAction (GUI_OBJECT_CHECKBOX, "videoOptions.usevsync", "as_handleVideoMenu");
	as_paraGui.setReady (GUI_OBJECT_CHECKBOX, "videoOptions.usevsync", true);
	as_paraGui.setTickedStatus ("videoOptions.usevsync", -1, presentVSync);
	//
	// Borderless window
	//
	as_paraGui.create (GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow");
	as_paraGui.addToScreen (GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow", "videoMenu");
	as_paraGui.setPosition (GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow", 3, GUI_COORD_PERCENT, 60, 30, 5, 7);
	as_paraGui.setLabel (GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow", 6, GUI_LABEL_LEFT, gui_getString("videoOptions.borderlesswindow"));
	as_paraGui.setFontName (GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow", "guiFont28");
	as_paraGui.setAction (GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow", "as_handleVideoMenu");
	as_paraGui.setReady (GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow", true);
	as_paraGui.setTickedStatus ("videoOptions.borderlesswindow", -1, windowBorderless);
	//
	// Allow high DPI
	//
	as_paraGui.create (GUI_OBJECT_CHECKBOX, "videoOptions.highdpi");
	as_paraGui.addToScreen (GUI_OBJECT_CHECKBOX, "videoOptions.highdpi", "videoMenu");
	as_paraGui.setPosition (GUI_OBJECT_CHECKBOX, "videoOptions.highdpi", 3, GUI_COORD_PERCENT, 60, 45, 5, 7);
	as_paraGui.setLabel (GUI_OBJECT_CHECKBOX, "videoOptions.highdpi", 6, GUI_LABEL_LEFT, gui_getString("videoOptions.highdpi"));
	as_paraGui.setFontName (GUI_OBJECT_CHECKBOX, "videoOptions.highdpi", "guiFont28");
	as_paraGui.setAction (GUI_OBJECT_CHECKBOX, "videoOptions.highdpi", "as_handleVideoMenu");
	as_paraGui.setReady (GUI_OBJECT_CHECKBOX, "videoOptions.highdpi", true);
	as_paraGui.setTickedStatus ("videoOptions.highdpi", -1, windowAllowHighDPI);
	//
	// Apply Button
	//
	as_paraGui.create (GUI_OBJECT_BUTTON, "videoMenu.applyButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "videoMenu.applyButton", "videoMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "videoMenu.applyButton", 3, GUI_LABEL_CENTER, gui_getString ("videoMenu.applyButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "videoMenu.applyButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "videoMenu.applyButton", 8, GUI_COORD_PERCENT, 59, 70, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "videoMenu.applyButton", "as_handleVideoMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "videoMenu.applyButton", true);
	//
	// Back button
	//
	as_paraGui.create (GUI_OBJECT_BUTTON, "videoMenu.backButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "videoMenu.backButton", "videoMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "videoMenu.backButton", 3, GUI_LABEL_CENTER, gui_getString ("backButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "videoMenu.backButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "videoMenu.backButton", 8, GUI_COORD_PERCENT, 59, 85, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "videoMenu.backButton", "as_handleVideoMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "videoMenu.backButton", true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
// Create the Audio options menu. Checkbox: enable sound; Slider: Volume; Slider: numChannels ( 1, 2)
void createAudioMenu ()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	string currentVolumeStr;
	string tempNum;

	as_paraGui.create (GUI_OBJECT_SCREEN, "audioOptions");

	as_paraGui.create (GUI_OBJECT_LABEL, "audioOptions.heading");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "audioOptions.heading", "audioOptions");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "audioOptions.heading", 0, GUI_COORD_ABSOLUTE, 3, 3, 2, 2);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "audioOptions.heading", 0, GUI_LABEL_LEFT, gui_getString("audioOptions.heading"));
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "audioOptions.heading", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "audioOptions.heading", true);

	as_paraGui.create (GUI_OBJECT_CHECKBOX, "audioOptions.enableSound");
	as_paraGui.addToScreen (GUI_OBJECT_CHECKBOX, "audioOptions.enableSound", "audioOptions");
	as_paraGui.setPosition (GUI_OBJECT_CHECKBOX, "audioOptions.enableSound", 3, GUI_COORD_PERCENT, 2, 20, 5, 7);
	as_paraGui.setLabel (GUI_OBJECT_CHECKBOX, "audioOptions.enableSound", 6, GUI_LABEL_LEFT, gui_getString("audioOptions.enableSound"));
	as_paraGui.setFontName (GUI_OBJECT_CHECKBOX, "audioOptions.enableSound", "guiFont28");
	as_paraGui.setAction (GUI_OBJECT_CHECKBOX, "audioOptions.enableSound", "as_handleAudioMenu");
	as_paraGui.setReady (GUI_OBJECT_CHECKBOX, "audioOptions.enableSound", true);
	as_paraGui.setTickedStatus ("audioOptions.enableSound", -1, enableSound);

	as_paraGui.create (GUI_OBJECT_SLIDER, "audioOptions.volume");
	as_paraGui.addToScreen (GUI_OBJECT_SLIDER, "audioOptions.volume", "audioOptions");
	as_paraGui.setPosition (GUI_OBJECT_SLIDER, "audioOptions.volume", 8, GUI_COORD_PERCENT, 5, 50, 40, 2);
	as_paraGui.setLabel (GUI_OBJECT_SLIDER, "audioOptions.volume", 12, GUI_LABEL_CENTER, gui_getString ("audioOptions.volume"));
	as_paraGui.setAction (GUI_OBJECT_SLIDER, "audioOptions.volume", "as_handleAudioMenu");

	for (int i = 0; i != 10; i++)
	{
		tempNum = formatInt (i + 1, "l");
		as_paraGui.addNewElement ("audioOptions.volume", tempNum, tempNum, SLIDER_TYPE_INT);
	}

	currentVolumeStr = formatInt (g_volumeLevel, "l");
	as_paraGui.setSliderValue ("audioOptions.volume", currentVolumeStr);
	as_paraGui.setFontName (GUI_OBJECT_SLIDER, "audioOptions.volume", "guiFont28");

	as_paraGui.create (GUI_OBJECT_SLIDER, "audioOptions.speakers");
	as_paraGui.addToScreen (GUI_OBJECT_SLIDER, "audioOptions.speakers", "audioOptions");
	as_paraGui.setPosition (GUI_OBJECT_SLIDER, "audioOptions.speakers", 8, GUI_COORD_PERCENT, 55, 50, 40, 2);
	as_paraGui.setLabel (GUI_OBJECT_SLIDER, "audioOptions.speakers", 12, GUI_LABEL_LEFT, gui_getString ("audioOptions.speakers"));
	as_paraGui.setAction (GUI_OBJECT_SLIDER, "audioOptions.speakers", "as_handleAudioMenu");
	as_paraGui.addNewElement ("audioOptions.speakers", "0", "Mono", SLIDER_TYPE_INT);
	as_paraGui.addNewElement ("audioOptions.speakers", "1", "Stereo", SLIDER_TYPE_INT);
	as_paraGui.setFontName (GUI_OBJECT_SLIDER, "audioOptions.speakers", "guiFont28");

	if (enableSound == true)
	{
		as_paraGui.setReady (GUI_OBJECT_SLIDER, "audioOptions.volume", true);
		as_paraGui.setReady (GUI_OBJECT_SLIDER, "audioOptions.speakers", true);
	}
	else
	{
		as_paraGui.setReady (GUI_OBJECT_SLIDER, "audioOptions.volume", false);
		as_paraGui.setReady (GUI_OBJECT_SLIDER, "audioOptions.speakers", false);
	}

	as_paraGui.create (GUI_OBJECT_BUTTON, "audioOptions.backButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "audioOptions.backButton", "audioOptions");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "audioOptions.backButton", 3, GUI_LABEL_CENTER, gui_getString ("backButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "audioOptions.backButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "audioOptions.backButton", 8, GUI_COORD_PERCENT, 59, 85, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "audioOptions.backButton", "as_handleAudioMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "audioOptions.backButton", true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Create the Controls menu
void createControlsMenu ()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create (GUI_OBJECT_SCREEN, "controlsMenu");

	as_paraGui.create (GUI_OBJECT_LABEL, "controlsOptions.heading");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "controlsOptions.heading", "controlsMenu");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "controlsOptions.heading", 0, GUI_COORD_ABSOLUTE, 3, 3, 2, 2);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "controlsOptions.heading", 0, GUI_LABEL_LEFT, gui_getString("controlsOptions.heading"));
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "controlsOptions.heading", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "controlsOptions.heading", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "controlsMenu.backButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "controlsMenu.backButton", "controlsMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "controlsMenu.backButton", 3, GUI_LABEL_CENTER, gui_getString ("backButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "controlsMenu.backButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "controlsMenu.backButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 5, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "controlsMenu.backButton", "as_handleControlsMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "controlsMenu.backButton", true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Create the Game menu
void createGameMenu ()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create (GUI_OBJECT_SCREEN, "gameMenu");

	as_paraGui.create (GUI_OBJECT_LABEL, "gameOptions.heading");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "gameOptions.heading", "gameMenu");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "gameOptions.heading", 0, GUI_COORD_ABSOLUTE, 3, 3, 2, 2);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "gameOptions.heading", 0, GUI_LABEL_LEFT, gui_getString("gameOptions.heading"));
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "gameOptions.heading", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "gameOptions.heading", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "gameMenu.backButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "gameMenu.backButton", "gameMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "gameMenu.backButton", 3, GUI_LABEL_CENTER, gui_getString ("backButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "gameMenu.backButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "gameMenu.backButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 5, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "gameMenu.backButton", "as_handleGameMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "gameMenu.backButton", true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the options menu
void createOptionsMenu ()
//----------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create (GUI_OBJECT_SCREEN, "optionsMenu");

	as_paraGui.create (GUI_OBJECT_BUTTON, "optionsMenu.videoButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "optionsMenu.videoButton", "optionsMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "optionsMenu.videoButton", 3, GUI_LABEL_CENTER, gui_getString ("optionsMenu.videoButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "optionsMenu.videoButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "optionsMenu.videoButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 1, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "optionsMenu.videoButton", "as_handleOptionsMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "optionsMenu.videoButton", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "optionsMenu.audioButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "optionsMenu.audioButton", "optionsMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "optionsMenu.audioButton", 3, GUI_LABEL_CENTER, gui_getString ("optionsMenu.audioButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "optionsMenu.audioButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "optionsMenu.audioButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 2, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "optionsMenu.audioButton", "as_handleOptionsMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "optionsMenu.audioButton", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "optionsMenu.controlsButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "optionsMenu.controlsButton", "optionsMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "optionsMenu.controlsButton", 3, GUI_LABEL_CENTER, gui_getString ("optionsMenu.controlsButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "optionsMenu.controlsButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "optionsMenu.controlsButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 3, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "optionsMenu.controlsButton", "as_handleOptionsMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "optionsMenu.controlsButton", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "optionsMenu.gameButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "optionsMenu.gameButton", "optionsMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "optionsMenu.gameButton", 3, GUI_LABEL_CENTER, gui_getString ("optionsMenu.gameButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "optionsMenu.gameButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "optionsMenu.gameButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 4, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "optionsMenu.gameButton", "as_handleOptionsMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "optionsMenu.gameButton", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "optionsMenu.backButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "optionsMenu.backButton", "optionsMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "optionsMenu.backButton", 3, GUI_LABEL_CENTER, gui_getString ("backButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "optionsMenu.backButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "optionsMenu.backButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 5, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "optionsMenu.backButton", "as_handleOptionsMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "optionsMenu.backButton", true);

}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the main menu
void createMainMenu ()
//----------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create (GUI_OBJECT_SCREEN, "mainMenu");

	as_paraGui.create (GUI_OBJECT_BUTTON, "mainMenu.startGameButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "mainMenu.startGameButton", "mainMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "mainMenu.startGameButton", 3, GUI_LABEL_CENTER, gui_getString ("mainMenu.startGameButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "mainMenu.startGameButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "mainMenu.startGameButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 1, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "mainMenu.startGameButton", "as_handleMainMenu");

	as_paraGui.create (GUI_OBJECT_BUTTON, "mainMenu.optionsButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "mainMenu.optionsButton", "mainMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "mainMenu.optionsButton", 3, GUI_LABEL_CENTER, gui_getString ("mainMenu.optionsButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "mainMenu.optionsButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "mainMenu.optionsButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 2, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "mainMenu.optionsButton", "as_handleMainMenu");

	as_paraGui.create (GUI_OBJECT_BUTTON, "mainMenu.tutorialButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", "mainMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", 3, GUI_LABEL_CENTER, gui_getString ("mainMenu.tutorialButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 3, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", "as_handleMainMenu");

	as_paraGui.create (GUI_OBJECT_BUTTON, "mainMenu.briefingButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "mainMenu.briefingButton", "mainMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "mainMenu.briefingButton", 3, GUI_LABEL_CENTER, gui_getString ("mainMenu.briefingButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "mainMenu.briefingButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "mainMenu.briefingButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 4, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "mainMenu.briefingButton", "as_handleMainMenu");

	as_paraGui.create (GUI_OBJECT_BUTTON, "mainMenu.quitButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "mainMenu.quitButton", "mainMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "mainMenu.quitButton", 3, GUI_LABEL_CENTER, gui_getString ("mainMenu.quitButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "mainMenu.quitButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "mainMenu.quitButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 5, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "mainMenu.quitButton", "as_handleMainMenu");

	as_paraGui.setReady (GUI_OBJECT_BUTTON, "mainMenu.startGameButton", true);
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "mainMenu.optionsButton", true);
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", true);
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "mainMenu.briefingButton", true);
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "mainMenu.quitButton", true);

}

//-----------------------------------------------------------------------------
//
// Create colors for the sideview graphic
void as_setSideviewColors ()
//-----------------------------------------------------------------------------
{
	as_createSideViewColor (SIDEVIEW_SHIP_COLOR, 142, 182, 12, 255);
	as_createSideViewColor (SIDEVIEW_ACTIVE_DECK_COLOR, 71, 199, 232, 255);
	as_createSideViewColor (SIDEVIEW_ENGINE_COLOR, 34, 34, 34, 255);
	as_createSideViewColor (SIDEVIEW_LIFT_COLOR, 18, 76, 88, 255);
	as_createSideViewColor (SIDEVIEW_ACTIVE_LIFT_COLOR, 38, 221, 188, 255);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the values for the intro scrollbox
void as_setupIntroScrollBox ()
//----------------------------------------------------------------------------------------------------------------------
{
	int introScrollBoxBorderWidth = 10;

	as_paraGui.create (GUI_OBJECT_SCREEN, "introScreen");

	as_paraGui.create           (GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox");
	as_paraGui.addToScreen      (GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", "introScreen");
	as_paraGui.setLabel         (GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", 4, GUI_LABEL_CENTER, gui_getString ("scrollBoxIntroText"));
	as_paraGui.setFontName      (GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", "introFont");
	as_paraGui.setPosition      (GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", 8, GUI_COORD_ABSOLUTE, introScrollBoxBorderWidth, 100, hiresVirtualWidth - introScrollBoxBorderWidth, hiresVirtualHeight - introScrollBoxBorderWidth - 100);
	as_paraGui.setColor         (GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", GUI_COL_ACTIVE, 100, 100, 15, 100);
	as_paraGui.setColor         (GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", GUI_COL_ACTIVE_LABEL, 20, 150, 100, 240);
	as_paraGui.setScrollSpeed   ("introScreen.scrollbox", 60.0);
	as_paraGui.setReady         (GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", true);

//	as_guiSetupScrollBox (SCROLLBOX_INTRO, gui_getString ("scrollBoxIntroText"));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Entry point to create the GUI
void as_createGUI ()
//----------------------------------------------------------------------------------------------------------------------
{
	as_setLanguageStrings ();

	sideviewDrawScale = 1.2f;
	as_setSideviewColors ();

	as_setupIntroScrollBox ();
	createMainMenu ();
	createOptionsMenu ();
	createVideoMenu ();
	createAudioMenu ();
	createControlsMenu ();
	createGameMenu ();

	as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "mainMenu"));
	as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "mainMenu.startGame");

	as_paraGui.setColor (GUI_OBJECT_BUTTON, "ALL", GUI_COL_ACTIVE, 50, 50, 200, 255);
	as_paraGui.setColor (GUI_OBJECT_BUTTON, "ALL", GUI_COL_INACTIVE, 50, 50, 100, 255);
	as_paraGui.setColor (GUI_OBJECT_BUTTON, "ALL", GUI_COL_ACTIVE_LABEL, 250, 250, 230, 255);
	as_paraGui.setColor (GUI_OBJECT_BUTTON, "ALL", GUI_COL_INACTIVE_LABEL, 150, 150, 130, 255);

	as_paraGui.setColor (GUI_OBJECT_SLIDER, "ALL", GUI_COL_ACTIVE, 50, 50, 200, 255);
	as_paraGui.setColor (GUI_OBJECT_SLIDER, "ALL", GUI_COL_INACTIVE, 50, 50, 100, 255);
	as_paraGui.setColor (GUI_OBJECT_SLIDER, "ALL", GUI_COL_ACTIVE_LABEL, 250, 250, 230, 255);
	as_paraGui.setColor (GUI_OBJECT_SLIDER, "ALL", GUI_COL_INACTIVE_LABEL, 150, 150, 130, 255);

	as_paraGui.setColor (GUI_OBJECT_CHECKBOX, "ALL", GUI_COL_ACTIVE, 50, 50, 200, 255);
	as_paraGui.setColor (GUI_OBJECT_CHECKBOX, "ALL", GUI_COL_INACTIVE, 50, 50, 100, 255);
	as_paraGui.setColor (GUI_OBJECT_CHECKBOX, "ALL", GUI_COL_ACTIVE_LABEL, 250, 250, 230, 255);
	as_paraGui.setColor (GUI_OBJECT_CHECKBOX, "ALL", GUI_COL_INACTIVE_LABEL, 150, 150, 130, 255);

	as_paraGui.setColor (GUI_OBJECT_LABEL, "ALL", GUI_COL_ACTIVE, 50, 50, 200, 255);
}
