#include <io/console.h>
#include <system/util.h>
#include "gui/guiLabel.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Render a label
void gui_renderLabel (int whichLabel)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string    fontName;
	__BOUNDING_BOX bb{};
	__PARA_COLOR   color{};

	//
	// Check to see if it's ready to use or not
	if (!gui.isReady (GUI_OBJECT_LABEL, whichLabel))
		return;
	//
	// Get the font to use to render the label
	fontName = gui.getFontName (GUI_OBJECT_LABEL, whichLabel);
	if (fontName.size () == 0)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get font for label [ %i ]", whichLabel));
		return;
	}
	fontClass.use (fontName);
	//
	// Get the position to render the label
	bb = gui.getBB (GUI_OBJECT_LABEL, whichLabel);
	if ((bb.x1 == -1) || (bb.y1 == -1) || (bb.x2 == -1) || (bb.y2 == -1))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get position for label [ %i ]", whichLabel));
		return;
	}
	color = gui.getColor (GUI_OBJECT_LABEL, whichLabel, GUI_COL_ACTIVE);
	if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for label [ %i ]", whichLabel));
		return;
	}

	fontClass.render(renderer.renderer, bb.x1, bb.y1, color.r, color.g, color.b, color.a, gui.getLabelText(GUI_OBJECT_LABEL, whichLabel));
}