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
		quitProgram = 1;
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
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "audioMenu"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "audioMenu.backButton");
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
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "audioMenu.backButton"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "optionsMenu"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "optionsMenu.audioButton");
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
	as_paraGui.create (GUI_OBJECT_SCREEN, "videoMenu");

	as_paraGui.create (GUI_OBJECT_BUTTON, "videoMenu.backButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "videoMenu.backButton", "videoMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "videoMenu.backButton", 3, GUI_LABEL_CENTER, gui_getString ("backButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "videoMenu.backButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "videoMenu.backButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 5, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "videoMenu.backButton", "as_handleVideoMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "videoMenu.backButton", true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
// Create the Audio options menu. Checkbox: enable sound; Slider: Volume; Slider: numChannels ( 1, 2)
void createAudioMenu ()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create (GUI_OBJECT_SCREEN, "audioMenu");

	as_paraGui.create (GUI_OBJECT_LABEL, "audioMenu.heading");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "audioMenu.heading", "audioMenu");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "audioMenu.heading", 0, GUI_COORD_PERCENT, 2, 2, 20, 20);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "audioMenu.heading", 0, GUI_LABEL_LEFT, "Audio Options");
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "audioMenu.heading", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "audioMenu.heading", true);

	as_paraGui.create (GUI_OBJECT_SLIDER, "audioMenu.volume");
	as_paraGui.addToScreen (GUI_OBJECT_SLIDER, "audioMenu.volume", "audioMenu");
	as_paraGui.setPosition (GUI_OBJECT_SLIDER, "audioMenu.volume", 8, GUI_COORD_PERCENT, 1, 50, 40, 2);
	as_paraGui.setLabel (GUI_OBJECT_SLIDER, "audioMenu.volume", 3, GUI_LABEL_CENTER, gui_getString ("audioMenu.volume"));

	string tempNum;

	for (int i = 0; i != 10; i++)
	{
		tempNum = formatInt (i + 1, "l");
		as_paraGui.addNewElement ("audioMenu.volume", tempNum, tempNum, SLIDER_TYPE_INT);
	}

//	as_paraGui.setSliderValue ("optionsAudioVolume", volumeLevelStr);

	as_paraGui.setReady (GUI_OBJECT_SLIDER, "audioMenu.volume", true);
	as_paraGui.setFontName (GUI_OBJECT_SLIDER, "audioMenu.volume", "guiFont");

	as_paraGui.create (GUI_OBJECT_BUTTON, "audioMenu.backButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "audioMenu.backButton", "audioMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "audioMenu.backButton", 3, GUI_LABEL_CENTER, gui_getString ("backButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "audioMenu.backButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "audioMenu.backButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 5, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "audioMenu.backButton", "as_handleAudioMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "audioMenu.backButton", true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Create the Controls menu
void createControlsMenu ()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create (GUI_OBJECT_SCREEN, "controlsMenu");

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
// Entry point to create the GUI
void as_createGUI ()
//----------------------------------------------------------------------------------------------------------------------
{
	as_setLanguageStrings ();

	sideviewDrawScale = 1.2f;
	as_setSideviewColors ();

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

	as_paraGui.setColor (GUI_OBJECT_LABEL, "ALL", GUI_COL_ACTIVE, 50, 50, 200, 255);
}
