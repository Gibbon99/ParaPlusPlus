enum PARA_GUI_OBJECTS
{
	GUI_OBJECT_SCREEN = 0,
	GUI_OBJECT_BUTTON,
	GUI_OBJECT_SLIDER,
	GUI_OBJECT_LABEL,
	GUI_OBJECT_SCROLLBOX,
	GUI_OBJECT_CHECKBOX,
	GUI_OBJECT_TEXTBOX,
	GUI_OBJECT_IMAGE,
	GUI_OBJECT_DIALOGBOX,
	NO_DIALOG_BOX,
	GUI_OBJECT_LAST
};

enum PARA_KEY_STATE
{
	PARA_KEY_UP = 0,
	PARA_KEY_DOWN
};

enum PARA_GUI_CODES
{
	GUI_OBJECT_NOT_FOUND = -1,
};

enum PARA_LANGUAGE_TYPES
{
	LANG_ENGLISH = 1,
	LANG_ITALIAN,
	LANG_GERMAN
};

enum PARA_COORD_TYPES
{
	GUI_COORD_PERCENT = 0,
	GUI_COORD_ABSOLUTE,

	GUI_LABEL_CENTER,
	GUI_LABEL_RIGHT,
	GUI_LABEL_LEFT,

	GUI_COL_ACTIVE,
	GUI_COL_INACTIVE,
	GUI_COL_ACTIVE_LABEL,
	GUI_COL_INACTIVE_LABEL,
};

enum PARA_ACTION_SOURCE
{
	KEY_ACTION_NO_SOURCE = 0,
	KEY_ACTION_KEYBOARD,
	KEY_ACTION_MOUSE,
	KEY_ACTION_JOYSTICK
};

enum PARA_KEY_NAMES
{
	KEY_FIRST = 0,
	KEY_LEFT,
	KEY_RIGHT,
	KEY_DOWN,
	KEY_UP,
	KEY_ACTION,
	KEY_PAUSE,
	KEY_ESCAPE,
	KEY_CONSOLE,
	KEY_SCREENSHOT,
	KEY_NUMBER_ACTIONS
};

enum PARA_SIDEVIEW_COLORS
{
	SIDEVIEW_SHIP_COLOR = 0,
	SIDEVIEW_ACTIVE_DECK_COLOR,
	SIDEVIEW_ENGINE_COLOR,
	SIDEVIEW_LIFT_COLOR,
	SIDEVIEW_ACTIVE_LIFT_COLOR,
	SIDEVIEW_NUM_COLORS,
};

enum PARA_EVENT_TYPES
{
	EVENT_TYPE_CONSOLE,
	EVENT_TYPE_LOGFILE,
	EVENT_TYPE_GAME,
};

enum PARA_EVENT_ACTIONS
{
	EVENT_ACTION_LOGFILE_OPEN = 0,
	EVENT_ACTION_LOGFILE_WRITE,
	EVENT_ACTION_LOGFILE_CLOSE,

	EVENT_ACTION_CONSOLE_ADD_LINE,
	EVENT_ACTION_CONSOLE_ADD_CHAR,
	EVENT_ACTION_CONSOLE_DELETE_CHAR,
	EVENT_ACTION_CONSOLE_ADD_CHAR_LINE,

	EVENT_ACTION_GAME_USE_NEW_RENDERER,
	EVENT_ACTION_GAME_SCRIPT_RESTART,
	EVENT_ACTION_GAME_LOAD_TEXTURE,
	EVENT_ACTION_GAME_LOAD_DECK,
	EVENT_ACTION_GAME_LOAD_FONT,
	EVENT_ACTION_GAME_CHANGE_MODE,
	EVENT_ACTION_GAME_CHECK_DECK_CLEAR,
	EVENT_ACTION_GAME_WON,
	EVENT_ACTION_GAME_OVER,
	EVENT_ACTION_DAMAGE_TO_DROID,
	EVENT_ACTION_DROID_COLLISION,
	EVENT_ACTION_CREATE_BULLET,
	EVENT_ACTION_REMOVE_BULLET,
	EVENT_ACTION_ADD_EMITTER,
	EVENT_ACTION_ADD_LIGHTMAP,
	EVENT_ACTION_INIT_TRANSFER_MODE,
	EVENT_ACTION_INIT_TRANSFER_TWO,

	EVENT_ACTION_AUDIO_INIT,
	EVENT_ACTION_AUDIO_PLAY,
	EVENT_ACTION_AUDIO_STOP_ALL,
	EVENT_ACTION_AUDIO_LOAD,
	EVENT_ACTION_AUDIO_SET_MASTER_VOLUME,
	EVENT_ACTION_AUDIO_STOP,
	EVENT_ACTION_AUDIO_START_BACKGROUND,

	EVENT_ACTION_END_LOST_SCREEN,

	EVENT_ACTION_START_BLINK_TIMER,
	EVENT_ACTION_STOP_BLINK_TIMER,

	EVENT_ACTION_START_BACKGROUND_SOUND
};

enum PARA_VARIABLE_TYPES
{
	VAR_TYPE_INT = 0,
	VAR_TYPE_FLOAT,
	VAR_TYPE_BOOL,
	VAR_TYPE_STRING,
	VAR_TYPE_LAST
};

enum PARA_CURRENT_MODES
{
	MODE_CONSOLE_EDIT = 300,
	MODE_CONSOLE_INIT,
	MODE_SHOW_SPLASH,
	MODE_GUI_INTROSCROLL,
	MODE_GUI_MAINMENU,
	MODE_GUI_TERMINAL,
	MODE_GUI_SHIPVIEW,
	MODE_GUI_DECKVIEW,
	MODE_GUI_DATABASE,
	MODE_GUI_LIFTVIEW,
	MODE_GAME,
	MODE_PRE_GAME,
	MODE_PRE_TRANSFER,
	MODE_TRANSFER_SCREEN_ONE,
	MODE_TRANSFER_PRE_SCREEN_TWO,
	MODE_TRANSFER_SCREEN_TWO,
	MODE_TRANSFER_SELECT_SIDE,
	MODE_PRE_TRANSFER_CHOOSE_SIDE,
	MODE_TRANSFER_CHOOSE_SIDE,
	MODE_PRE_TRANSFER_GAME,
	MODE_TRANSFER_GAME,
	MODE_GUI_TUT_TRANSFER_START,
	MODE_GUI_TUT_TRANSFER_GAME,
	MODE_TRANSFER_DEADLOCK,
	MODE_GUI_TUT_LIFTS,
	MODE_GUI_TUT_TERMINALS,
	MODE_GUI_TUT_HEALING,
	MODE_GUI_TUT_TIPS,
	MODE_GAME_PAUSE_ON,
	MODE_GAME_PAUSE_OFF,
	MODE_END_PRE_LOST_SCREEN,
	MODE_END_LOST_SCREEN,
	MODE_GUI_WON_SCREEN,
	MODE_GUI_PRE_HIGHSCORE_SCREEN,
	MODE_TRANSFER_RESULT,
	MODE_GUI_KEYCODE_ENTRY,
	MODE_GUI_HIGHSCORE_DISPLAY,
	MODE_GUI_HIGHSCORE_ENTRY,

	MODE_GAME_OVER,
};

enum PARA_SLIDER_TYPES
{
	SLIDER_TYPE_INT = 0,
	SLIDER_TYPE_FLOAT,
	SLIDER_TYPE_STRING
};

enum PARA_BULLET_TYPES
{
	BULLET_TYPE_NORMAL = 0,
	BULLET_TYPE_SINGLE,
	BULLET_TYPE_DOUBLE,
	BULLET_TYPE_DISRUPTER
};

enum DROID_MODES
{
	DROID_MODE_NORMAL = 0,
	DROID_MODE_EXPLODING,
	DROID_MODE_FOR_REMOVAL,
	DROID_MODE_DEAD
};

enum PHYSICS_DAMAGE_TYPES
{
	PHYSIC_DAMAGE_BUMP = 0,
	PHYSIC_DAMAGE_BULLET,
	PHYSIC_DAMAGE_EXPLOSION
};

enum PARTICLE_TYPES
{
	PARTICLE_TYPE_EXPLOSION = 0,
	PARTICLE_TYPE_SPARK,
	PARTICLE_TYPE_TRAIL
};

enum PARTICLE_RENDER_TYPES
{
	PARTICLE_RENDER_PRIMITIVE = 0,
	PARTICLE_RENDER_BITMAP
};

enum LIGHTMAP_TYPES
{
	LIGHTMAP_TYPE_ALERT = 0,
	LIGHTMAP_TYPE_BULLET,
	LIGHTMAP_TYPE_SPARK,
	LIGHTMAP_TYPE_EXPLOSION
};

enum VISIBLE_TYPE_STATES
{
	VISIBLE_STATE_GO_NOT_VISIBLE = 0,
	VISIBLE_STATE_GO_VISIBLE,
	VISIBLE_STATE_IS_VISIBLE
};

enum TRANSFER_ROW_TYPES
{
	TRANSFER_ROW_FULL_LINE = 0,
	TRANSFER_ROW_HALF_LINE,
	TRANSFER_ROW_3_4_LINE,
	TRANSFER_ROW_FULL_LINE_1,
	TRANSFER_ROW_QUARTER_LINE,
	TRANSFER_ROW_REPEAT_HALF,
	TRANSFER_ROW_REPEAT_QUARTER,
	TRANSFER_ROW_FULL_LINE_2,
	TRANSFER_ROW_REVERSE_HALF,
	TRANSFER_ROW_REVERSE_QUARTER,
	TRANSFER_ROW_TWO_INTO_ONE_MIDDLE,
	TRANSFER_ROW_ONE_INTO_TWO_MIDDLE,
	TRANSFER_ROW_FULL_LINE_3,
	TRANSFER_ROW_ONE_INTO_TWO_TOP,
	TRANSFER_ROW_ONE_INTO_TWO_BOTTOM,
	TRANSFER_ROW_TWO_INTO_ONE_TOP,
	TRANSFER_ROW_TWO_INTO_ONE_BOTTOM,
	TRANSFER_ROW_LAUNCHER_LEFT,
	TRANSFER_ROW_LAUNCHER_RIGHT,
	TRANSFER_ROW_LAUNCHER_RIGHT_COLOR,
	TRANSFER_ROW_LAUNCHER_LEFT_COLOR
};

enum TRANSFER_COLORS
{
	TRANSFER_COLOR_LEFT = 0,
	TRANSFER_COLOR_RIGHT,
	TRANSFER_COLOR_BACKGROUND,
	TRANSFER_COLOR_BORDER,
	TRANSFER_COLOR_DISPUTE
};

enum ATTACK_NAMES
{
	NO_ATTACK_TARGET = -2,
	TARGET_PLAYER = -1
};

enum HIGHSCORE_ROWS
{
	NUM_HIGHSCORE_ROWS = 5
};