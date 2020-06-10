int transferLabelStartX = 2;
int transferLabelStartY = 90;
int transferNextButtonX = 70;
int transferNextButtonY = 85;
int transferCancelButtonX = 5;
int transferCancelButtonY = 85;
int transferImageX = 50;
int transferImageY = 35;
float hudSize      = 80;
float tutImageSize = 128;

//-------------------------------------------------------------------------------------------------------------------------------------------
//
// Handle elements for the tutorial screens
void as_handleTutorialElements ()
//-------------------------------------------------------------------------------------------------------------------------------------------
{

//
// Start of the tutorial screens - how to move
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutorialCancel"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "mainMenu"));
		sys_setNewMode (MODE_GUI_MAINMENU, true);
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "mainMenu.startGameButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutMoveNext"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "scrTutTransfer"));
		sys_setNewMode (MODE_GUI_TUT_TRANSFER_START, true);
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransNext");
		return;
	}

//
// Tutorial on doing a transfer
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransCancel"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "mainMenu"));
		sys_setNewMode (MODE_GUI_MAINMENU, true);
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "mainMenu.startGameButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransNext"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "scrTutTransGame"));
		sys_setNewMode (MODE_GUI_TUT_TRANSFER_GAME, true);
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameNext");
		return;
	}
//
// Tutorial on transfer game
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameCancel"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "mainMenu"));
		sys_setNewMode (MODE_GUI_MAINMENU, true);
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "mainMenu.startGameButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameNext"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "scrTutLifts"));
		sys_setNewMode (MODE_GUI_TUT_LIFTS, true);
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftNext");
		return;
	}
//
// How to use the lifts
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftCancel"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "mainMenu"));
		sys_setNewMode (MODE_GUI_MAINMENU, true);
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "mainMenu.startGameButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftNext"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "scrTutTerminals"));
		sys_setNewMode (MODE_GUI_TUT_TERMINALS, true);
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalNext");
		return;
	}
//
// How to use the terminals
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalCancel"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "mainMenu"));
		sys_setNewMode (MODE_GUI_MAINMENU, true);
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "mainMenu.startGameButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalNext"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "scrTutHealing"));
		sys_setNewMode (MODE_GUI_TUT_HEALING, true);
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealinglNext");
		return;
	}
//
// How to use the healing tiles
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealingCancel"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "mainMenu"));
		sys_setNewMode (MODE_GUI_MAINMENU, true);
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "mainMenu.startGameButton");
		return;
	}

	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealinglNext"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "scrTutTips"));
		sys_setNewMode (MODE_GUI_TUT_TIPS, true);
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "scrTutTips.buttonTutTipsCancel");
		return;
	}
//
// Gameplay tips
	if (as_paraGui.getActiveObjectIndex () == as_paraGui.getIndex (GUI_OBJECT_BUTTON, "scrTutTips.buttonTutTipsCancel"))
	{
		as_paraGui.setCurrentScreen (as_paraGui.getIndex (GUI_OBJECT_SCREEN, "mainMenu"));
		sys_setNewMode (MODE_GUI_MAINMENU, true);
		as_paraGui.setActiveObject (as_paraGui.getCurrentScreen (), GUI_OBJECT_BUTTON, "mainMenu.startGameButton");
		return;
	}
}

//-------------------------------------------------------------------------------------------------------------------------------------------
//
// Setup tutorial screen
void createTutorialScreens ()
//-------------------------------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create (GUI_OBJECT_SCREEN, "scrTutorial");

	as_paraGui.create (GUI_OBJECT_LABEL, "scrTutorial.textLabelTutMove");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "scrTutorial.textLabelTutMove", "scrTutorial");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "scrTutorial.textLabelTutMove", 4, GUI_COORD_ABSOLUTE, transferLabelStartX, transferLabelStartY, 10, 10);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "scrTutorial.textLabelTutMove", 2, GUI_LABEL_CENTER, gui_getString ("scrTutorial.tutMoveLabel"));
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "scrTutorial.textLabelTutMove", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "scrTutorial.textLabelTutMove", true);

	as_paraGui.create (GUI_OBJECT_TEXTBOX, "scrTutorial.tutTextBox");
	as_paraGui.addToScreen (GUI_OBJECT_TEXTBOX, "scrTutorial.tutTextBox", "scrTutorial");
	as_paraGui.setPosition (GUI_OBJECT_TEXTBOX, "scrTutorial.tutTextBox", 4, GUI_COORD_PERCENT, 5, 35, 90, 90);
	as_paraGui.setLabel (GUI_OBJECT_TEXTBOX, "scrTutorial.tutTextBox", 2, GUI_LABEL_LEFT, gui_getString ("scrTutorial.tutMoveText"));
	as_paraGui.setFontName (GUI_OBJECT_TEXTBOX, "scrTutorial.tutTextBox", "guiFont28");
	as_paraGui.setReady (GUI_OBJECT_TEXTBOX, "scrTutorial.tutTextBox", true);


	as_paraGui.create (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutMoveNext");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutMoveNext", "scrTutorial");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutMoveNext", 4, GUI_COORD_PERCENT, transferNextButtonX, transferNextButtonY, 25, buttonHeight);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutMoveNext", 2, GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutMoveNext", "guiFont");
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutMoveNext", "as_handleTutorialElements");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutMoveNext", true);


	as_paraGui.create (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutorialCancel");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutorialCancel", "scrTutorial");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutorialCancel", 4, GUI_COORD_PERCENT, transferCancelButtonX, transferCancelButtonY, 25, buttonHeight);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutorialCancel", 2, GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutorialCancel", "as_handleTutorialElements");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutorialCancel", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "scrTutorial.buttonTutorialCancel", true);


	//
	// Screen how to start transfer mode
	//
	as_paraGui.create (GUI_OBJECT_SCREEN, "scrTutTransfer");

	as_paraGui.create (GUI_OBJECT_LABEL, "scrTutTransfer.textLabelTutTransMove");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "scrTutTransfer.textLabelTutTransMove", "scrTutTransfer");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "scrTutTransfer.textLabelTutTransMove", 4, GUI_COORD_ABSOLUTE, transferLabelStartX, transferLabelStartY, 10, 10);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "scrTutTransfer.textLabelTutTransMove", 2, GUI_LABEL_CENTER, gui_getString ("scrTutTransfer.tutTransferLabel"));
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "scrTutTransfer.textLabelTutTransMove", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "scrTutTransfer.textLabelTutTransMove", true);

	as_paraGui.create (GUI_OBJECT_TEXTBOX, "scrTutTransfer.tutTransTextBox");
	as_paraGui.addToScreen (GUI_OBJECT_TEXTBOX, "scrTutTransfer.tutTransTextBox", "scrTutTransfer");
	as_paraGui.setPosition (GUI_OBJECT_TEXTBOX, "scrTutTransfer.tutTransTextBox", 4, GUI_COORD_PERCENT, 5, 35, 90, 90);
	as_paraGui.setLabel (GUI_OBJECT_TEXTBOX, "scrTutTransfer.tutTransTextBox", 2, GUI_LABEL_LEFT, gui_getString ("scrTutTransfer.tutTransText"));
	as_paraGui.setFontName (GUI_OBJECT_TEXTBOX, "scrTutTransfer.tutTransTextBox", "guiFont28");
	as_paraGui.setReady (GUI_OBJECT_TEXTBOX, "scrTutTransfer.tutTransTextBox", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransNext");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransNext", "scrTutTransfer");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransNext", 4, GUI_COORD_PERCENT, transferNextButtonX, transferNextButtonY, 25, buttonHeight);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransNext", 2, GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransNext", "as_handleTutorialElements");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransNext", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransNext", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransCancel");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransCancel", "scrTutTransfer");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransCancel", 4, GUI_COORD_PERCENT, transferCancelButtonX, transferCancelButtonY, 25, buttonHeight);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransCancel", 2, GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransCancel", "as_handleTutorialElements");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransCancel", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "scrTutTransfer.buttonTutTransCancel", true);

	//
	// Screen explaining transfer game
	//
	as_paraGui.create (GUI_OBJECT_SCREEN, "scrTutTransGame");

	as_paraGui.create (GUI_OBJECT_LABEL, "scrTutTransGame.textLabelTutTransGame");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "scrTutTransGame.textLabelTutTransGame", "scrTutTransGame");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "scrTutTransGame.textLabelTutTransGame", 4, GUI_COORD_ABSOLUTE, transferLabelStartX, transferLabelStartY, 10, 10);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "scrTutTransGame.textLabelTutTransGame", 2, GUI_LABEL_CENTER, gui_getString ("scrTutTransGame.tutTransGameLabel"));
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "scrTutTransGame.textLabelTutTransGame", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "scrTutTransGame.textLabelTutTransGame", true);

	as_paraGui.create (GUI_OBJECT_TEXTBOX, "scrTutTransGame.tutTransGameTextBox");
	as_paraGui.addToScreen (GUI_OBJECT_TEXTBOX, "scrTutTransGame.tutTransGameTextBox", "scrTutTransGame");
	as_paraGui.setPosition (GUI_OBJECT_TEXTBOX, "scrTutTransGame.tutTransGameTextBox", 4, GUI_COORD_PERCENT, 5, 35, 90, 90);
	as_paraGui.setLabel (GUI_OBJECT_TEXTBOX, "scrTutTransGame.tutTransGameTextBox", 2, GUI_LABEL_LEFT, gui_getString ("scrTutTransGame.tutTransGameText"));
	as_paraGui.setFontName (GUI_OBJECT_TEXTBOX, "scrTutTransGame.tutTransGameTextBox", "guiFont28");
	as_paraGui.setReady (GUI_OBJECT_TEXTBOX, "scrTutTransGame.tutTransGameTextBox", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameNext");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameNext", "scrTutTransGame");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameNext", 4, GUI_COORD_PERCENT, transferNextButtonX, transferNextButtonY, 25, buttonHeight);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameNext", 2, GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameNext", "as_handleTutorialElements");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameNext", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameNext", true);


	as_paraGui.create (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameCancel");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameCancel", "scrTutTransGame");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameCancel", 4, GUI_COORD_PERCENT, transferCancelButtonX, transferCancelButtonY, 25, buttonHeight);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameCancel", 2, GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameCancel", "as_handleTutorialElements");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameCancel", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "scrTutTransGame.buttonTutTransGameCancel", true);


	//
	// Screen showing how to use lifts
	//
	as_paraGui.create (GUI_OBJECT_SCREEN, "scrTutLifts");

	as_paraGui.create (GUI_OBJECT_LABEL, "scrTutLifts.textLabelTutLifts");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "scrTutLifts.textLabelTutLifts", "scrTutLifts");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "scrTutLifts.textLabelTutLifts", 4, GUI_COORD_ABSOLUTE, transferLabelStartX, transferLabelStartY, 10, 10);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "scrTutLifts.textLabelTutLifts", 2, GUI_LABEL_CENTER, gui_getString ("scrTutLifts.tutLiftLabel"));
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "scrTutLifts.textLabelTutLifts", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "scrTutLifts.textLabelTutLifts", true);

	as_paraGui.create (GUI_OBJECT_TEXTBOX, "scrTutLifts.tutLiftTextBox");
	as_paraGui.addToScreen (GUI_OBJECT_TEXTBOX, "scrTutLifts.tutLiftTextBox", "scrTutLifts");
	as_paraGui.setPosition (GUI_OBJECT_TEXTBOX, "scrTutLifts.tutLiftTextBox", 4, GUI_COORD_PERCENT, 5, 50, 90, 90);
	as_paraGui.setLabel (GUI_OBJECT_TEXTBOX, "scrTutLifts.tutLiftTextBox", 2, GUI_LABEL_LEFT, gui_getString ("scrTutLifts.tutLiftText"));
	as_paraGui.setFontName (GUI_OBJECT_TEXTBOX, "scrTutLifts.tutLiftTextBox", "guiFont28");
	as_paraGui.setReady (GUI_OBJECT_TEXTBOX, "scrTutLifts.tutLiftTextBox", true);

	as_paraGui.create (GUI_OBJECT_IMAGE, "scrTutLifts.tutImageLift");
	as_paraGui.addToScreen (GUI_OBJECT_IMAGE, "scrTutLifts.tutImageLift", "scrTutLifts");
	as_paraGui.setLabel (GUI_OBJECT_IMAGE, "scrTutLifts.tutImageLift", 0, GUI_LABEL_CENTER, "tut_lift");
	as_paraGui.setPosition (GUI_OBJECT_IMAGE, "scrTutLifts.tutImageLift", 4, GUI_COORD_PERCENT, transferImageX, transferImageY, tutImageSize, tutImageSize);
	as_paraGui.setReady (GUI_OBJECT_IMAGE, "scrTutLifts.tutImageLift", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftNext");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftNext", "scrTutLifts");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftNext", 4, GUI_COORD_PERCENT, transferNextButtonX, transferNextButtonY, 25, buttonHeight);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftNext", 2, GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftNext", "as_handleTutorialElements");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftNext", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftNext", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftCancel");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftCancel", "scrTutLifts");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftCancel", 4, GUI_COORD_PERCENT, transferCancelButtonX, transferCancelButtonY, 25, buttonHeight);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftCancel", 2, GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftCancel", "as_handleTutorialElements");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftCancel", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "scrTutLifts.buttonTutLiftCancel", true);


	//
	// Screen showing terminal use
	//
	as_paraGui.create (GUI_OBJECT_SCREEN, "scrTutTerminals");

	as_paraGui.create (GUI_OBJECT_LABEL, "scrTutTerminals.textLabelTutTerminals");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "scrTutTerminals.textLabelTutTerminals", "scrTutTerminals");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "scrTutTerminals.textLabelTutTerminals", 4, GUI_COORD_ABSOLUTE, transferLabelStartX, transferLabelStartY, 10, 10);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "scrTutTerminals.textLabelTutTerminals", 2, GUI_LABEL_CENTER, gui_getString ("scrTutTerminals.tutTerminalLabel"));
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "scrTutTerminals.textLabelTutTerminals", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "scrTutTerminals.textLabelTutTerminals", true);

	as_paraGui.create (GUI_OBJECT_TEXTBOX, "scrTutTerminals.tutTerminalTextBox");
	as_paraGui.addToScreen (GUI_OBJECT_TEXTBOX, "scrTutTerminals.tutTerminalTextBox", "scrTutTerminals");
	as_paraGui.setPosition (GUI_OBJECT_TEXTBOX, "scrTutTerminals.tutTerminalTextBox", 4, GUI_COORD_PERCENT, 5, 50, 90, 90);
	as_paraGui.setLabel (GUI_OBJECT_TEXTBOX, "scrTutTerminals.tutTerminalTextBox", 2, GUI_LABEL_LEFT, gui_getString ("scrTutTerminals.tutTerminalText"));
	as_paraGui.setFontName (GUI_OBJECT_TEXTBOX, "scrTutTerminals.tutTerminalTextBox", "guiFont28");
	as_paraGui.setReady (GUI_OBJECT_TEXTBOX, "scrTutTerminals.tutTerminalTextBox", true);

	as_paraGui.create (GUI_OBJECT_IMAGE, "scrTutTerminals.tutImageTerminal");
	as_paraGui.addToScreen (GUI_OBJECT_IMAGE, "scrTutTerminals.tutImageTerminal", "scrTutTerminals");
	as_paraGui.setPosition (GUI_OBJECT_IMAGE, "scrTutTerminals.tutImageTerminal", 4, GUI_COORD_PERCENT, transferImageX, transferImageY, tutImageSize, tutImageSize);
	as_paraGui.setLabel (GUI_OBJECT_IMAGE, "scrTutTerminals.tutImageTerminal", 0, GUI_LABEL_CENTER, "tut_terminal");
	as_paraGui.setReady (GUI_OBJECT_IMAGE, "scrTutTerminals.tutImageTerminal", true);


	as_paraGui.create (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalNext");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalNext", "scrTutTerminals");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalNext", 4, GUI_COORD_PERCENT, transferNextButtonX, transferNextButtonY, 25, buttonHeight);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalNext", 2, GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalNext", "as_handleTutorialElements");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalNext", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalNext", true);


	as_paraGui.create (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalCancel");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalCancel", "scrTutTerminals");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalCancel", 4, GUI_COORD_PERCENT, transferCancelButtonX, transferCancelButtonY, 25, buttonHeight);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalCancel", 2, GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalCancel", "as_handleTutorialElements");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalCancel", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "scrTutTerminals.buttonTutTerminalCancel", true);


	//
	// Screen showing healing use
	//
	as_paraGui.create (GUI_OBJECT_SCREEN, "scrTutHealing");

	as_paraGui.create (GUI_OBJECT_LABEL, "scrTutHealing.textLabelTutHealing");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "scrTutHealing.textLabelTutHealing", "scrTutHealing");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "scrTutHealing.textLabelTutHealing", 4, GUI_COORD_ABSOLUTE, transferLabelStartX, transferLabelStartY, 10, 10);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "scrTutHealing.textLabelTutHealing", 2, GUI_LABEL_CENTER, gui_getString ("scrTutHealing.tutHealingLabel"));
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "scrTutHealing.textLabelTutHealing", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "scrTutHealing.textLabelTutHealing", true);


	as_paraGui.create (GUI_OBJECT_TEXTBOX, "scrTutHealing.tutHealingTextBox");
	as_paraGui.addToScreen (GUI_OBJECT_TEXTBOX, "scrTutHealing.tutHealingTextBox", "scrTutHealing");
	as_paraGui.setPosition (GUI_OBJECT_TEXTBOX, "scrTutHealing.tutHealingTextBox", 4, GUI_COORD_PERCENT, 5, 50, 90, 90);
	as_paraGui.setLabel (GUI_OBJECT_TEXTBOX, "scrTutHealing.tutHealingTextBox", 2, GUI_LABEL_LEFT, gui_getString ("scrTutHealing.tutHealingText"));
	as_paraGui.setFontName (GUI_OBJECT_TEXTBOX, "scrTutHealing.tutHealingTextBox", "guiFont28");
	as_paraGui.setReady (GUI_OBJECT_TEXTBOX, "scrTutHealing.tutHealingTextBox", true);


	as_paraGui.create (GUI_OBJECT_IMAGE, "scrTutHealing.tutImageHealing");
	as_paraGui.addToScreen (GUI_OBJECT_IMAGE, "scrTutHealing.tutImageHealing", "scrTutHealing");
	as_paraGui.setPosition (GUI_OBJECT_IMAGE, "scrTutHealing.tutImageHealing", 4, GUI_COORD_PERCENT, transferImageX, transferImageY, tutImageSize, tutImageSize);
	as_paraGui.setLabel (GUI_OBJECT_IMAGE, "scrTutHealing.tutImageHealing", 0, GUI_LABEL_CENTER, "tut_healing");
	as_paraGui.setReady (GUI_OBJECT_IMAGE, "scrTutHealing.tutImageHealing", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealinglNext");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealinglNext", "scrTutHealing");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealinglNext", 4, GUI_COORD_PERCENT, transferNextButtonX, transferNextButtonY, 25, buttonHeight);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealinglNext", 2, GUI_LABEL_CENTER, gui_getString ("nextButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealinglNext", "as_handleTutorialElements");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealinglNext", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealinglNext", true);


	as_paraGui.create (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealingCancel");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealingCancel", "scrTutHealing");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealingCancel", 4, GUI_COORD_PERCENT, transferCancelButtonX, transferCancelButtonY, 25, buttonHeight);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealingCancel", 2, GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealingCancel", "as_handleTutorialElements");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealingCancel", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "scrTutHealing.buttonTutHealingCancel", true);

	//
	// Screen showing some gameplay tips
	//
	as_paraGui.create (GUI_OBJECT_SCREEN, "scrTutTips");

	as_paraGui.create (GUI_OBJECT_LABEL, "scrTutTips.textLabelTutTips");
	as_paraGui.addToScreen (GUI_OBJECT_LABEL, "scrTutTips.textLabelTutTips", "scrTutTips");
	as_paraGui.setPosition (GUI_OBJECT_LABEL, "scrTutTips.textLabelTutTips", 4, GUI_COORD_ABSOLUTE, transferLabelStartX, transferLabelStartY, 10, 10);
	as_paraGui.setLabel (GUI_OBJECT_LABEL, "scrTutTips.textLabelTutTips", 2, GUI_LABEL_CENTER, gui_getString ("scrTutTips.tutTipsLabel"));
	as_paraGui.setFontName (GUI_OBJECT_LABEL, "scrTutTips.textLabelTutTips", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_LABEL, "scrTutTips.textLabelTutTips", true);

	as_paraGui.create (GUI_OBJECT_TEXTBOX, "scrTutTips.tutTipsTextBox");
	as_paraGui.addToScreen (GUI_OBJECT_TEXTBOX, "scrTutTips.tutTipsTextBox", "scrTutTips");
	as_paraGui.setPosition (GUI_OBJECT_TEXTBOX, "scrTutTips.tutTipsTextBox", 4, GUI_COORD_PERCENT, 5, 35, 90, 90);
	as_paraGui.setLabel (GUI_OBJECT_TEXTBOX, "scrTutTips.tutTipsTextBox", 2, GUI_LABEL_LEFT, gui_getString ("scrTutTips.tutTipsText"));
	as_paraGui.setFontName (GUI_OBJECT_TEXTBOX, "scrTutTips.tutTipsTextBox", "guiFont28");
	as_paraGui.setReady (GUI_OBJECT_TEXTBOX, "scrTutTips.tutTipsTextBox", true);

	as_paraGui.create (GUI_OBJECT_BUTTON, "scrTutTips.buttonTutTipsCancel");
	as_paraGui.addToScreen (GUI_OBJECT_BUTTON, "scrTutTips.buttonTutTipsCancel", "scrTutTips");
	as_paraGui.setPosition (GUI_OBJECT_BUTTON, "scrTutTips.buttonTutTipsCancel", 4, GUI_COORD_PERCENT, transferCancelButtonX, transferCancelButtonY, 25, buttonHeight);
	as_paraGui.setLabel (GUI_OBJECT_BUTTON, "scrTutTips.buttonTutTipsCancel", 2, GUI_LABEL_CENTER, gui_getString ("cancelButton"));
	as_paraGui.setAction (GUI_OBJECT_BUTTON, "scrTutTips.buttonTutTipsCancel", "as_handleTutorialElements");
	as_paraGui.setFontName (GUI_OBJECT_BUTTON, "scrTutTips.buttonTutTipsCancel", "guiFont");
	as_paraGui.setReady (GUI_OBJECT_BUTTON, "scrTutTips.buttonTutTipsCancel", true);
}