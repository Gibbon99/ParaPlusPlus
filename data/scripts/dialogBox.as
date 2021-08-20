//-------------------------------------------------------------------------------------------------------------------
//
// Handle the actions for dialog boxes
void as_handleDialogBoxes ()
//-------------------------------------------------------------------------------------------------------------------
{
	if (as_paraGui.getActiveObjectIndexDialogbox () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "quitGameDialogbox.denyButton"))
	{
		// Back to game
		as_paraGui.setCurrentDialogbox (NO_DIALOG_BOX);
		return;
	}

	if (as_paraGui.getActiveObjectIndexDialogbox () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "quitGameDialogbox.confirmButton"))
	{
		// Destroy player
		gam_setHudText ("hudMainMenu");
		as_paraGui.setCurrentDialogbox (NO_DIALOG_BOX);
		as_paraDroid.setCurrentHealth(-200);
		return;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//
// Create a dialogbox for quit game confirmation
void as_createQuitGameDialogbox ()
//-------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create (GUI_OBJECT_DIALOGBOX, "quitGameDialogbox");
	as_paraGui.setPosition (GUI_OBJECT_DIALOGBOX, "quitGameDialogbox", 4, GUI_COORD_ABSOLUTE, 60, 130, 685, 250);
	as_paraGui.setLabel (GUI_OBJECT_DIALOGBOX, "quitGameDialogbox", 5, GUI_LABEL_CENTER, gui_getString ("quitGameDialogbox"));
	as_paraGui.setFontName (GUI_OBJECT_DIALOGBOX, "quitGameDialogbox", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_DIALOGBOX, "quitGameDialogbox", true);

	as_paraGui.create (GUI_OBJECT_TEXTBOX, "quitGameDialogbox.text");
	as_paraGui.addToDialogbox (GUI_OBJECT_TEXTBOX, "quitGameDialogbox.text", "quitGameDialogbox");
	as_paraGui.setPosition (GUI_OBJECT_TEXTBOX, "quitGameDialogbox.text", 4, GUI_COORD_ABSOLUTE, 65, 190, 663, 100);
	as_paraGui.setLabel (GUI_OBJECT_TEXTBOX, "quitGameDialogbox.text", 2, GUI_LABEL_CENTER, gui_getString ("quitGameDialogbox.text"));
	as_paraGui.setFontName (GUI_OBJECT_TEXTBOX, "quitGameDialogbox.text", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_TEXTBOX, "quitGameDialogbox.text", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "quitGameDialogbox.confirmButton");
	as_paraGui.addToDialogbox (GUI_OBJECT_BUTTON, "quitGameDialogbox.confirmButton", "quitGameDialogbox");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "quitGameDialogbox.confirmButton", 4, GUI_COORD_ABSOLUTE, 85, 300, 205, 60);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "quitGameDialogbox.confirmButton", 2, GUI_LABEL_CENTER, gui_getString ("quitGameDialogbox.confirmButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "quitGameDialogbox.confirmButton", "as_handleDialogBoxes");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "quitGameDialogbox.confirmButton", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "quitGameDialogbox.confirmButton", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "quitGameDialogbox.denyButton");
	as_paraGui.addToDialogbox (GUI_OBJECT_BUTTON, "quitGameDialogbox.denyButton", "quitGameDialogbox");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "quitGameDialogbox.denyButton", 4, GUI_COORD_ABSOLUTE, 515, 300, 205, 60);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "quitGameDialogbox.denyButton", 2, GUI_LABEL_CENTER, gui_getString ("quitGameDialogbox.denyButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "quitGameDialogbox.denyButton", "as_handleDialogBoxes");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "quitGameDialogbox.denyButton", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "quitGameDialogbox.denyButton", true);
}
