#include <system/util.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include "gui/dialogBox.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Render a dialog box - over the top of everything else
void gui_renderDialogbox (int whichDialogbox)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string    fontName;
	__BOUNDING_BOX bb{};
	__PARA_COLOR   dialogboxColor{};
	__PARA_COLOR   headingColor{};
	int            cornerRadius;
	int            gapSize;
	float          ratioX;
	float          ratioY;

	ratioX = static_cast<float>(gameWinWidth) / static_cast<float>(windowWidth);
	ratioY = static_cast<float>(gameWinHeight) / static_cast<float>(windowHeight);
	//
	// Check to see if it's ready to use or not
	if (!gui.isReady (GUI_OBJECT_DIALOGBOX, whichDialogbox))
		return;

	//
	// Get the font to use to render the label
	fontName = gui.getFontName (GUI_OBJECT_DIALOGBOX, whichDialogbox);
	if (fontName.size () == 0)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get font for dialogbox [ %i ]", whichDialogbox));
		return;
	}
	fontClass.use (fontName);
	//
	// Get the offset for the heading
	gapSize = gui.getGapSize (GUI_OBJECT_DIALOGBOX, whichDialogbox);
	if (gapSize == PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get gapSize for dialogbox [ %i ]", whichDialogbox));
		return;
	}
	//
	// Get the position to render the dialogbox
	bb = gui.getBB (GUI_OBJECT_DIALOGBOX, whichDialogbox);
	if ((bb.x1 == PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND) || (bb.y1 == PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND) || (bb.x2 == PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND) || (bb.y2 == PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get position for dialogbox [ %i ]", whichDialogbox));
		return;
	}
	//
	// Get the radius used to draw the corners
	cornerRadius = gui.getRadius (GUI_OBJECT_DIALOGBOX, whichDialogbox);
	if (PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND == cornerRadius)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get radius for dialogbox [ %i ]", whichDialogbox));
		return;
	}
	dialogboxColor = gui.getColor (GUI_OBJECT_DIALOGBOX, whichDialogbox, GUI_COL_ACTIVE);
	if ((dialogboxColor.r == PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND) || (dialogboxColor.g == PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND) || (dialogboxColor.b == PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND) ||
	    (dialogboxColor.a == PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for dialogbox [ %i ]", whichDialogbox));
		return;
	}

	headingColor = gui.getColor (GUI_OBJECT_DIALOGBOX, whichDialogbox, GUI_COL_ACTIVE_LABEL);
	if ((headingColor.r == PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND) || (headingColor.g == PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND) || (headingColor.b == PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND) ||
	    (headingColor.a == PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for dialogbox [ %i ]", headingColor));
		return;
	}
	//
	// Render the dialogbox
	roundedBoxRGBA (renderer.renderer, bb.x1 * ratioX, bb.y1 * ratioY, bb.x2 * ratioX, bb.y2 * ratioY, cornerRadius, dialogboxColor.r, dialogboxColor.g, dialogboxColor.b, dialogboxColor.a);

	hlineRGBA (renderer.renderer, (bb.x1 + gapSize) * ratioX, (bb.x2 - gapSize) * ratioX, (bb.y1 + fontClass.height() - gapSize) * ratioY, headingColor.r,headingColor.g,headingColor.b,255);

	fontClass.render(renderer.renderer, bb.x1 + gapSize, bb.y1 + gapSize, headingColor.r, headingColor.g, headingColor.b, headingColor.a, gui.getLabelText(GUI_OBJECT_DIALOGBOX, whichDialogbox));
}