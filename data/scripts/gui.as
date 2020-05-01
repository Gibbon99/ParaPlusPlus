//----------------------------------------------------------------------------------------------------------------------
//
// Create the main menu
void createMainMenu()
//----------------------------------------------------------------------------------------------------------------------
{
	int buttonWidth = 40;
	int buttonHeight = 10;
	int buttonSpacing = 13;
	int buttonStartX = 10;

	as_paraGui.create(GUI_OBJECT_SCREEN, "mainMenu");

	as_paraGui.create(GUI_OBJECT_BUTTON, "mainMenu.startGameButton");
	as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "mainMenu.startGameButton", "mainMenu");
	as_paraGui.setLabel(GUI_OBJECT_BUTTON, "mainMenu.startGameButton", 3, GUI_LABEL_CENTER, gui_getString("mainMenu.startGameButton"));
	as_paraGui.setFontName(GUI_OBJECT_BUTTON, "mainMenu.startGameButton", "guiFont");
	as_paraGui.setPosition(GUI_OBJECT_BUTTON, "mainMenu.startGameButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 1, buttonWidth, buttonHeight);
	as_paraGui.setAction(GUI_OBJECT_BUTTON, "mainMenu.startGameButton", "START GAME -Function");

	as_paraGui.create(GUI_OBJECT_BUTTON, "mainMenu.optionsButton");
	as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "mainMenu.optionsButton", "mainMenu");
	as_paraGui.setLabel(GUI_OBJECT_BUTTON, "mainMenu.optionsButton", 3, GUI_LABEL_CENTER, gui_getString("mainMenu.optionsButton"));
	as_paraGui.setFontName(GUI_OBJECT_BUTTON, "mainMenu.optionsButton", "guiFont");
	as_paraGui.setPosition(GUI_OBJECT_BUTTON, "mainMenu.optionsButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 2, buttonWidth, buttonHeight);
	as_paraGui.setAction(GUI_OBJECT_BUTTON, "mainMenu.optionsButton", "OPTIONS -Function");

	as_paraGui.create(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton");
	as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", "mainMenu");
	as_paraGui.setLabel(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", 3, GUI_LABEL_CENTER, gui_getString("mainMenu.tutorialButton"));
	as_paraGui.setFontName(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", "guiFont");
	as_paraGui.setPosition(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 3, buttonWidth, buttonHeight);
	as_paraGui.setAction(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", "TUTORIAL -Function");

	as_paraGui.create(GUI_OBJECT_BUTTON, "mainMenu.briefingButton");
	as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "mainMenu.briefingButton", "mainMenu");
	as_paraGui.setLabel(GUI_OBJECT_BUTTON, "mainMenu.briefingButton", 3, GUI_LABEL_CENTER, gui_getString("mainMenu.briefingButton"));
	as_paraGui.setFontName(GUI_OBJECT_BUTTON, "mainMenu.briefingButton", "guiFont");
	as_paraGui.setPosition(GUI_OBJECT_BUTTON, "mainMenu.briefingButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 4, buttonWidth, buttonHeight);
	as_paraGui.setAction(GUI_OBJECT_BUTTON, "mainMenu.briefingButton", "BRIEFING -Function");

	as_paraGui.create(GUI_OBJECT_BUTTON, "mainMenu.quitButton");
	as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "mainMenu.quitButton", "mainMenu");
	as_paraGui.setLabel(GUI_OBJECT_BUTTON, "mainMenu.quitButton", 3, GUI_LABEL_CENTER, gui_getString("mainMenu.quitButton"));
	as_paraGui.setFontName(GUI_OBJECT_BUTTON, "mainMenu.quitButton", "guiFont");
	as_paraGui.setPosition(GUI_OBJECT_BUTTON, "mainMenu.quitButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 5, buttonWidth, buttonHeight);
	as_paraGui.setAction(GUI_OBJECT_BUTTON, "mainMenu.quitButton", "QUIT -Function");

	as_paraGui.setColor (GUI_OBJECT_BUTTON, "ALL", GUI_COL_ACTIVE, 50, 50, 200, 255);
	as_paraGui.setColor (GUI_OBJECT_BUTTON, "ALL", GUI_COL_INACTIVE, 50, 50, 100, 255);
	as_paraGui.setColor (GUI_OBJECT_BUTTON, "ALL", GUI_COL_ACTIVE_LABEL, 250, 250, 230, 255);
	as_paraGui.setColor (GUI_OBJECT_BUTTON, "ALL", GUI_COL_INACTIVE_LABEL, 150, 150, 130, 255);

	as_paraGui.setReady(GUI_OBJECT_BUTTON, "mainMenu.startGameButton", true);
	as_paraGui.setReady(GUI_OBJECT_BUTTON, "mainMenu.optionsButton", true);
	as_paraGui.setReady(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", true);
	as_paraGui.setReady(GUI_OBJECT_BUTTON, "mainMenu.briefingButton", true);
	as_paraGui.setReady(GUI_OBJECT_BUTTON, "mainMenu.quitButton", true);

}

//----------------------------------------------------------------------------------------------------------------------
//
// Entry point to create the GUI
void as_createGUI()
//----------------------------------------------------------------------------------------------------------------------
{
	as_setLanguageStrings();

	createMainMenu();
}
