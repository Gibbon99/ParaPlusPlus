//----------------------------------------------------------------------------------------------------------------------
//
// Create the main menu
void createMainMenu()
//----------------------------------------------------------------------------------------------------------------------
{
	as_paraGui.create(GUI_OBJECT_SCREEN, "mainMenu");
	as_paraGui.create(GUI_OBJECT_BUTTON, "mainMenu.quitButton");
	as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "mainMenu.quitButton", "mainMenu");
	as_paraGui.setLabel(GUI_OBJECT_BUTTON, "mainMenu.quitButton", 3, GUI_LABEL_CENTER, "Quit Button");
	as_paraGui.setFontName(GUI_OBJECT_BUTTON, "mainMenu.quitButton", "guiFont");
	as_paraGui.setPosition(GUI_OBJECT_BUTTON, "mainMenu.quitButton", 8, GUI_COORD_ABSOLUTE, 100, 2, 295, 48);
	as_paraGui.setAction(GUI_OBJECT_BUTTON, "mainMenu.quitButton", "TODO-Function");

	as_paraGui.create(GUI_OBJECT_SCREEN, "mainMenu");
	as_paraGui.create(GUI_OBJECT_BUTTON, "mainMenu.startGame");
	as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "mainMenu.startGame", "mainMenu");
	as_paraGui.setLabel(GUI_OBJECT_BUTTON, "mainMenu.startGame", 3, GUI_LABEL_CENTER, "Start Game");
	as_paraGui.setFontName(GUI_OBJECT_BUTTON, "mainMenu.startGame", "guiFont");
	as_paraGui.setPosition(GUI_OBJECT_BUTTON, "mainMenu.startGame", 8, GUI_COORD_ABSOLUTE, 100, 60, 295, 59);
	as_paraGui.setAction(GUI_OBJECT_BUTTON, "mainMenu.startGame", "TODO-Function");

	as_paraGui.setColor (GUI_OBJECT_BUTTON, "ALL", GUI_COL_ACTIVE, 50, 50, 200, 255);
	as_paraGui.setColor (GUI_OBJECT_BUTTON, "ALL", GUI_COL_INACTIVE, 50, 50, 100, 255);
	as_paraGui.setColor (GUI_OBJECT_BUTTON, "ALL", GUI_COL_ACTIVE_LABEL, 250, 250, 230, 255);
	as_paraGui.setColor (GUI_OBJECT_BUTTON, "ALL", GUI_COL_INACTIVE_LABEL, 150, 150, 130, 255);
	as_paraGui.setReady(GUI_OBJECT_BUTTON, "mainMenu.quitButton", true);
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "mainMenu.startGame", true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Entry point to create the GUI
void as_createGUI()
//----------------------------------------------------------------------------------------------------------------------
{
	createMainMenu();
}
