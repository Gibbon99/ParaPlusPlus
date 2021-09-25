//----------------------------------------------------------------------------------------------------------------------
//
// Angelscript - Setup and handle the highscore table
//
//----------------------------------------------------------------------------------------------------------------------

int headingStartX = 10;
int headingSpacingY = 16;
int elementSpacingY = 16;
int rowPositionStartY = 30;
int rowPositionSpaceY = 8;
int scorePositionStartX = 30;
int namePositionStartX = 60;
string labelNameNum = "highScoreDisplay.row";
string labelNameScore = "highScoreDisplay.score";
string labelNameName = "highScoreDisplay.name";
string rowIndexStr;

int initialSliderWidth = 15;
int initialSliderSpacing = 24;
int initialSliderHeight = 2;
int initialSliderStartX = 20;

string validCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890. -";

//----------------------------------------------------------------------------------------------------------------------
//
// Update the values in the GUI labels from the current score table
void as_refreshHighscoreLabels()
//----------------------------------------------------------------------------------------------------------------------
{
    for (int i = 0; i != NUM_HIGHSCORE_ROWS; i++ )
    {
        rowIndexStr = formatInt(i + 1, "l");
        as_paraGui.setLabel(GUI_OBJECT_LABEL, labelNameScore + rowIndexStr, 0, GUI_LABEL_LEFT, gui_getHighScoreValueByIndex(i));
        as_paraGui.setLabel(GUI_OBJECT_LABEL, labelNameName + rowIndexStr, 0, GUI_LABEL_LEFT, gui_getHighScoreNameByIndex(i));
    }
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process input from highscore screen
void as_handleHighscoreDisplay()
//----------------------------------------------------------------------------------------------------------------------
{
    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "highScoreDisplay.backButton"))
    {
        gam_setHudText("hudMainMenu");
        as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "mainMenu"));
        as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), GUI_OBJECT_BUTTON, "mainMenu.startGameButton");
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "highScoreEntry.backButton"))
    {
        string playerName = "";

        playerName = as_paraGui.getSliderValue("highScoreEntry.initial0");
        playerName += as_paraGui.getSliderValue("highScoreEntry.initial1");
        playerName += as_paraGui.getSliderValue("highScoreEntry.initial2");

        gui_insertNewScore(playerName);

        as_refreshHighscoreLabels();
        //
        // Display the high score table
        gui_showHighscoreTable();
        return;
    }
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the screen to enter high score initials
void createHighscoreEntry()
//----------------------------------------------------------------------------------------------------------------------
{
    as_paraGui.create(GUI_OBJECT_SCREEN, "highScoreEntry");


    as_paraGui.create(GUI_OBJECT_IMAGE, "highScoreEntry.backGround");
    as_paraGui.addToScreen(GUI_OBJECT_IMAGE, "highScoreEntry.backGround", "highScoreEntry");
    as_paraGui.setLabel(GUI_OBJECT_IMAGE, "highScoreEntry.backGround", 0, GUI_LABEL_CENTER, "universe");
    as_paraGui.setPosition(GUI_OBJECT_IMAGE, "highScoreEntry.backGround", 0, GUI_COORD_ABSOLUTE, 0, 0, 0, 0);
    as_paraGui.setReady(GUI_OBJECT_IMAGE, "highScoreEntry.backGround", true);

    as_paraGui.create(GUI_OBJECT_LABEL, "highScoreEntry.achieved");
    as_paraGui.addToScreen(GUI_OBJECT_LABEL, "highScoreEntry.achieved", "highScoreEntry");
    as_paraGui.setPosition(GUI_OBJECT_LABEL, "highScoreEntry.achieved", 8, GUI_COORD_PERCENT, headingStartX, headingSpacingY, 40, 10);
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "highScoreEntry.achieved", 0, GUI_LABEL_LEFT, gui_getString("highScoreEntry.achieved"));
    as_paraGui.setFontName(GUI_OBJECT_LABEL, "highScoreEntry.achieved", "guiFont");
    as_paraGui.setReady(GUI_OBJECT_LABEL, "highScoreEntry.achieved", true);

    as_paraGui.create(GUI_OBJECT_LABEL, "highScoreEntry.initials");
    as_paraGui.addToScreen(GUI_OBJECT_LABEL, "highScoreEntry.initials", "highScoreEntry");
    as_paraGui.setPosition(GUI_OBJECT_LABEL, "highScoreEntry.initials", 8, GUI_COORD_PERCENT, headingStartX, headingSpacingY * 2, 40, 10);
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "highScoreEntry.initials", 0, GUI_LABEL_LEFT, gui_getString("highScoreEntry.initials"));
    as_paraGui.setFontName(GUI_OBJECT_LABEL, "highScoreEntry.initials", "guiFont");
    as_paraGui.setReady(GUI_OBJECT_LABEL, "highScoreEntry.initials", true);

    for (int i = 0;
    i != 3;
    i++
)
    {
        rowIndexStr = formatInt(i, "l");

        as_paraGui.create(GUI_OBJECT_SLIDER, "highScoreEntry.initial" + rowIndexStr);
        as_paraGui.addToScreen(GUI_OBJECT_SLIDER, "highScoreEntry.initial" + rowIndexStr, "highScoreEntry");
        as_paraGui.setPosition(GUI_OBJECT_SLIDER, "highScoreEntry.initial" + rowIndexStr, 8, GUI_COORD_PERCENT, initialSliderStartX + (initialSliderSpacing * i), headingSpacingY * 4, initialSliderWidth, initialSliderHeight);
        as_paraGui.setLabel(GUI_OBJECT_SLIDER, "highScoreEntry.initial" + rowIndexStr, 12, GUI_LABEL_CENTER, gui_getString("emptyString"));
        as_paraGui.setAction(GUI_OBJECT_SLIDER, "highScoreEntry.initial" + rowIndexStr, "");

        for (uint charIndex = 0;
        charIndex != validCharacters.length();
        charIndex++
    )
        {
            as_paraGui.addNewElement("highScoreEntry.initial" + rowIndexStr, validCharacters.substr(charIndex, 1), validCharacters.substr(charIndex, 1), SLIDER_TYPE_STRING);
        }

//    as_paraGui.setSliderValue("highScoreEntry.volume", currentVolumeStr);
        as_paraGui.setFontName(GUI_OBJECT_SLIDER, "highScoreEntry.initial" + rowIndexStr, "guiFont");
        as_paraGui.setAction(GUI_OBJECT_SLIDER, "highScoreEntry.initial" + rowIndexStr, "as_handleHighscoreDisplay");
        as_paraGui.setReady(GUI_OBJECT_SLIDER, "highScoreEntry.initial" + rowIndexStr, true);
    }

    as_paraGui.create(GUI_OBJECT_BUTTON, "highScoreEntry.backButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "highScoreEntry.backButton", "highScoreEntry");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "highScoreEntry.backButton", 3, GUI_LABEL_CENTER, gui_getString("backButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "highScoreEntry.backButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "highScoreEntry.backButton", 8, GUI_COORD_PERCENT, headingStartX, elementSpacingY * 5, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "highScoreEntry.backButton", "as_handleHighscoreDisplay");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "highScoreEntry.backButton", true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the screen to display the highscore table
void createHighscoreDisplay()
//----------------------------------------------------------------------------------------------------------------------
{

    int
    rowCounterInt;

    as_paraGui.create(GUI_OBJECT_SCREEN, "highScoreDisplay");

    as_paraGui.create(GUI_OBJECT_IMAGE, "highScoreDisplay.backGround");
    as_paraGui.addToScreen(GUI_OBJECT_IMAGE, "highScoreDisplay.backGround", "highScoreDisplay");
    as_paraGui.setLabel(GUI_OBJECT_IMAGE, "highScoreDisplay.backGround", 0, GUI_LABEL_CENTER, "universe");
    as_paraGui.setPosition(GUI_OBJECT_IMAGE, "highScoreDisplay.backGround", 0, GUI_COORD_ABSOLUTE, 0, 0, 0, 0);
    as_paraGui.setReady(GUI_OBJECT_IMAGE, "highScoreDisplay.backGround", true);

    as_paraGui.create(GUI_OBJECT_LABEL, "highScoreDisplay.heading");
    as_paraGui.addToScreen(GUI_OBJECT_LABEL, "highScoreDisplay.heading", "highScoreDisplay");
    as_paraGui.setPosition(GUI_OBJECT_LABEL, "highScoreDisplay.heading", 8, GUI_COORD_PERCENT, headingStartX, headingSpacingY, 40, 10);
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "highScoreDisplay.heading", 0, GUI_LABEL_LEFT, gui_getString("highScoreDisplay.heading"));
    as_paraGui.setFontName(GUI_OBJECT_LABEL, "highScoreDisplay.heading", "guiFont");
    as_paraGui.setReady(GUI_OBJECT_LABEL, "highScoreDisplay.heading", true);

    for (int i = 0;
    i != NUM_HIGHSCORE_ROWS;
    i++
)
    {
        rowIndexStr = formatInt(i + 1, "l");

        as_paraGui.create(GUI_OBJECT_LABEL, labelNameNum + rowIndexStr);
        as_paraGui.addToScreen(GUI_OBJECT_LABEL, labelNameNum + rowIndexStr, "highScoreDisplay");
        as_paraGui.setPosition(GUI_OBJECT_LABEL, labelNameNum + rowIndexStr, 8, GUI_COORD_PERCENT, headingStartX, rowPositionStartY + (rowPositionSpaceY * i), 40, 10);
        as_paraGui.setLabel(GUI_OBJECT_LABEL, labelNameNum + rowIndexStr, 0, GUI_LABEL_LEFT, rowIndexStr);
        as_paraGui.setFontName(GUI_OBJECT_LABEL, labelNameNum + rowIndexStr, "guiFont");
        as_paraGui.setReady(GUI_OBJECT_LABEL, labelNameNum + rowIndexStr, true);

        as_paraGui.create(GUI_OBJECT_LABEL, labelNameScore + rowIndexStr);
        as_paraGui.addToScreen(GUI_OBJECT_LABEL, labelNameScore + rowIndexStr, "highScoreDisplay");
        as_paraGui.setPosition(GUI_OBJECT_LABEL, labelNameScore + rowIndexStr, 8, GUI_COORD_PERCENT, scorePositionStartX, rowPositionStartY + (rowPositionSpaceY * i), 40, 10);
        as_paraGui.setLabel(GUI_OBJECT_LABEL, labelNameScore + rowIndexStr, 0, GUI_LABEL_LEFT, gui_getHighScoreValueByIndex(i));
        as_paraGui.setFontName(GUI_OBJECT_LABEL, labelNameScore + rowIndexStr, "guiFont");
        as_paraGui.setReady(GUI_OBJECT_LABEL, labelNameScore + rowIndexStr, true);

        as_paraGui.create(GUI_OBJECT_LABEL, labelNameName + rowIndexStr);
        as_paraGui.addToScreen(GUI_OBJECT_LABEL, labelNameName + rowIndexStr, "highScoreDisplay");
        as_paraGui.setPosition(GUI_OBJECT_LABEL, labelNameName + rowIndexStr, 8, GUI_COORD_PERCENT, namePositionStartX, rowPositionStartY + (rowPositionSpaceY * i), 40, 10);
        as_paraGui.setLabel(GUI_OBJECT_LABEL, labelNameName + rowIndexStr, 0, GUI_LABEL_LEFT, gui_getHighScoreNameByIndex(i));
        as_paraGui.setFontName(GUI_OBJECT_LABEL, labelNameName + rowIndexStr, "guiFont");
        as_paraGui.setReady(GUI_OBJECT_LABEL, labelNameName + rowIndexStr, true);
    }

    as_paraGui.create(GUI_OBJECT_BUTTON, "highScoreDisplay.backButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "highScoreDisplay.backButton", "highScoreDisplay");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "highScoreDisplay.backButton", 3, GUI_LABEL_CENTER, gui_getString("backButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "highScoreDisplay.backButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "highScoreDisplay.backButton", 8, GUI_COORD_PERCENT, headingStartX, elementSpacingY * 5, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "highScoreDisplay.backButton", "as_handleHighscoreDisplay");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "highScoreDisplay.backButton", true);
}
