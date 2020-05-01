//--------------------------------------------------------------------------------------------------------------------
//
// " ! " - is a line break in a textbox = needs a space either side
//
// Create English text strings
void as_setEnglishText ()
//--------------------------------------------------------------------------------------------------------------------
{
	gui_addKeyAndText ("gameLeft", "Move left");
	gui_addKeyAndText ("gameRight", "Move Right");
	gui_addKeyAndText ("gameDown", "Move down");
	gui_addKeyAndText ("gameUp", "Move up");
	gui_addKeyAndText ("gameAction", "Perform an action");
	gui_addKeyAndText ("gamePause", "Pause the game");
	gui_addKeyAndText ("gameEscape", "Escape");
	gui_addKeyAndText ("consoleAction", "Invoke the console");
	gui_addKeyAndText ("gameScreenShot", "Take a screenshot");

	gui_addKeyAndText ("mainMenu.startGameButton", "Start Game");
	gui_addKeyAndText ("mainMenu.optionsButton", "Options");
	gui_addKeyAndText ("mainMenu.tutorialButton", "Tutorial");
	gui_addKeyAndText ("mainMenu.briefingButton", "Briefing");
	gui_addKeyAndText ("mainMenu.quitButton", "Quit Game");
}

