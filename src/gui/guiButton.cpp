#include <io/console.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <io/logFile.h>
#include "gui/guiButton.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Draw a button element
void gui_renderButton (int whichButton, bool hasFocus)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string    fontName;
	__BOUNDING_BOX bb{};
	__PARA_COLOR   color{};
	int            cornerRadius;
	int            labelPos;
	int            gapSize;
	double         textPosX;
	double         textPosY;

	PARA_Surface      *tempSurface  = nullptr;
	PARA_Texture      *tempTexture  = nullptr;

	//
	// Check to see if it's ready to use or not
	if (!gui.isReady (GUI_OBJECT_BUTTON, whichButton))
		return;

	//
	// Get the font to use to render the label
	fontName = gui.getFontName (GUI_OBJECT_BUTTON, whichButton);
	if (fontName.size () == 0)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get font for button [ %i ]", whichButton));
		return;
	}
	fontClass.use (fontName);
	//
	// Get the position to render the button
	bb = gui.getBB (GUI_OBJECT_BUTTON, whichButton);
	if ((bb.x1 == -1) || (bb.y1 == -1) || (bb.x2 == -1) || (bb.y2 == -1))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get position for button [ %i ]", whichButton));
		return;
	}
	//
	// Get the radius used to draw the corners
	cornerRadius = gui.getRadius (GUI_OBJECT_BUTTON, whichButton);
	if (PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND == cornerRadius)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get radius for button [ %i ]", whichButton));
		return;
	}

	//
	// Get the color for active or inactive focus
	if (hasFocus)
	{
		color = gui.getColor (GUI_OBJECT_BUTTON, whichButton, GUI_COL_ACTIVE);
		if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for button [ %i ]", whichButton));
			return;
		}
	}
	else
	{
		color = gui.getColor (GUI_OBJECT_BUTTON, whichButton, GUI_COL_INACTIVE);
		if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for button [ %i ]", whichButton));
			return;
		}
	}
	//
	// Render the button
	roundedBoxRGBA (renderer.renderer, bb.x1, bb.y1, bb.x2, bb.y2, cornerRadius, color.r, color.g, color.b, color.a);

	//
	// Now render the text label
	//

	labelPos = gui.getLabelPos (GUI_OBJECT_BUTTON, whichButton);
	if (PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND == labelPos)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get the label position for button [ %i ]", whichButton));
		return;
	}
	gapSize = gui.getGapSize (GUI_OBJECT_BUTTON, whichButton);
	if (PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND == gapSize)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get the gap size for button [ %i ]", whichButton));
		return;
	}

	const auto buttonHeight = (bb.y2 - bb.y1);
	const auto fontHeight = fontClass.height();

	std::string searchIn = gui.getLabelText (GUI_OBJECT_BUTTON, whichButton);
	if ((searchIn.find("q")) && (searchIn.find("j")) && (searchIn.find("p")) && (searchIn.find("y")) && (searchIn.find("g")) == std::string::npos)  // not in string
		textPosY = bb.y1 + ( (buttonHeight - (fontHeight + fontClass.descent())) / 2);
	else
		textPosY = bb.y1 + ( (buttonHeight - fontHeight) / 2);

	switch (labelPos)
	{
		case GUI_LABEL_CENTER:
			textPosX = bb.x1 + (((bb.x2 - bb.x1) - fontClass.width (gui.getLabelText (GUI_OBJECT_BUTTON, whichButton))) / 2);
			break;

		case GUI_LABEL_LEFT:
			textPosX = bb.x1 + gapSize;
			break;

		case GUI_LABEL_RIGHT:
			textPosX = (bb.x2 - fontClass.width (gui.getLabelText (GUI_OBJECT_BUTTON, whichButton))) - gapSize;
			break;
	}
	//
	// Get the color for the text label
	if (hasFocus)
	{
		color = gui.getColor (GUI_OBJECT_BUTTON, whichButton, GUI_COL_ACTIVE_LABEL);
		if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for button label [ %i ]", whichButton));
			return;
		}
	}
	else
	{
		color = gui.getColor (GUI_OBJECT_BUTTON, whichButton, GUI_COL_INACTIVE_LABEL);
		if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for button label [ %i ]", whichButton));
			return;
		}
	}

	fontClass.setColor(color.r, color.g, color.b, color.a);
	tempSurface = fontClass.write(textPosX, textPosY, gui.getLabelText (GUI_OBJECT_BUTTON, whichButton));
	if (nullptr == tempSurface)
	{
		log_addEvent (sys_getString ("%s", "Unable to create temp surface when rendering console."));
		return;
	}
	tempTexture = SDL_CreateTextureFromSurface (renderer.renderer, tempSurface);
	if (nullptr == tempTexture)
	{
		log_addEvent (sys_getString ("%s", "Unable to create temp texture when rendering console."));
		return;
	}
	SDL_RenderCopy (renderer.renderer, tempTexture, nullptr, &fontClass.pos);
	SDL_DestroyTexture (tempTexture);
}