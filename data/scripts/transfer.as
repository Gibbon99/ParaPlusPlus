//-------------------------------------------------------------------------------------------------------------------
//
// Handle input for transfer screens
void as_handleTransferScreens ()
//-------------------------------------------------------------------------------------------------------------------
{
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "guiTransferOne.nextButton"))
	{
		// Change to second transfer screen

		sys_setNewMode(MODE_TRANSFER_PRE_SCREEN_TWO, false);
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "guiTransferTwo"));
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "guiTransferTwo.nextButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "guiTransferTwo.nextButton"))
	{
		// Change to transfer screen
		return;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the first transfer screen
void as_setupTransferOne ()
//----------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create (GUI_OBJECT_SCREEN, "guiTransferOne");

	as_paraGui.create (GUI_OBJECT_LABEL, "guiTransferOne.heading");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "guiTransferOne.heading", "guiTransferOne");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "guiTransferOne.heading", 4, GUI_COORD_PERCENT, 5, 20, 10, 10);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "guiTransferOne.heading", 2, GUI_LABEL_CENTER, gui_getString ("guiTransferOne.heading"));
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "guiTransferOne.heading", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "guiTransferOne.heading", true);

	as_paraGui.create (GUI_OBJECT_TEXTBOX, "guiTransferOne.textbox");
	as_paraGui.addToScreen (GUI_OBJECT_TEXTBOX, "guiTransferOne.textbox", "guiTransferOne");
	as_paraGui.setPosition (GUI_OBJECT_TEXTBOX, "guiTransferOne.textbox", 4, GUI_COORD_PERCENT, 5, 35, 90, 90);
	as_paraGui.setLabel (GUI_OBJECT_TEXTBOX, "guiTransferOne.textbox", 2, GUI_LABEL_LEFT, gui_getString ("guiTransferOne.textbox"));
	as_paraGui.setFontName (GUI_OBJECT_TEXTBOX, "guiTransferOne.textbox", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_TEXTBOX, "guiTransferOne.textbox", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "guiTransferOne.nextButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "guiTransferOne.nextButton", "guiTransferOne");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "guiTransferOne.nextButton", 4, GUI_COORD_PERCENT, 74, 85, 25, 10);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "guiTransferOne.nextButton", 2, GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "guiTransferOne.nextButton", "as_handleTransferScreens");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "guiTransferOne.nextButton", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "guiTransferOne.nextButton", true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the second transfer screen
void as_setupTransferTwo ()
//----------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create (GUI_OBJECT_SCREEN, "guiTransferTwo");

	as_paraGui.create (GUI_OBJECT_LABEL, "guiTransferTwo.heading");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "guiTransferTwo.heading", "guiTransferTwo");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "guiTransferTwo.heading", 4, GUI_COORD_PERCENT, 5, 20, 10, 10);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "guiTransferTwo.heading", 2, GUI_LABEL_CENTER, gui_getString ("guiTransferTwo.heading"));
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "guiTransferTwo.heading", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "guiTransferTwo.heading", true);

	as_paraGui.create (GUI_OBJECT_TEXTBOX, "guiTransferTwo.textbox");
	as_paraGui.addToScreen (GUI_OBJECT_TEXTBOX, "guiTransferTwo.textbox", "guiTransferTwo");
	as_paraGui.setPosition (GUI_OBJECT_TEXTBOX, "guiTransferTwo.textbox", 4, GUI_COORD_PERCENT, 5, 35, 90, 90);
	as_paraGui.setLabel (GUI_OBJECT_TEXTBOX, "guiTransferTwo.textbox", 2, GUI_LABEL_LEFT, gui_getString ("guiTransferTwo.textbox"));
	as_paraGui.setFontName (GUI_OBJECT_TEXTBOX, "guiTransferTwo.textbox", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_TEXTBOX, "guiTransferTwo.textbox", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "guiTransferTwo.nextButton");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "guiTransferTwo.nextButton", "guiTransferTwo");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "guiTransferTwo.nextButton", 4, GUI_COORD_PERCENT, 74, 85, 25, 10);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "guiTransferTwo.nextButton", 2, GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "guiTransferTwo.nextButton", "as_handleTransferScreens");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "guiTransferTwo.nextButton", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "guiTransferTwo.nextButton", true);
}

//-------------------------------------------------------------------------------------------------------------------
//
// Setup the variables for the transfer screen
void as_initTransferValues ()
//-------------------------------------------------------------------------------------------------------------------
{
	numberTransferRows   = 12;
	transferBitmapWidth  = 800;
	transferBitmapHeight = 600;

	as_setupTransferOne ();
	as_setupTransferTwo ();
}