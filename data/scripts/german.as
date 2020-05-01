//--------------------------------------------------------------------------------------------------------------------
//
// " ! " - is a line break in a textbox = needs a space either side
//
// Create German text strings
void as_setGermanText ()
//--------------------------------------------------------------------------------------------------------------------
{
	gui_addKeyAndText("gameLeft", "Geh nach links");
	gui_addKeyAndText("gameRight", "Nach rechts bewegen");
	gui_addKeyAndText("gameDown", "Sich abwärts bewegen");
	gui_addKeyAndText("gameUp", "Bewegen Sie sich nach oben");
	gui_addKeyAndText("gameAction", "Führen Sie eine Aktion aus");
	gui_addKeyAndText("gamePause", "Unterbrechen Sie das Spiel");
	gui_addKeyAndText("gameEscape", "Flucht");
	gui_addKeyAndText("consoleAction", "Rufen Sie die Konsole auf");
	gui_addKeyAndText("gameScreenShot", "Mach ein Bildschirmfoto");

	gui_addKeyAndText ("mainMenu.startGameButton", "Spiel starten");
	gui_addKeyAndText ("mainMenu.optionsButton", "Optionen");
	gui_addKeyAndText ("mainMenu.tutorialButton", "Tutorial");
	gui_addKeyAndText ("mainMenu.briefingButton", "Anweisung");
	gui_addKeyAndText ("mainMenu.quitButton", "Spiel verlassen");
}