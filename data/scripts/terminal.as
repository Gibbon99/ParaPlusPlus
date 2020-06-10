//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Process the elements on the deckview screen
void as_handleDeckviewScreen()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "deckviewScreen.backButton"))
	{
		gam_setHudText("terminalMenu.terminalText");
		sys_setNewMode (MODE_GUI_TERMINAL, true);
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "terminalMenu"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "terminalMenu.deckviewButton");
		return;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Process the elements on the shipview screen
void as_handleShipviewScreen()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "shipviewScreen.backButton"))
	{
		gam_setHudText("terminalMenu.terminalText");
		sys_setNewMode (MODE_GUI_TERMINAL, true);
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "terminalMenu"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "terminalMenu.shipviewButton");
		return;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Process the elements on the terminal menu
void as_handleTerminalMenu ()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "terminalMenu.databaseButton"))
	{
		gam_setHudText("terminalMenu.databaseButton");
		gam_prepareDatabaseScreen(0);
		as_paraGui.restartScrollBox("databaseScreen.scrollbox");
		sys_setNewMode(MODE_GUI_DATABASE, true);
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "databaseScreen"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "databaseScreen.backButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "terminalMenu.deckviewButton"))
	{
		gam_setHudText("terminalMenu.deckviewButton");
		sys_setNewMode(MODE_GUI_DECKVIEW, true);
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "deckviewScreen"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "deckviewScreen.backButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "terminalMenu.shipviewButton"))
	{
		gam_setHudText("terminalMenu.shipviewButton");
		sys_setNewMode(MODE_GUI_SHIPVIEW, true);
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "shipviewScreen"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "shipviewScreen.backButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "terminalMenu.logoffButton"))
	{
		//
		// Change mode back to game
		sys_addEvent (EVENT_TYPE_GAME, EVENT_ACTION_AUDIO_START_BACKGROUND, 0, "");
		sys_setNewMode(MODE_GAME, true);
		return;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Process the elements on the database screen
void as_handleDatabaseMenu ()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "databaseScreen.backButton"))
	{
		gam_setHudText("terminalMenu.terminalText");
		sys_setNewMode (MODE_GUI_TERMINAL, true);
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "terminalMenu"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "terminalMenu.databaseButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "databaseScreen.previousButton"))
	{
		//
		// Move to previous droid
		gam_previousDatabase();
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "databaseScreen.nextButton"))
	{
		//
		// Move to next droid
		gam_nextDatabase();
		return;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the screen to view the ship
void createShipViewScreen()
//----------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create (GUI_OBJECT_SCREEN, "shipviewScreen");

	as_paraGui.create (GUI_OBJECT_BUTTON, "shipviewScreen.backButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "shipviewScreen.backButton", "shipviewScreen");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "shipviewScreen.backButton", 3, GUI_LABEL_CENTER, gui_getString ("backButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "shipviewScreen.backButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "shipviewScreen.backButton", 8, GUI_COORD_PERCENT, 59, 85, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "shipviewScreen.backButton", "as_handleShipviewScreen");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "shipviewScreen.backButton", true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the screen to view the deck view
void createDeckViewScreen()
//----------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create (GUI_OBJECT_SCREEN, "deckviewScreen");

	as_paraGui.create (GUI_OBJECT_BUTTON, "deckviewScreen.backButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "deckviewScreen.backButton", "deckviewScreen");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "deckviewScreen.backButton", 3, GUI_LABEL_CENTER, gui_getString ("backButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "deckviewScreen.backButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "deckviewScreen.backButton", 8, GUI_COORD_PERCENT, 59, 85, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "deckviewScreen.backButton", "as_handleDeckviewScreen");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "deckviewScreen.backButton", true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the terminal menu screen
void createTerminalMenu ()
//----------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create (GUI_OBJECT_SCREEN, "terminalMenu");

	as_paraGui.create (GUI_OBJECT_IMAGE, "terminalMenu.backGround");
	as_paraGui.addToScreen (GUI_OBJECT_IMAGE, "terminalMenu.backGround", "terminalMenu");
	as_paraGui.setLabel (GUI_OBJECT_IMAGE, "terminalMenu.backGround", 0, GUI_LABEL_CENTER, "universe");
	as_paraGui.setPosition (GUI_OBJECT_IMAGE, "terminalMenu.backGround", 0, GUI_COORD_ABSOLUTE, 0, 0, 0, 0);
	as_paraGui.setReady (GUI_OBJECT_IMAGE, "terminalMenu.backGround", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "terminalMenu.databaseButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "terminalMenu.databaseButton", "terminalMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "terminalMenu.databaseButton", 3, GUI_LABEL_CENTER, gui_getString ("terminalMenu.databaseButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "terminalMenu.databaseButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "terminalMenu.databaseButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 1, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "terminalMenu.databaseButton", "as_handleTerminalMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "terminalMenu.databaseButton", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "terminalMenu.deckviewButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "terminalMenu.deckviewButton", "terminalMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "terminalMenu.deckviewButton", 3, GUI_LABEL_CENTER, gui_getString ("terminalMenu.deckviewButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "terminalMenu.deckviewButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "terminalMenu.deckviewButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 2, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "terminalMenu.deckviewButton", "as_handleTerminalMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "terminalMenu.deckviewButton", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "terminalMenu.shipviewButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "terminalMenu.shipviewButton", "terminalMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "terminalMenu.shipviewButton", 3, GUI_LABEL_CENTER, gui_getString ("terminalMenu.shipviewButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "terminalMenu.shipviewButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "terminalMenu.shipviewButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 3, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "terminalMenu.shipviewButton", "as_handleTerminalMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "terminalMenu.shipviewButton", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "terminalMenu.logoffButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "terminalMenu.logoffButton", "terminalMenu");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "terminalMenu.logoffButton", 3, GUI_LABEL_CENTER, gui_getString ("terminalMenu.logoffButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "terminalMenu.logoffButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "terminalMenu.logoffButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 4, buttonWidth, buttonHeight);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "terminalMenu.logoffButton", "as_handleTerminalMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "terminalMenu.logoffButton", true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the database view screen
void createDatabaseScreen ()
//----------------------------------------------------------------------------------------------------------------------
{
	int databaseScrollBoxBorderWidth = 10;

	as_paraGui.create (GUI_OBJECT_SCREEN, "databaseScreen");

	as_paraGui.create (GUI_OBJECT_SCROLLBOX, "databaseScreen.scrollbox");
	as_paraGui.addToScreen (GUI_OBJECT_SCROLLBOX, "databaseScreen.scrollbox", "databaseScreen");
	as_paraGui.setLabel (GUI_OBJECT_SCROLLBOX, "databaseScreen.scrollbox", 4, GUI_LABEL_CENTER, "Loading database information...");
	as_paraGui.setFontName (GUI_OBJECT_SCROLLBOX, "databaseScreen.scrollbox", "guiFont28");
	as_paraGui.setPosition (GUI_OBJECT_SCROLLBOX, "databaseScreen.scrollbox", 8, GUI_COORD_ABSOLUTE, databaseScrollBoxBorderWidth, 100,
	                        hiresVirtualWidth / 2, hiresVirtualHeight - databaseScrollBoxBorderWidth - 200);
	as_paraGui.setColor (GUI_OBJECT_SCROLLBOX, "databaseScreen.scrollbox", GUI_COL_ACTIVE, 100, 100, 15, 100);
	as_paraGui.setColor (GUI_OBJECT_SCROLLBOX, "databaseScreen.scrollbox", GUI_COL_ACTIVE_LABEL, 20, 150, 100, 240);
	as_paraGui.setScrollSpeed (GUI_OBJECT_SCROLLBOX, "databaseScreen.scrollbox", 30.0);
	as_paraGui.setReady (GUI_OBJECT_SCROLLBOX, "databaseScreen.scrollbox", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "databaseScreen.previousButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "databaseScreen.previousButton", "databaseScreen");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "databaseScreen.previousButton", 3, GUI_LABEL_CENTER, gui_getString ("databaseScreen.previousButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "databaseScreen.previousButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "databaseScreen.previousButton", 8, GUI_COORD_PERCENT, 1, 85, 25, 10);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "databaseScreen.previousButton", "as_handleDatabaseMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "databaseScreen.previousButton", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "databaseScreen.backButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "databaseScreen.backButton", "databaseScreen");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "databaseScreen.backButton", 3, GUI_LABEL_CENTER, gui_getString ("backButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "databaseScreen.backButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "databaseScreen.backButton", 8, GUI_COORD_PERCENT, 38, 85, 25, 10);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "databaseScreen.backButton", "as_handleDatabaseMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "databaseScreen.backButton", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "databaseScreen.nextButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "databaseScreen.nextButton", "databaseScreen");
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "databaseScreen.nextButton", 3, GUI_LABEL_CENTER, gui_getString ("databaseScreen.nextButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "databaseScreen.nextButton", "guiFont");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "databaseScreen.nextButton", 8, GUI_COORD_PERCENT, 74, 85, 25, 10);
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "databaseScreen.nextButton", "as_handleDatabaseMenu");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "databaseScreen.nextButton", true);
}
