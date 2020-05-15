#include <io/console.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <system/util.h>
#include "gui/guiCheckbox.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Render a checkbox
void gui_renderCheckbox (int whichObject, bool hasFocus)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string    fontName;
	__BOUNDING_BOX bb{};
	__PARA_COLOR   color{};
	__PARA_COLOR   labelColor{};
	int            cornerRadius;
	int            labelPos;
	int            gapSize;
	double         textPosX;
	double         textPosY;

	//
	// Check to see if it's ready to use or not
	if (!gui.isReady (GUI_OBJECT_CHECKBOX, whichObject))
		return;

	//
	// Get the font to use to render the label
	fontName = gui.getFontName (GUI_OBJECT_CHECKBOX, whichObject);
	if (fontName.size () == 0)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get font for checkbox [ %i ]", whichObject));
		return;
	}
	fontClass.use (fontName);
	//
	// Get the position to render the button
	bb = gui.getBB (GUI_OBJECT_CHECKBOX, whichObject);
	if ((bb.x1 == -1) || (bb.y1 == -1) || (bb.x2 == -1) || (bb.y2 == -1))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get position for checkbox [ %i ]", whichObject));
		return;
	}

	//
	// Get the radius used to draw the corners
	cornerRadius = gui.getRadius (GUI_OBJECT_CHECKBOX, whichObject);
	if (PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND == cornerRadius)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get radius for checkbox [ %i ]", whichObject));
		return;
	}

	//
	// Get the color for active or inactive focus
	if (hasFocus)
	{
		color = gui.getColor (GUI_OBJECT_CHECKBOX, whichObject, GUI_COL_ACTIVE);
		if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for checkbox [ %i ]", whichObject));
			return;
		}
	}
	else
	{
		color = gui.getColor (GUI_OBJECT_CHECKBOX, whichObject, GUI_COL_INACTIVE);
		if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for checkbox [ %i ]", whichObject));
			return;
		}
	}
	//
	// Get the label color for active or inactive focus
	if (hasFocus)
	{
		labelColor = gui.getColor (GUI_OBJECT_CHECKBOX, whichObject, GUI_COL_ACTIVE_LABEL);
		if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for checkbox [ %i ]", whichObject));
			return;
		}
	}
	else
	{
		labelColor = gui.getColor (GUI_OBJECT_CHECKBOX, whichObject, GUI_COL_INACTIVE_LABEL);
		if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for checkbox [ %i ]", whichObject));
			return;
		}
	}

	labelPos = gui.getLabelPos (GUI_OBJECT_CHECKBOX, whichObject);
	if (PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND == labelPos)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get the label position for checkbox [ %i ]", whichObject));
		return;
	}

	//
	// Spacing between box and label
	gapSize = gui.getGapSize (GUI_OBJECT_CHECKBOX, whichObject);
	if (PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND == gapSize)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get the gap size for checkbox [ %i ]", whichObject));
		return;
	}

	auto fontHeight = fontClass.height();

	textPosY = bb.y1 + abs((((bb.y2 - bb.y1) - fontHeight) / 2));

	switch (labelPos)
	{
		case GUI_LABEL_CENTER:
			textPosX = bb.x2 + gapSize;
			break;

		case GUI_LABEL_LEFT:
			textPosX = bb.x2 + gapSize;
			break;

		case GUI_LABEL_RIGHT:
			textPosX = bb.x2 + gapSize;
			break;
	}

	//
	// Render the tickbox
	if (gui.getTickedStatus (whichObject))
	{
		thickLineRGBA (renderer.renderer, bb.x1, bb.y1, bb.x2, bb.y1, 3, color.r, color.g, color.b, color.a);
		thickLineRGBA (renderer.renderer, bb.x2, bb.y1, bb.x2, bb.y2, 3, color.r, color.g, color.b, color.a);
		thickLineRGBA (renderer.renderer, bb.x1, bb.y2, bb.x2, bb.y2, 3, color.r, color.g, color.b, color.a);
		thickLineRGBA (renderer.renderer, bb.x1, bb.y1, bb.x1, bb.y2, 3, color.r, color.g, color.b, color.a);

		roundedBoxRGBA (renderer.renderer, bb.x1 + 7, bb.y1 + 7, bb.x2 - 7, bb.y2 - 7, cornerRadius, color.r, color.g, color.b, color.a);
	}
	else
	{
		thickLineRGBA (renderer.renderer, bb.x1, bb.y1, bb.x2, bb.y1, 3, color.r, color.g, color.b, color.a);
		thickLineRGBA (renderer.renderer, bb.x2, bb.y1, bb.x2, bb.y2, 3, color.r, color.g, color.b, color.a);
		thickLineRGBA (renderer.renderer, bb.x1, bb.y2, bb.x2, bb.y2, 3, color.r, color.g, color.b, color.a);
		thickLineRGBA (renderer.renderer, bb.x1, bb.y1, bb.x1, bb.y2, 3, color.r, color.g, color.b, color.a);
	}

	//
	// Render the label
	std::string labelText = gui.getLabelText(GUI_OBJECT_CHECKBOX, whichObject);
	fontClass.render(renderer.renderer, textPosX, textPosY, labelColor.r, labelColor.g, labelColor.b, labelColor.a, labelText);
}