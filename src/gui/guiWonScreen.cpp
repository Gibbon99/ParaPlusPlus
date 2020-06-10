#include <system/util.h>
#include <game/hud.h>
#include "gui/guiWonScreen.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Prepare the Won Screen
void gui_prepareWonScreen()
//----------------------------------------------------------------------------------------------------------------------
{
	gam_setHudText ("wonGameOver");

	sys_setNewMode (MODE_GUI_WON_SCREEN, true);
}