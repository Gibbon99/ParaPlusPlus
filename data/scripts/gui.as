//----------------------------------------------------------------------------------------------------------------------
//
// Angelscript - Setup and handle the GUI
//
//----------------------------------------------------------------------------------------------------------------------

int buttonWidth = 40;
int buttonHeight = 10;
int buttonSpacing = 16;
int buttonStartX = 10;
int scrollBoxBorderWidth = 10;

//----------------------------------------------------------------------------------------------------------------------
//
// Handle the actions from the main menu
void as_handleMainMenu()
//----------------------------------------------------------------------------------------------------------------------
{
    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "mainMenu.startGameButton")) {
        gam_setHudText("hudGameOn");
        sys_setNewMode(MODE_PRE_GAME, false);
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "mainMenu.optionsButton")) {
        gam_setHudText("hudOptions");
        as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "optionsMenu"));
        // Set active object
        as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), "optionsMenu.backButton");
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton")) {
        gam_setHudText("hudTutorial");
        as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "scrTutorial"));
        as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), "scrTutorial.buttonTutMoveNext");
        sys_setNewMode(MODE_GUI_TUT_TRANSFER_START, true);
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "mainMenu.briefingButton")) {
        gam_setHudText("hudBriefing");
        gam_addAudioEvent(EVENT_ACTION_AUDIO_PLAY, true, 0, 127, "scrollBeeps");
        sys_setNewMode(MODE_GUI_INTROSCROLL, true);
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "mainMenu.quitButton")) {
        quitProgram = true;
        return;
    }
}

//----------------------------------------------------------------------------------------------------------------------
//
// Handle the actions from the options menu
void as_handleOptionsMenu()
//----------------------------------------------------------------------------------------------------------------------
{
    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "optionsMenu.videoButton")) {
        gam_setHudText("optionsMenu.videoButton");
        as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "videoOptions"));
        as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), "videoOptions.backButton");
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "optionsMenu.audioButton")) {
        gam_setHudText("optionsMenu.audioButton");
        as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "audioOptions"));
        as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), "audioOptions.backButton");
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "optionsMenu.controlsButton")) {
        gam_setHudText("optionsMenu.controlsButton");
        as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "controlsMenu"));
        as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), "controlsMenu.backButton");
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "optionsMenu.gameButton")) {
        gam_setHudText("optionsMenu.gameButton");
        as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "gameMenu"));
        as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), "gameMenu.backButton");
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "optionsMenu.backButton")) {
        gam_setHudText("hudMainMenu");
        as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "mainMenu"));
        as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), "mainMenu.optionsButton");
        return;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Process the elements on the Audio menu
void as_handleAudioMenu()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
    int
    enableSoundInt;

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "audioOptions.backButton")) {
        g_volumeLevel = parseInt(as_paraGui.getSliderValue("audioOptions.volume"));
        as_audio.setMasterVolume(g_volumeLevel);
        io_updateConfigValueInt("g_volumeLevel", g_volumeLevel);

        as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "optionsMenu"));
        as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), "optionsMenu.audioButton");
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_CHECKBOX, "audioOptions.enableSound")) {
        enableSound = !enableSound;
        as_paraGui.setTickedStatus("audioOptions.enableSound", -1, enableSound);
        enableSoundInt = enableSound ? 1 : 0;
        io_updateConfigValueInt("enableSound", enableSoundInt);
        if (enableSound == true) {
            as_paraGui.setReady(GUI_OBJECT_SLIDER, "audioOptions.volume", true);
            as_paraGui.setReady(GUI_OBJECT_SLIDER, "audioOptions.speakers", true);
        } else {
            as_paraGui.setReady(GUI_OBJECT_SLIDER, "audioOptions.volume", false);
            as_paraGui.setReady(GUI_OBJECT_SLIDER, "audioOptions.speakers", false);
        }
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_SLIDER, "audioOptions.volume")) {
        return;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Refresh the key label
void as_refreshControlLabels()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "controlsOptions.leftKeyname", 0, GUI_LABEL_LEFT, as_paraGui.getKeyName(KEY_LEFT));
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "controlsOptions.rightKeyname", 0, GUI_LABEL_LEFT, as_paraGui.getKeyName(KEY_RIGHT));
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "controlsOptions.upKeyname", 0, GUI_LABEL_LEFT, as_paraGui.getKeyName(KEY_UP));
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "controlsOptions.downKeyname", 0, GUI_LABEL_LEFT, as_paraGui.getKeyName(KEY_DOWN));
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "controlsOptions.actionKeyname", 0, GUI_LABEL_LEFT, as_paraGui.getKeyName(KEY_ACTION));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Process the elements on the Control menu
void as_handleControlsMenu()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "controlsOptions.left"))
    {
        gam_setHudText("hudPressKey");
        io_initNewKeycodeValue(KEY_LEFT);
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "controlsOptions.right"))
    {
        gam_setHudText("hudPressKey");
        io_initNewKeycodeValue(KEY_RIGHT);
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "controlsOptions.up"))
    {
        gam_setHudText("hudPressKey");
        io_initNewKeycodeValue(KEY_UP);
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "controlsOptions.down"))
    {
        gam_setHudText("hudPressKey");
        io_initNewKeycodeValue(KEY_DOWN);
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "controlsOptions.action"))
    {
        gam_setHudText("hudPressKey");
        io_initNewKeycodeValue(KEY_ACTION);
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "controlsMenu.backButton")) {
        gam_setHudText("hudOptions");
        as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "optionsMenu"));
        as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), "optionsMenu.controlsButton");
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "controlsMenu.applyButton"))
    {
        as_paraGui.saveKeymap();  // Save keymappings to file
        gam_setHudText("hudOptions");
        as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "optionsMenu"));
        as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), "optionsMenu.controlsButton");
        return;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Process the elements on the Control menu
void as_handleGameMenu()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
string guiCurrentLanguage;

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "gameMenu.backButton"))
    {
        tileStyle = as_paraGui.getSliderValue("gameMenu.tileStyle");
        tileColor = as_paraGui.getSliderValue("gameMenu.tileColor");
        guiCurrentLanguage = as_paraGui.getSliderValue("gameMenu.language");

    if ("English" == guiCurrentLanguage)
        currentLanguage = LANG_ENGLISH;

    if ("Italian" == guiCurrentLanguage)
        currentLanguage = LANG_ITALIAN;

    if ("German" == guiCurrentLanguage)
        currentLanguage = LANG_GERMAN;

    as_setLanguageStrings();

    gam_setTileType();

    io_updateConfigValue("tileStyle", tileStyle);
    io_updateConfigValue("tileColor", tileColor);
    io_updateConfigValueInt("currentLanguage", currentLanguage);

    gam_setHudText("hudOptions");
    as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "optionsMenu"));
    as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), "optionsMenu.gameButton");
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Process the elements on the Control menu
void as_handleVideoOptions()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
    int
    boolToInt;

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_CHECKBOX, "videoOptions.highdpi")) {
        windowAllowHighDPI = !windowAllowHighDPI;
        as_paraGui.setTickedStatus("videoOptions.highdpi", -1, windowAllowHighDPI);
        boolToInt = windowAllowHighDPI ? 1 : 0;
        io_updateConfigValueInt("windowAllowHighDPI", boolToInt);
        return;
    }


    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow")) {
        windowBorderless = !windowBorderless;
        as_paraGui.setTickedStatus("videoOptions.borderlesswindow", -1, windowBorderless);
        boolToInt = windowBorderless ? 1 : 0;
        io_updateConfigValueInt("windowBorderless", boolToInt);
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_CHECKBOX, "videoOptions.usevsync")) {
        presentVSync = !presentVSync;
        as_paraGui.setTickedStatus("videoOptions.usevsync", -1, presentVSync);
        boolToInt = presentVSync ? 1 : 0;
        io_updateConfigValueInt("presentVSync", boolToInt);
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_CHECKBOX, "videoOptions.screeneffect")) {
        doScreenEffect = !doScreenEffect;
        as_paraGui.setTickedStatus("videoOptions.screeneffect", -1, doScreenEffect);
        boolToInt = doScreenEffect ? 1 : 0;
        io_updateConfigValueInt("doScreenEffect", boolToInt);
        return;
    }

    if (as_paraGui.getActiveObjectIndex() == as_paraGui.getIndex(GUI_OBJECT_BUTTON, "videoOptions.backButton")) {
        gam_setHudText("hudOptions");
        as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "optionsMenu"));
        as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), "optionsMenu.videoButton");
        return;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Create the Video options menu. Window Setting; Which Renderer; Use VSync Checkbox
void createVideoOptions()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
    int numRenderers;
    string tempNum;
    string currentRendererStr;

    as_paraGui.create(GUI_OBJECT_SCREEN, "videoOptions");
    /*
        as_paraGui.create (GUI_OBJECT_LABEL, "videoOptions.heading");
        as_paraGui.addToScreen (GUI_OBJECT_LABEL, "videoOptions.heading", "videoMenu");
        as_paraGui.setPosition (GUI_OBJECT_LABEL, "videoOptions.heading", 0, GUI_COORD_ABSOLUTE, 3, 3, 2, 2);
        as_paraGui.setLabel (GUI_OBJECT_LABEL, "videoOptions.heading", 0, GUI_LABEL_LEFT, gui_getString ("videoOptions.heading"));
        as_paraGui.setFontName (GUI_OBJECT_LABEL, "videoOptions.heading", "guiFont");
        as_paraGui.setReady (GUI_OBJECT_LABEL, "videoOptions.heading", true);
        */
    //
    // Renderers
    //
    numRenderers = as_renderer.getNumRenderers();
    as_paraGui.create(GUI_OBJECT_SLIDER, "videoOptions.renderers");
    as_paraGui.addToScreen(GUI_OBJECT_SLIDER, "videoOptions.renderers", "videoOptions");
    as_paraGui.setPosition(GUI_OBJECT_SLIDER, "videoOptions.renderers", 8, GUI_COORD_PERCENT, 5, 25, 30, 2);
    as_paraGui.setLabel(GUI_OBJECT_SLIDER, "videoOptions.renderers", 12, GUI_LABEL_CENTER, gui_getString("videoOptions.renderers"));
    as_paraGui.setAction(GUI_OBJECT_SLIDER, "videoOptions.renderers", "as_handleVideoOptions");

    for (int i = 0; i != numRenderers; i++)
    {
        tempNum = formatInt(i, "l");
        as_paraGui.addNewElement("videoOptions.renderers", tempNum, as_renderer.getRendererByIndex(i), SLIDER_TYPE_INT);
    }

    currentRendererStr = formatInt(as_renderer.getCurrentRendererIndex(), "l");
    as_paraGui.setSliderValue("videoOptions.renderers", currentRendererStr);
    as_paraGui.setFontName(GUI_OBJECT_SLIDER, "videoOptions.renderers", "guiFont28");
    as_paraGui.setReady(GUI_OBJECT_SLIDER, "videoOptions.renderers", true);
    //
    // Display
    //
    as_paraGui.create(GUI_OBJECT_SLIDER, "videoOptions.display");
    as_paraGui.addToScreen(GUI_OBJECT_SLIDER, "videoOptions.display", "videoOptions");
    as_paraGui.setPosition(GUI_OBJECT_SLIDER, "videoOptions.display", 8, GUI_COORD_PERCENT, 5, 52, 30, 2);
    as_paraGui.setLabel(GUI_OBJECT_SLIDER, "videoOptions.display", 12, GUI_LABEL_CENTER, gui_getString("videoOptions.display"));
    as_paraGui.setAction(GUI_OBJECT_SLIDER, "videoOptions.display", "as_handleVideoOptions");

    as_paraGui.addNewElement("videoOptions.display", "0", gui_getString("videoOptions.windowed"), SLIDER_TYPE_INT);
    as_paraGui.addNewElement("videoOptions.display", "1", gui_getString("videoOptions.fullscreen"), SLIDER_TYPE_INT);
    as_paraGui.addNewElement("videoOptions.display", "2", gui_getString("videoOptions.desktop"), SLIDER_TYPE_INT);

//	currentRendererStr = formatInt(as_renderer.getCurrentRendererIndex(), "l");
//	as_paraGui.setSliderValue ("videoOptions.display", currentRendererStr);
    as_paraGui.setFontName(GUI_OBJECT_SLIDER, "videoOptions.display", "guiFont28");
    as_paraGui.setReady(GUI_OBJECT_SLIDER, "videoOptions.display", true);
    //
    // Scale quality
    //
    as_paraGui.create(GUI_OBJECT_SLIDER, "videoOptions.scalequality");
    as_paraGui.addToScreen(GUI_OBJECT_SLIDER, "videoOptions.scalequality", "videoOptions");
    as_paraGui.setPosition(GUI_OBJECT_SLIDER, "videoOptions.scalequality", 8, GUI_COORD_PERCENT, 5, 84, 30, 2);
    as_paraGui.setLabel(GUI_OBJECT_SLIDER, "videoOptions.scalequality", 12, GUI_LABEL_CENTER, gui_getString("videoOptions.scalequality"));
    as_paraGui.setAction(GUI_OBJECT_SLIDER, "videoOptions.scalequality", "as_handleVideoOptions");

    as_paraGui.addNewElement("videoOptions.scalequality", "0", gui_getString("videoOptions.nearest"), SLIDER_TYPE_INT);
    as_paraGui.addNewElement("videoOptions.scalequality", "1", gui_getString("videoOptions.linear"), SLIDER_TYPE_INT);
    as_paraGui.addNewElement("videoOptions.scalequality", "2", gui_getString("videoOptions.best"), SLIDER_TYPE_INT);

//	currentRendererStr = formatInt(as_renderer.getCurrentRendererIndex(), "l");
//	as_paraGui.setSliderValue ("videoOptions.display", currentRendererStr);
    as_paraGui.setFontName(GUI_OBJECT_SLIDER, "videoOptions.scalequality", "guiFont28");
    as_paraGui.setReady(GUI_OBJECT_SLIDER, "videoOptions.scalequality", true);
    //
    // Use VSync
    //
    as_paraGui.create(GUI_OBJECT_CHECKBOX, "videoOptions.usevsync");
    as_paraGui.addToScreen(GUI_OBJECT_CHECKBOX, "videoOptions.usevsync", "videoOptions");
    as_paraGui.setPosition(GUI_OBJECT_CHECKBOX, "videoOptions.usevsync", 3, GUI_COORD_PERCENT, 60, 15, 5, 7);
    as_paraGui.setLabel(GUI_OBJECT_CHECKBOX, "videoOptions.usevsync", 6, GUI_LABEL_LEFT, gui_getString("videoOptions.usevsync"));
    as_paraGui.setFontName(GUI_OBJECT_CHECKBOX, "videoOptions.usevsync", "guiFont28");
    as_paraGui.setAction(GUI_OBJECT_CHECKBOX, "videoOptions.usevsync", "as_handleVideoOptions");
    as_paraGui.setReady(GUI_OBJECT_CHECKBOX, "videoOptions.usevsync", true);
    as_paraGui.setTickedStatus("videoOptions.usevsync", -1, presentVSync);
    //
    // Borderless window
    //
    as_paraGui.create(GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow");
    as_paraGui.addToScreen(GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow", "videoOptions");
    as_paraGui.setPosition(GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow", 3, GUI_COORD_PERCENT, 60, 30, 5, 7);
    as_paraGui.setLabel(GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow", 6, GUI_LABEL_LEFT, gui_getString("videoOptions.borderlesswindow"));
    as_paraGui.setFontName(GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow", "guiFont28");
    as_paraGui.setAction(GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow", "as_handleVideoOptions");
    as_paraGui.setReady(GUI_OBJECT_CHECKBOX, "videoOptions.borderlesswindow", true);
    as_paraGui.setTickedStatus("videoOptions.borderlesswindow", -1, windowBorderless);
    //
    // Allow high DPI
    //
    as_paraGui.create(GUI_OBJECT_CHECKBOX, "videoOptions.highdpi");
    as_paraGui.addToScreen(GUI_OBJECT_CHECKBOX, "videoOptions.highdpi", "videoOptions");
    as_paraGui.setPosition(GUI_OBJECT_CHECKBOX, "videoOptions.highdpi", 3, GUI_COORD_PERCENT, 60, 45, 5, 7);
    as_paraGui.setLabel(GUI_OBJECT_CHECKBOX, "videoOptions.highdpi", 6, GUI_LABEL_LEFT, gui_getString("videoOptions.highdpi"));
    as_paraGui.setFontName(GUI_OBJECT_CHECKBOX, "videoOptions.highdpi", "guiFont28");
    as_paraGui.setAction(GUI_OBJECT_CHECKBOX, "videoOptions.highdpi", "as_handleVideoOptions");
    as_paraGui.setReady(GUI_OBJECT_CHECKBOX, "videoOptions.highdpi", true);
    as_paraGui.setTickedStatus("videoOptions.highdpi", -1, windowAllowHighDPI);
    //
    // Use screen effect
    //
    as_paraGui.create(GUI_OBJECT_CHECKBOX, "videoOptions.screeneffect");
    as_paraGui.addToScreen(GUI_OBJECT_CHECKBOX, "videoOptions.screeneffect", "videoOptions");
    as_paraGui.setPosition(GUI_OBJECT_CHECKBOX, "videoOptions.screeneffect", 3, GUI_COORD_PERCENT, 60, 60, 5, 7);
    as_paraGui.setLabel(GUI_OBJECT_CHECKBOX, "videoOptions.screeneffect", 6, GUI_LABEL_LEFT, gui_getString("videoOptions.screeneffect"));
    as_paraGui.setFontName(GUI_OBJECT_CHECKBOX, "videoOptions.screeneffect", "guiFont28");
    as_paraGui.setAction(GUI_OBJECT_CHECKBOX, "videoOptions.screeneffect", "as_handleVideoOptions");
    as_paraGui.setReady(GUI_OBJECT_CHECKBOX, "videoOptions.screeneffect", true);
    as_paraGui.setTickedStatus("videoOptions.screeneffect", -1, doScreenEffect);
    //
    // Apply Button
    //
    as_paraGui.create(GUI_OBJECT_BUTTON, "videoOptions.applyButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "videoOptions.applyButton", "videoOptions");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "videoOptions.applyButton", 3, GUI_LABEL_CENTER, gui_getString("videoOptions.applyButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "videoOptions.applyButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "videoOptions.applyButton", 8, GUI_COORD_PERCENT, 59, 70, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "videoOptions.applyButton", "as_handleVideoOptions");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "videoOptions.applyButton", true);
    //
    // Back button
    //
    as_paraGui.create(GUI_OBJECT_BUTTON, "videoOptions.backButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "videoOptions.backButton", "videoOptions");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "videoOptions.backButton", 3, GUI_LABEL_CENTER, gui_getString("backButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "videoOptions.backButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "videoOptions.backButton", 8, GUI_COORD_PERCENT, 59, 85, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "videoOptions.backButton", "as_handleVideoOptions");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "videoOptions.backButton", true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Create the Audio options menu. Checkbox: enable sound; Slider: Volume; Slider: numChannels ( 1, 2)
void createAudioMenu()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
    string currentVolumeStr;
    string tempNum;

    as_paraGui.create(GUI_OBJECT_SCREEN, "audioOptions");

    /*
    as_paraGui.create (GUI_OBJECT_LABEL, "audioOptions.heading");
    as_paraGui.addToScreen (GUI_OBJECT_LABEL, "audioOptions.heading", "audioOptions");
    as_paraGui.setPosition (GUI_OBJECT_LABEL, "audioOptions.heading", 0, GUI_COORD_ABSOLUTE, 3, 3, 2, 2);
    as_paraGui.setLabel (GUI_OBJECT_LABEL, "audioOptions.heading", 0, GUI_LABEL_LEFT, gui_getString ("audioOptions.heading"));
    as_paraGui.setFontName (GUI_OBJECT_LABEL, "audioOptions.heading", "guiFont");
    as_paraGui.setReady (GUI_OBJECT_LABEL, "audioOptions.heading", true);
*/

    as_paraGui.create(GUI_OBJECT_CHECKBOX, "audioOptions.enableSound");
    as_paraGui.addToScreen(GUI_OBJECT_CHECKBOX, "audioOptions.enableSound", "audioOptions");
    as_paraGui.setPosition(GUI_OBJECT_CHECKBOX, "audioOptions.enableSound", 3, GUI_COORD_PERCENT, 2, 20, 5, 7);
    as_paraGui.setLabel(GUI_OBJECT_CHECKBOX, "audioOptions.enableSound", 6, GUI_LABEL_LEFT, gui_getString("audioOptions.enableSound"));
    as_paraGui.setFontName(GUI_OBJECT_CHECKBOX, "audioOptions.enableSound", "guiFont28");
    as_paraGui.setAction(GUI_OBJECT_CHECKBOX, "audioOptions.enableSound", "as_handleAudioMenu");
    as_paraGui.setReady(GUI_OBJECT_CHECKBOX, "audioOptions.enableSound", true);
    as_paraGui.setTickedStatus("audioOptions.enableSound", -1, enableSound);

    as_paraGui.create(GUI_OBJECT_SLIDER, "audioOptions.volume");
    as_paraGui.addToScreen(GUI_OBJECT_SLIDER, "audioOptions.volume", "audioOptions");
    as_paraGui.setPosition(GUI_OBJECT_SLIDER, "audioOptions.volume", 8, GUI_COORD_PERCENT, 5, 50, 40, 2);
    as_paraGui.setLabel(GUI_OBJECT_SLIDER, "audioOptions.volume", 12, GUI_LABEL_CENTER, gui_getString("audioOptions.volume"));
    as_paraGui.setAction(GUI_OBJECT_SLIDER, "audioOptions.volume", "as_handleAudioMenu");

    for (int i = 0; i != 10; i++)
    {
        tempNum = formatInt(i + 1, "l");
        as_paraGui.addNewElement("audioOptions.volume", tempNum, tempNum, SLIDER_TYPE_INT);
    }

    currentVolumeStr = formatInt(g_volumeLevel, "l");
    as_paraGui.setSliderValue("audioOptions.volume", currentVolumeStr);
    as_paraGui.setFontName(GUI_OBJECT_SLIDER, "audioOptions.volume", "guiFont28");

    as_paraGui.create(GUI_OBJECT_SLIDER, "audioOptions.speakers");
    as_paraGui.addToScreen(GUI_OBJECT_SLIDER, "audioOptions.speakers", "audioOptions");
    as_paraGui.setPosition(GUI_OBJECT_SLIDER, "audioOptions.speakers", 8, GUI_COORD_PERCENT, 55, 50, 40, 2);
    as_paraGui.setLabel(GUI_OBJECT_SLIDER, "audioOptions.speakers", 12, GUI_LABEL_LEFT, gui_getString("audioOptions.speakers"));
    as_paraGui.setAction(GUI_OBJECT_SLIDER, "audioOptions.speakers", "as_handleAudioMenu");
    as_paraGui.addNewElement("audioOptions.speakers", "0", "Mono", SLIDER_TYPE_INT);
    as_paraGui.addNewElement("audioOptions.speakers", "1", "Stereo", SLIDER_TYPE_INT);
    as_paraGui.setFontName(GUI_OBJECT_SLIDER, "audioOptions.speakers", "guiFont28");

    if (enableSound == true)
    {
        as_paraGui.setReady(GUI_OBJECT_SLIDER, "audioOptions.volume", true);
        as_paraGui.setReady(GUI_OBJECT_SLIDER, "audioOptions.speakers", true);
    } else {
        as_paraGui.setReady(GUI_OBJECT_SLIDER, "audioOptions.volume", false);
        as_paraGui.setReady(GUI_OBJECT_SLIDER, "audioOptions.speakers", false);
    }

    as_paraGui.create(GUI_OBJECT_BUTTON, "audioOptions.backButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "audioOptions.backButton", "audioOptions");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "audioOptions.backButton", 3, GUI_LABEL_CENTER, gui_getString("backButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "audioOptions.backButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "audioOptions.backButton", 8, GUI_COORD_PERCENT, 59, 85, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "audioOptions.backButton", "as_handleAudioMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "audioOptions.backButton", true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Create the Controls menu
void createControlsMenu()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
    int controlsButtonSpacing = 12;
    int controlsButtonsStartY = 22;
    int controlsGapSize = 15;
    int controlsButtonsNameY = controlsButtonsStartY + 3;

    as_paraGui.create(GUI_OBJECT_SCREEN, "controlsMenu");

    as_paraGui.create(GUI_OBJECT_LABEL, "controlsOptions.heading");
    as_paraGui.addToScreen(GUI_OBJECT_LABEL, "controlsOptions.heading", "controlsMenu");
    as_paraGui.setPosition(GUI_OBJECT_LABEL, "controlsOptions.heading", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing, buttonWidth, buttonHeight);
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "controlsOptions.heading", 0, GUI_LABEL_LEFT, gui_getString("controlsOptions.heading"));
    as_paraGui.setFontName(GUI_OBJECT_LABEL, "controlsOptions.heading", "guiFont");
    as_paraGui.setReady(GUI_OBJECT_LABEL, "controlsOptions.heading", true);


    as_paraGui.create(GUI_OBJECT_BUTTON, "controlsOptions.left");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "controlsOptions.left", "controlsMenu");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "controlsOptions.left", 8, GUI_COORD_PERCENT, buttonStartX, controlsButtonsStartY, buttonWidth, buttonHeight);
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "controlsOptions.left", controlsGapSize, GUI_LABEL_LEFT, gui_getString("gameLeft"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "controlsOptions.left", "guiFont");
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "controlsOptions.left", "as_handleControlsMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "controlsOptions.left", true);

    as_paraGui.create(GUI_OBJECT_BUTTON, "controlsOptions.right");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "controlsOptions.right", "controlsMenu");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "controlsOptions.right", 8, GUI_COORD_PERCENT, buttonStartX, controlsButtonsStartY + (controlsButtonSpacing * 1), buttonWidth, buttonHeight);
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "controlsOptions.right", controlsGapSize, GUI_LABEL_LEFT, gui_getString("gameRight"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "controlsOptions.right", "guiFont");
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "controlsOptions.right", "as_handleControlsMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "controlsOptions.right", true);

    as_paraGui.create(GUI_OBJECT_BUTTON, "controlsOptions.up");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "controlsOptions.up", "controlsMenu");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "controlsOptions.up", 8, GUI_COORD_PERCENT, buttonStartX, controlsButtonsStartY + (controlsButtonSpacing * 2), buttonWidth, buttonHeight);
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "controlsOptions.up", controlsGapSize, GUI_LABEL_LEFT, gui_getString("gameUp"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "controlsOptions.up", "guiFont");
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "controlsOptions.up", "as_handleControlsMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "controlsOptions.up", true);

    as_paraGui.create(GUI_OBJECT_BUTTON, "controlsOptions.down");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "controlsOptions.down", "controlsMenu");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "controlsOptions.down", 8, GUI_COORD_PERCENT, buttonStartX, controlsButtonsStartY + (controlsButtonSpacing * 3), buttonWidth, buttonHeight);
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "controlsOptions.down", controlsGapSize, GUI_LABEL_LEFT, gui_getString("gameDown"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "controlsOptions.down", "guiFont");
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "controlsOptions.down", "as_handleControlsMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "controlsOptions.down", true);

    as_paraGui.create(GUI_OBJECT_BUTTON, "controlsOptions.action");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "controlsOptions.action", "controlsMenu");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "controlsOptions.action", 8, GUI_COORD_PERCENT, buttonStartX, controlsButtonsStartY + (controlsButtonSpacing * 4), buttonWidth, buttonHeight);
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "controlsOptions.action", controlsGapSize, GUI_LABEL_LEFT, gui_getString("gameAction"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "controlsOptions.action", "guiFont");
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "controlsOptions.action", "as_handleControlsMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "controlsOptions.action", true);

// Show the text name of the keys
    as_paraGui.create(GUI_OBJECT_LABEL, "controlsOptions.leftKeyname");
    as_paraGui.addToScreen(GUI_OBJECT_LABEL, "controlsOptions.leftKeyname", "controlsMenu");
    as_paraGui.setPosition(GUI_OBJECT_LABEL, "controlsOptions.leftKeyname", 9, GUI_COORD_PERCENT, 55, controlsButtonsNameY, 10, 10);
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "controlsOptions.leftKeyname", 0, GUI_LABEL_LEFT, as_paraGui.getKeyName(KEY_LEFT));
    as_paraGui.setFontName(GUI_OBJECT_LABEL, "controlsOptions.leftKeyname", "guiFont");
    as_paraGui.setReady(GUI_OBJECT_LABEL, "controlsOptions.leftKeyname", true);

    as_paraGui.create(GUI_OBJECT_LABEL, "controlsOptions.rightKeyname");
    as_paraGui.addToScreen(GUI_OBJECT_LABEL, "controlsOptions.rightKeyname", "controlsMenu");
    as_paraGui.setPosition(GUI_OBJECT_LABEL, "controlsOptions.rightKeyname", 9, GUI_COORD_PERCENT, 55, controlsButtonsNameY + (controlsButtonSpacing * 1), 10, 10);
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "controlsOptions.rightKeyname", 0, GUI_LABEL_LEFT, as_paraGui.getKeyName(KEY_RIGHT));
    as_paraGui.setFontName(GUI_OBJECT_LABEL, "controlsOptions.rightKeyname", "guiFont");
    as_paraGui.setReady(GUI_OBJECT_LABEL, "controlsOptions.rightKeyname", true);

    as_paraGui.create(GUI_OBJECT_LABEL, "controlsOptions.upKeyname");
    as_paraGui.addToScreen(GUI_OBJECT_LABEL, "controlsOptions.upKeyname", "controlsMenu");
    as_paraGui.setPosition(GUI_OBJECT_LABEL, "controlsOptions.upKeyname", 9, GUI_COORD_PERCENT, 55, controlsButtonsNameY + (controlsButtonSpacing * 2), 10, 10);
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "controlsOptions.upKeyname", 0, GUI_LABEL_LEFT, as_paraGui.getKeyName(KEY_UP));
    as_paraGui.setFontName(GUI_OBJECT_LABEL, "controlsOptions.upKeyname", "guiFont");
    as_paraGui.setReady(GUI_OBJECT_LABEL, "controlsOptions.upKeyname", true);

    as_paraGui.create(GUI_OBJECT_LABEL, "controlsOptions.downKeyname");
    as_paraGui.addToScreen(GUI_OBJECT_LABEL, "controlsOptions.downKeyname", "controlsMenu");
    as_paraGui.setPosition(GUI_OBJECT_LABEL, "controlsOptions.downKeyname", 9, GUI_COORD_PERCENT, 55, controlsButtonsNameY + (controlsButtonSpacing * 3), 10, 10);
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "controlsOptions.downKeyname", 0, GUI_LABEL_LEFT, as_paraGui.getKeyName(KEY_DOWN));
    as_paraGui.setFontName(GUI_OBJECT_LABEL, "controlsOptions.downKeyname", "guiFont");
    as_paraGui.setReady(GUI_OBJECT_LABEL, "controlsOptions.downKeyname", true);

    as_paraGui.create(GUI_OBJECT_LABEL, "controlsOptions.actionKeyname");
    as_paraGui.addToScreen(GUI_OBJECT_LABEL, "controlsOptions.actionKeyname", "controlsMenu");
    as_paraGui.setPosition(GUI_OBJECT_LABEL, "controlsOptions.actionKeyname", 9, GUI_COORD_PERCENT, 55, controlsButtonsNameY + (controlsButtonSpacing * 4), 10, 10);
    as_paraGui.setLabel(GUI_OBJECT_LABEL, "controlsOptions.actionKeyname", 0, GUI_LABEL_LEFT, as_paraGui.getKeyName(KEY_ACTION));
    as_paraGui.setFontName(GUI_OBJECT_LABEL, "controlsOptions.actionKeyname", "guiFont");
    as_paraGui.setReady(GUI_OBJECT_LABEL, "controlsOptions.actionKeyname", true);

    as_paraGui.create(GUI_OBJECT_BUTTON, "controlsMenu.backButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "controlsMenu.backButton", "controlsMenu");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "controlsMenu.backButton", 3, GUI_LABEL_CENTER, gui_getString("backButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "controlsMenu.backButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "controlsMenu.backButton", 8, GUI_COORD_PERCENT, buttonStartX, (buttonSpacing * 5) + 10, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "controlsMenu.backButton", "as_handleControlsMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "controlsMenu.backButton", true);

    as_paraGui.create(GUI_OBJECT_BUTTON, "controlsMenu.applyButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "controlsMenu.applyButton", "controlsMenu");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "controlsMenu.applyButton", 3, GUI_LABEL_CENTER, gui_getString("videoOptions.applyButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "controlsMenu.applyButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "controlsMenu.applyButton", 8, GUI_COORD_PERCENT, 55, (buttonSpacing * 5) + 10, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "controlsMenu.applyButton", "as_handleControlsMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "controlsMenu.applyButton", true);

}

//-------------------------------------------------------------------------------------------------------------------------------------------------
//
// Create the Game menu
void createGameMenu()
//-------------------------------------------------------------------------------------------------------------------------------------------------
{
    string tempNum;

    as_paraGui.create(GUI_OBJECT_SCREEN, "gameMenu");

    as_paraGui.create(GUI_OBJECT_SLIDER, "gameMenu.language");
    as_paraGui.addToScreen(GUI_OBJECT_SLIDER, "gameMenu.language", "gameMenu");
    as_paraGui.setPosition(GUI_OBJECT_SLIDER, "gameMenu.language", 8, GUI_COORD_PERCENT, 5, 30, 40, 2);
    as_paraGui.setLabel(GUI_OBJECT_SLIDER, "gameMenu.language", 12, GUI_LABEL_CENTER, gui_getString("gameMenu.language"));

    tempNum = formatInt(LANG_ENGLISH, "l");
    as_paraGui.addNewElement("gameMenu.language", tempNum, "English", SLIDER_TYPE_INT);

    tempNum = formatInt(LANG_ITALIAN, "l");
    as_paraGui.addNewElement("gameMenu.language", tempNum, "Italian", SLIDER_TYPE_INT);

    tempNum = formatInt(LANG_GERMAN, "l");
    as_paraGui.addNewElement("gameMenu.language", tempNum, "German", SLIDER_TYPE_INT);

    if (LANG_ENGLISH == currentLanguage)
       as_paraGui.setSliderValue("gameMenu.language", "English");

    if (LANG_ITALIAN == currentLanguage)
       as_paraGui.setSliderValue("gameMenu.language", "Italian");

    if (LANG_GERMAN == currentLanguage)
       as_paraGui.setSliderValue("gameMenu.language", "German");

    as_paraGui.setFontName(GUI_OBJECT_SLIDER, "gameMenu.language", "guiFont28");
    as_paraGui.setReady(GUI_OBJECT_SLIDER, "gameMenu.language", true);


    as_paraGui.create(GUI_OBJECT_SLIDER, "gameMenu.tileStyle");
    as_paraGui.addToScreen(GUI_OBJECT_SLIDER, "gameMenu.tileStyle", "gameMenu");
    as_paraGui.setPosition(GUI_OBJECT_SLIDER, "gameMenu.tileStyle", 8, GUI_COORD_PERCENT, 55, 50, 40, 2);
    as_paraGui.setLabel(GUI_OBJECT_SLIDER, "gameMenu.tileStyle", 12, GUI_LABEL_LEFT, gui_getString("gameMenu.tileStyle"));
    as_paraGui.setAction(GUI_OBJECT_SLIDER, "gameMenu.tileStyle", "as_handleGameMenu");
    as_paraGui.addNewElement("gameMenu.tileStyle", "retro", "retro", SLIDER_TYPE_STRING);
    as_paraGui.addNewElement("gameMenu.tileStyle", "future", "future", SLIDER_TYPE_STRING);
    as_paraGui.setFontName(GUI_OBJECT_SLIDER, "gameMenu.tileStyle", "guiFont28");

    as_paraGui.setSliderValue("gameMenu.tileStyle", tileStyle);
    as_paraGui.setReady(GUI_OBJECT_SLIDER, "gameMenu.tileStyle", true);

    as_paraGui.create(GUI_OBJECT_SLIDER, "gameMenu.tileColor");
    as_paraGui.addToScreen(GUI_OBJECT_SLIDER, "gameMenu.tileColor", "gameMenu");
    as_paraGui.setPosition(GUI_OBJECT_SLIDER, "gameMenu.tileColor", 8, GUI_COORD_PERCENT, 5, 50, 40, 2);
    as_paraGui.setLabel(GUI_OBJECT_SLIDER, "gameMenu.tileColor", 12, GUI_LABEL_LEFT, gui_getString("gameMenu.tileColor"));
    as_paraGui.setAction(GUI_OBJECT_SLIDER, "gameMenu.tileColor", "as_handleGameMenu");
    as_paraGui.addNewElement("gameMenu.tileColor", "blue", "blue", SLIDER_TYPE_STRING);
    as_paraGui.addNewElement("gameMenu.tileColor", "gray", "gray", SLIDER_TYPE_STRING);
    as_paraGui.addNewElement("gameMenu.tileColor", "green", "green", SLIDER_TYPE_STRING);
    as_paraGui.addNewElement("gameMenu.tileColor", "red", "red", SLIDER_TYPE_STRING);
    as_paraGui.addNewElement("gameMenu.tileColor", "turquoise", "turquoise", SLIDER_TYPE_STRING);
    as_paraGui.addNewElement("gameMenu.tileColor", "yellow", "yellow", SLIDER_TYPE_STRING);
    as_paraGui.setFontName(GUI_OBJECT_SLIDER, "gameMenu.tileColor", "guiFont28");

    as_paraGui.setSliderValue("gameMenu.tileColor", tileColor);
    as_paraGui.setReady(GUI_OBJECT_SLIDER, "gameMenu.tileColor", true);

    as_paraGui.create(GUI_OBJECT_BUTTON, "gameMenu.backButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "gameMenu.backButton", "gameMenu");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "gameMenu.backButton", 3, GUI_LABEL_CENTER, gui_getString("backButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "gameMenu.backButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "gameMenu.backButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 5, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "gameMenu.backButton", "as_handleGameMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "gameMenu.backButton", true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the options menu
void createOptionsMenu()
//----------------------------------------------------------------------------------------------------------------------
{
    as_paraGui.create(GUI_OBJECT_SCREEN, "optionsMenu");

    as_paraGui.create(GUI_OBJECT_IMAGE, "optionsMenu.backGround");
    as_paraGui.addToScreen(GUI_OBJECT_IMAGE, "optionsMenu.backGround", "optionsMenu");
    as_paraGui.setLabel(GUI_OBJECT_IMAGE, "optionsMenu.backGround", 0, GUI_LABEL_CENTER, "universe");
    as_paraGui.setPosition(GUI_OBJECT_IMAGE, "optionsMenu.backGround", 0, GUI_COORD_ABSOLUTE, 0, 0, 0, 0);
    as_paraGui.setReady(GUI_OBJECT_IMAGE, "optionsMenu.backGround", true);

    as_paraGui.create(GUI_OBJECT_BUTTON, "optionsMenu.videoButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "optionsMenu.videoButton", "optionsMenu");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "optionsMenu.videoButton", 3, GUI_LABEL_CENTER, gui_getString("optionsMenu.videoButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "optionsMenu.videoButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "optionsMenu.videoButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 1, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "optionsMenu.videoButton", "as_handleOptionsMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "optionsMenu.videoButton", true);

    as_paraGui.create(GUI_OBJECT_BUTTON, "optionsMenu.audioButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "optionsMenu.audioButton", "optionsMenu");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "optionsMenu.audioButton", 3, GUI_LABEL_CENTER, gui_getString("optionsMenu.audioButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "optionsMenu.audioButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "optionsMenu.audioButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 2, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "optionsMenu.audioButton", "as_handleOptionsMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "optionsMenu.audioButton", true);

    as_paraGui.create(GUI_OBJECT_BUTTON, "optionsMenu.controlsButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "optionsMenu.controlsButton", "optionsMenu");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "optionsMenu.controlsButton", 3, GUI_LABEL_CENTER, gui_getString("optionsMenu.controlsButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "optionsMenu.controlsButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "optionsMenu.controlsButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 3, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "optionsMenu.controlsButton", "as_handleOptionsMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "optionsMenu.controlsButton", true);

    as_paraGui.create(GUI_OBJECT_BUTTON, "optionsMenu.gameButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "optionsMenu.gameButton", "optionsMenu");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "optionsMenu.gameButton", 3, GUI_LABEL_CENTER, gui_getString("optionsMenu.gameButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "optionsMenu.gameButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "optionsMenu.gameButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 4, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "optionsMenu.gameButton", "as_handleOptionsMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "optionsMenu.gameButton", true);

    as_paraGui.create(GUI_OBJECT_BUTTON, "optionsMenu.backButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "optionsMenu.backButton", "optionsMenu");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "optionsMenu.backButton", 3, GUI_LABEL_CENTER, gui_getString("backButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "optionsMenu.backButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "optionsMenu.backButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 5, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "optionsMenu.backButton", "as_handleOptionsMenu");
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "optionsMenu.backButton", true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create the main menu
void createMainMenu()
//----------------------------------------------------------------------------------------------------------------------
{
    as_paraGui.create(GUI_OBJECT_SCREEN, "mainMenu");

    as_paraGui.create(GUI_OBJECT_IMAGE, "mainMenu.backGround");
    as_paraGui.addToScreen(GUI_OBJECT_IMAGE, "mainMenu.backGround", "mainMenu");
    as_paraGui.setLabel(GUI_OBJECT_IMAGE, "mainMenu.backGround", 0, GUI_LABEL_CENTER, "universe");
    as_paraGui.setPosition(GUI_OBJECT_IMAGE, "mainMenu.backGround", 0, GUI_COORD_ABSOLUTE, 0, 0, 0, 0);
    as_paraGui.setReady(GUI_OBJECT_IMAGE, "mainMenu.backGround", true);

    as_paraGui.create(GUI_OBJECT_BUTTON, "mainMenu.startGameButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "mainMenu.startGameButton", "mainMenu");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "mainMenu.startGameButton", 3, GUI_LABEL_CENTER, gui_getString("mainMenu.startGameButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "mainMenu.startGameButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "mainMenu.startGameButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 1, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "mainMenu.startGameButton", "as_handleMainMenu");

    as_paraGui.create(GUI_OBJECT_BUTTON, "mainMenu.optionsButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "mainMenu.optionsButton", "mainMenu");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "mainMenu.optionsButton", 3, GUI_LABEL_CENTER, gui_getString("mainMenu.optionsButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "mainMenu.optionsButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "mainMenu.optionsButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 2, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "mainMenu.optionsButton", "as_handleMainMenu");

    as_paraGui.create(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", "mainMenu");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", 3, GUI_LABEL_CENTER, gui_getString("mainMenu.tutorialButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 3, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", "as_handleMainMenu");

    as_paraGui.create(GUI_OBJECT_BUTTON, "mainMenu.briefingButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "mainMenu.briefingButton", "mainMenu");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "mainMenu.briefingButton", 3, GUI_LABEL_CENTER, gui_getString("mainMenu.briefingButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "mainMenu.briefingButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "mainMenu.briefingButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 4, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "mainMenu.briefingButton", "as_handleMainMenu");

    as_paraGui.create(GUI_OBJECT_BUTTON, "mainMenu.quitButton");
    as_paraGui.addToScreen(GUI_OBJECT_BUTTON, "mainMenu.quitButton", "mainMenu");
    as_paraGui.setLabel(GUI_OBJECT_BUTTON, "mainMenu.quitButton", 3, GUI_LABEL_CENTER, gui_getString("mainMenu.quitButton"));
    as_paraGui.setFontName(GUI_OBJECT_BUTTON, "mainMenu.quitButton", "guiFont");
    as_paraGui.setPosition(GUI_OBJECT_BUTTON, "mainMenu.quitButton", 8, GUI_COORD_PERCENT, buttonStartX, buttonSpacing * 5, buttonWidth, buttonHeight);
    as_paraGui.setAction(GUI_OBJECT_BUTTON, "mainMenu.quitButton", "as_handleMainMenu");

    as_paraGui.setReady(GUI_OBJECT_BUTTON, "mainMenu.startGameButton", true);
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "mainMenu.optionsButton", true);
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "mainMenu.tutorialButton", true);
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "mainMenu.briefingButton", true);
    as_paraGui.setReady(GUI_OBJECT_BUTTON, "mainMenu.quitButton", true);
}

//-----------------------------------------------------------------------------
//
// Create colors for the sideview graphic
void as_setSideviewColors()
//-----------------------------------------------------------------------------
{
    as_createSideViewColor(SIDEVIEW_SHIP_COLOR, 142, 182, 12, 255);
    as_createSideViewColor(SIDEVIEW_ACTIVE_DECK_COLOR, 71, 199, 232, 255);
    as_createSideViewColor(SIDEVIEW_ENGINE_COLOR, 34, 34, 34, 255);
    as_createSideViewColor(SIDEVIEW_LIFT_COLOR, 18, 76, 88, 255);
    as_createSideViewColor(SIDEVIEW_ACTIVE_LIFT_COLOR, 38, 221, 188, 255);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the values for the won screen scrollbox
void as_setupWinScreenScrollBox()
//----------------------------------------------------------------------------------------------------------------------
{
    as_paraGui.create(GUI_OBJECT_SCREEN, "wonScreen");

    as_paraGui.create(GUI_OBJECT_SCROLLBOX, "wonScreen.scrollbox");
    as_paraGui.addToScreen(GUI_OBJECT_SCROLLBOX, "wonScreen.scrollbox", "wonScreen");
    as_paraGui.setLabel(GUI_OBJECT_SCROLLBOX, "wonScreen.scrollbox", 4, GUI_LABEL_CENTER, gui_getString("scrollBoxWonText"));
    as_paraGui.setFontName(GUI_OBJECT_SCROLLBOX, "wonScreen.scrollbox", "introFont");
    as_paraGui.setPosition(GUI_OBJECT_SCROLLBOX, "wonScreen.scrollbox", 8, GUI_COORD_ABSOLUTE, scrollBoxBorderWidth, 100, hiresVirtualWidth - scrollBoxBorderWidth, hiresVirtualHeight - scrollBoxBorderWidth - 100);
    as_paraGui.setColor(GUI_OBJECT_SCROLLBOX, "wonScreen.scrollbox", GUI_COL_ACTIVE, 100, 100, 15, 100);
    as_paraGui.setColor(GUI_OBJECT_SCROLLBOX, "wonScreen.scrollbox", GUI_COL_ACTIVE_LABEL, 20, 150, 100, 240);
    as_paraGui.setScrollSpeed(GUI_OBJECT_SCROLLBOX, "wonScreen.scrollbox", 60.0);
    as_paraGui.setReady(GUI_OBJECT_SCROLLBOX, "wonScreen.scrollbox", true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the values for the intro scrollbox
void as_setupIntroScrollBox()
//----------------------------------------------------------------------------------------------------------------------
{
    as_paraGui.create(GUI_OBJECT_SCREEN, "introScreen");

    as_paraGui.create(GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox");
    as_paraGui.addToScreen(GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", "introScreen");
    as_paraGui.setLabel(GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", 4, GUI_LABEL_CENTER, gui_getString("scrollBoxIntroText"));
    as_paraGui.setFontName(GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", "introFont");
    as_paraGui.setPosition(GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", 8, GUI_COORD_ABSOLUTE, scrollBoxBorderWidth, 100, hiresVirtualWidth - scrollBoxBorderWidth, hiresVirtualHeight - scrollBoxBorderWidth - 100);
    as_paraGui.setColor(GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", GUI_COL_ACTIVE, 100, 100, 15, 100);
    as_paraGui.setColor(GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", GUI_COL_ACTIVE_LABEL, 20, 150, 100, 240);
    as_paraGui.setScrollSpeed(GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", 60.0);
    as_paraGui.setReady(GUI_OBJECT_SCROLLBOX, "introScreen.scrollbox", true);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Entry point to create the GUI
void as_createGUI()
//----------------------------------------------------------------------------------------------------------------------
{
    as_setLanguageStrings();

    baseGameSpeed = 1.0;        // lower is faster - can not be zero
    sideviewDrawScale = 1.2;
    healingAnimSpeed = 0.3;
    playerFriction = 0.7;
    gravity = 25.0;
    doorAnimSpeed = 1.0;
    collisionLimit = 3;
    collisionCount = 6.0;
    bulletDensity = 0.1;
    bulletFriction = 0.01;
    bulletAnimationSpeed = 0.5;
    bulletMoveSpeed = 100.0;
    hudTextPosX = 15;
    hudTextPosY = 27;
    hudScorePosX = 620;
    hudScorePosY = 27;
    visibleFadeValue = 30;
    redAlertLevel = 500;
    yellowAlertLevel = 300;
    updateScoreDelay = 0.5;
    distanceForDoorSoundMax = 50.0;   // In meters
    powerdownLevelScore = 1000;
    healingDelayCounter = 0.8;
    healingAmountPerTick = 1;
    explosionAnimationSpeed = 1.5;
    explosionDamage = 5;
    alertLevelVolume = 190;  // Higher is softer - 1 is full loud
    staticAnimationDelay = 0.8;
    lostScreenShowTime = 120;  // 10 seconds
    disrupterFadeAmount = 20;
    influenceTimelimit = 1000;
    influenceTimeLeftWarning = 250;
    influenceTimelimtDelay = 0.2;
    maxNumBumps = 10;
    bounceCounterDelay = 0.3;
    databaseDroidRenderYOffset = 40;

    as_setSideviewColors();

    as_setupIntroScrollBox();
    as_setupWinScreenScrollBox();
    as_createQuitGameDialogbox();
    as_initTransferValues();

    createMainMenu();
    createOptionsMenu();
    createVideoOptions();
    createAudioMenu();
    createControlsMenu();
    createGameMenu();

    createDatabaseScreen();
    createTerminalMenu();
    createDeckViewScreen();
    createShipViewScreen();

    createTutorialScreens();

    createHighscoreDisplay();
    createHighscoreEntry();

    as_paraGui.setCurrentScreen(as_paraGui.getIndex(GUI_OBJECT_SCREEN, "mainMenu"));
    as_paraGui.setActiveObject(as_paraGui.getCurrentScreen(), "mainMenu.startGame");

    as_paraGui.setColor(GUI_OBJECT_BUTTON, "ALL", GUI_COL_ACTIVE, 50, 50, 200, 255);
    as_paraGui.setColor(GUI_OBJECT_BUTTON, "ALL", GUI_COL_INACTIVE, 50, 50, 100, 255);
    as_paraGui.setColor(GUI_OBJECT_BUTTON, "ALL", GUI_COL_ACTIVE_LABEL, 250, 250, 230, 255);
    as_paraGui.setColor(GUI_OBJECT_BUTTON, "ALL", GUI_COL_INACTIVE_LABEL, 150, 150, 130, 255);

    as_paraGui.setColor(GUI_OBJECT_SLIDER, "ALL", GUI_COL_ACTIVE, 50, 50, 200, 255);
    as_paraGui.setColor(GUI_OBJECT_SLIDER, "ALL", GUI_COL_INACTIVE, 50, 50, 100, 255);
    as_paraGui.setColor(GUI_OBJECT_SLIDER, "ALL", GUI_COL_ACTIVE_LABEL, 250, 250, 230, 255);
    as_paraGui.setColor(GUI_OBJECT_SLIDER, "ALL", GUI_COL_INACTIVE_LABEL, 150, 150, 130, 255);

    as_paraGui.setColor(GUI_OBJECT_CHECKBOX, "ALL", GUI_COL_ACTIVE, 50, 50, 200, 255);
    as_paraGui.setColor(GUI_OBJECT_CHECKBOX, "ALL", GUI_COL_INACTIVE, 50, 50, 100, 255);
    as_paraGui.setColor(GUI_OBJECT_CHECKBOX, "ALL", GUI_COL_ACTIVE_LABEL, 250, 250, 230, 255);
    as_paraGui.setColor(GUI_OBJECT_CHECKBOX, "ALL", GUI_COL_INACTIVE_LABEL, 150, 150, 130, 255);

    as_paraGui.setColor(GUI_OBJECT_LABEL, "ALL", GUI_COL_ACTIVE, 250, 250, 250, 255);
    as_paraGui.setColor(GUI_OBJECT_TEXTBOX, "ALL", GUI_COL_ACTIVE, 200, 200, 200, 255);

    as_paraGui.setColor(GUI_OBJECT_DIALOGBOX, "ALL", GUI_COL_ACTIVE, 250, 250, 250, 240);
    as_paraGui.setColor(GUI_OBJECT_DIALOGBOX, "ALL", GUI_COL_ACTIVE_LABEL, 251, 78, 18, 254);
    as_paraGui.setColor(GUI_OBJECT_TEXTBOX, "quitGameDialogbox.text", GUI_COL_ACTIVE, 251, 78, 18, 254);
}