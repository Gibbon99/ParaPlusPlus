//--------------------------------------------------------------------------------------------------------------------
//
// " ! " - is a line break in a textbox = needs a space either side
//
// Create English text strings
void as_setEnglishText ()
//--------------------------------------------------------------------------------------------------------------------
{
	gui_addKeyAndText ("scrollBoxIntroText", "Welcome to Paradroid. ! ! Press [ LEFT CTRL KEY ] to play or wait for further instructions ! Use the [ ARROW KEYS ] to navigate the menus ! Press [ LEFT CTRL KEY ] to make a choice ! ! ^ Clear the freighter of robots by destroying them with twin lasers or by transferring control to them. ! ! Control is by keyboard only, as follows: ! ! At all times the keyboard moves the robot and holding fire down will allow use of lifts and consoles. ! ! In addition, pressing fire with no keys pressed will prepare the current robot for transfer. Contact with another robot with the fire key down will initiate transfer. ! ! ! ^ ! ! ! ! A fleet of Robo-Freighters on its way to the Beta Ceti system reported entering an uncharted field of asteroids. Each ship carries a cargo of battle droids to reinforce the outworld defences. ! ! Two distress beacons have been recovered. Similar messages were stored on each. The ships had been bombarded by a powerful radionic beam from one of the asteroids. ! ! All of the robots on the ships, including those in storage, became hyper-active. The crews report an attack by droids, isolating them on the bridge. They cannot reach the shuttle and can hold out for only a couple more hours. ! ! Since these beacons were located two days ago, we can only fear the worst. ! ! Some of the fleet was last seen heading for enemy space. In enemy hands the droids can be used against our forces. ! ! Docking would be impossible, but we can beam aboard a prototype Influence Device. ! ^  ! ! ! The 001 Influence device consists of a helmet, which, when placed over a robot's control unit can halt the normal activities of that robot for a short time. The helmet has its own power supply and powers the robot itself, at an upgraded capability. The helmet also uses an energy cloak for protection of the host. ! ! The helmet is fitted with twin lasers mounted in a turrent. These are low powered and have a slow recycle rate. ! ! Most of the device's resources are channeled towards holding control of the host robot, as it attempts to resume 'normal' operation. It is therefore necessary to change the host robot often to prevent the device from burning out. Transfer to a new robot requires the device to drain its host of energy in order to take it over. Failure to achieve transfer results in the device being a free agent once more. ! ! Further information incoming... ! ^  ! ! ! An Influence device can transmit only certain data, namely its own location and the location of other robots in visual range. This data is merged with known ship layouts on your C64 remote terminal. ! ! Additional information about the ship and robots may be obtained by accessing the ship's computer at a console. A small-scale plan of the whole deck is available, as well as a side elevation of the ship. ! ! Robots are represented on-screen as a symbol showing a three-digit number. The first digit shown is the important one, the class of the robot. It denotes strength also. ! ! To find out more about any given robot, use the robot enquiry system at a console. Only data about units of a lower class than your current host is available, since it is the host's security clearance which is used to access the console. ! ! ! ! ! Further information incoming... ! ^  ! ! ! ! ! Paradroid programmed by Andrew Braybrook. ! ! ! Sound advice by Steve Turner. ! ! ! SDL2 port by David Berry. ! 2019. ! ! ! ! ! Powered by the SDL2.0 library. ! Scripting engine by AngelScript. ! Physics by Box2D. ! Filewatcher by James Wynn 2009. ! ! ! ! Press fire to play. ! ! ! ! ! ! ^ # ");
	gui_addKeyAndText ("gameLeft", "Move left");
	gui_addKeyAndText ("gameRight", "Move Right");
	gui_addKeyAndText ("gameDown", "Move down");
	gui_addKeyAndText ("gameUp", "Move up");
	gui_addKeyAndText ("gameAction", "Perform an action");
	gui_addKeyAndText ("gamePause", "Pause the game");
	gui_addKeyAndText ("gameEscape", "Escape");
	gui_addKeyAndText ("consoleAction", "Invoke the console");
	gui_addKeyAndText ("gameScreenShot", "Take a screenshot");

	gui_addKeyAndText ("backButton", "Back");

	gui_addKeyAndText ("optionsMenu.videoButton", "Video");
	gui_addKeyAndText ("optionsMenu.audioButton", "Audio");
	gui_addKeyAndText ("optionsMenu.controlsButton", "Controls");
	gui_addKeyAndText ("optionsMenu.gameButton", "Game");
	gui_addKeyAndText ("audioOptions.volume", "Volume");
	gui_addKeyAndText ("audioOptions.enableSound", "Enable sound");
	gui_addKeyAndText ("audioOptions.speakers", "Speakers");
	gui_addKeyAndText ("audioOptions.heading", "AUDIO OPTIONS");

	gui_addKeyAndText ("videoOptions.heading", "VIDEO OPTIONS");
	gui_addKeyAndText ("videoOptions.renderers", "Renderer");
	gui_addKeyAndText ("videoOptions.display", "Display");
	gui_addKeyAndText ("videoOptions.windowed", "Window");
	gui_addKeyAndText ("videoOptions.fullscreen", "Fullscreen");
	gui_addKeyAndText ("videoOptions.desktop", "Full Desktop");

	gui_addKeyAndText ("videoOptions.nearest", "Nearest");
	gui_addKeyAndText ("videoOptions.linear", "Linear");
	gui_addKeyAndText ("videoOptions.best", "Best");
	gui_addKeyAndText ("videoOptions.scalequality", "Scale quality");

	gui_addKeyAndText ("videoOptions.usevsync", "Use vsync");
	gui_addKeyAndText ("videoOptions.borderlesswindow", "No window frame");
	gui_addKeyAndText ("videoOptions.highdpi", "Allow High DPI");
	gui_addKeyAndText ("videoMenu.applyButton", "Apply");

	gui_addKeyAndText ("gameOptions.heading", "GAME OPTIONS");

	gui_addKeyAndText ("controlsOptions.heading", "CONTROLS");

	gui_addKeyAndText ("mainMenu.startGameButton", "Start Game");
	gui_addKeyAndText ("mainMenu.optionsButton", "Options");
	gui_addKeyAndText ("mainMenu.tutorialButton", "Tutorial");
	gui_addKeyAndText ("mainMenu.briefingButton", "Briefing");
	gui_addKeyAndText ("mainMenu.quitButton", "Quit Game");
}

