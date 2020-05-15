#include <io/console.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <system/util.h>
#include "gui/guiScrollbox.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Render a scrollbox
void gui_renderScrollbox (std::string whichScrollbox, double interpolate)
//----------------------------------------------------------------------------------------------------------------------
{
	int            objectIndex   = 0;
	int            cornerRadius  = 0;
	int            lineCount     = 0;
	double         drawPositionY = 0.0;
	double         renderPosX    = 0.0;
	double         renderPosY    = 0.0;
	__BOUNDING_BOX bb{};
	__PARA_COLOR   backGroundColor{};
	__PARA_COLOR   fontColor{};
	std::string    fontName;

	//
	// Find the index for this object
	objectIndex = gui.getIndex (GUI_OBJECT_SCROLLBOX, whichScrollbox);
	if (GUI_OBJECT_NOT_FOUND == objectIndex)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to locate object [ %s ]. Has it been created ?.", whichScrollbox.c_str ()));
		return;
	}

	bb = gui.getBB (GUI_OBJECT_SCROLLBOX, objectIndex);
	if ((bb.x1 == -1) || (bb.y1 == -1) || (bb.x2 == -1) || (bb.y2 == -1))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get boundingbox size for scrollbox [ %i ]", objectIndex));
		return;
	}

	//
	// Get the radius used to draw the corners
	cornerRadius = gui.getRadius (GUI_OBJECT_SCROLLBOX, objectIndex);
	if (PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND == cornerRadius)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get radius for scrollbox [ %i ]", objectIndex));
		return;
	}

	backGroundColor = gui.getColor (GUI_OBJECT_SCROLLBOX, objectIndex, GUI_COL_ACTIVE);
	if ((backGroundColor.r == -1) || (backGroundColor.g == -1) || (backGroundColor.b == -1) || (backGroundColor.a == -1))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for scrollbox [ %i ]", objectIndex));
		return;
	}

	fontColor = gui.getColor (GUI_OBJECT_SCROLLBOX, objectIndex, GUI_COL_ACTIVE_LABEL);
	if ((fontColor.r == -1) || (fontColor.g == -1) || (fontColor.b == -1) || (fontColor.a == -1))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get fontColor for scrollbox [ %i ]", objectIndex));
		return;
	}

	fontName = gui.getFontName (GUI_OBJECT_SCROLLBOX, objectIndex);
	if (fontName.size () == 0)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get fontname for scrollbox [ %i ]", objectIndex));
		return;
	}
	fontClass.use (fontName);
	//
	// Draw background
	roundedBoxRGBA (renderer.renderer, bb.x1, bb.y1, bb.x2, bb.y2, cornerRadius, backGroundColor.r, backGroundColor.g, backGroundColor.b, backGroundColor.a);

	double alphaStep = 255.0 / static_cast<double>(fontClass.height ());
	double lineAlpha = 0.0;

	lineCount = 0;
	if (gui.getScrollDelay (objectIndex) > 0)
	{
		drawPositionY = gui.getScrollY (objectIndex);
	}
	else
	{
		drawPositionY = gui.getScrollY (objectIndex) - gui.getPreviousScrollY (objectIndex);
		drawPositionY *= interpolate;
		drawPositionY += gui.getPreviousScrollY (objectIndex);
	}

	for (auto lineItr = gui.getRBegin (objectIndex); lineItr != gui.getREnd (objectIndex); ++lineItr)
	{
		lineAlpha = fontColor.a;

		if (lineCount == 0)
		{
			gui.setLineFade (objectIndex, gui.getLineFade (objectIndex) + alphaStep);

			if (gui.getLineFade (objectIndex) > fontColor.a)
			{
				gui.setLineFade (objectIndex, fontColor.a);
			}
			lineAlpha = gui.getLineFade (objectIndex);
		}
		else if (lineCount == gui.getNumberPrintLines (objectIndex) - 2)
		{
			lineAlpha = 255.0 - (alphaStep * gui.getScrollY (objectIndex));
		}
	renderPosX = bb.x1; // + gapSize / 2.0

	renderPosY = ((bb.y2 - fontClass.height()) - (lineCount * fontClass.height()) - drawPositionY);

	fontClass.render(renderer.renderer, renderPosX, renderPosY, fontColor.r, fontColor.g, fontColor.b, static_cast<Uint8>(lineAlpha), lineItr->c_str());

	lineCount++;
	if (lineCount == gui.getNumberPrintLines(objectIndex) - 1)
		break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Move the scrollbox text up by one pixel
void gui_scrollOnePixel (int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string    fontName;
	__BOUNDING_BOX bb{};

	fontName = gui.getFontName (GUI_OBJECT_SCROLLBOX, objectIndex);
	if (fontName.size () == 0)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get fontname for scrollbox [ %i ]", objectIndex));
		return;
	}
	fontClass.use (fontName);

	bb = gui.getBB (GUI_OBJECT_SCROLLBOX, objectIndex);
	if ((bb.x1 == -1) || (bb.y1 == -1) || (bb.x2 == -1) || (bb.y2 == -1))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get boundingbox size for scrollbox [ %i ]", objectIndex));
		return;
	}

	gui.setPreviousScrollY (objectIndex, gui.getScrollY (objectIndex));
	gui.setScrollY (objectIndex, gui.getScrollY (objectIndex) + 2.0); //gui.getScrollDelay (objectIndex));

	if (gui.getScrollY (objectIndex) > fontClass.height () - 1)
	{
		gui.setScrollY (objectIndex, 0.0);
		gui.setPreviousScrollY (objectIndex, -1.0);

		gui.getNextLineOfText (objectIndex);
		gui.setNumberPrintLines (objectIndex, gui.getNumberPrintLines (objectIndex) + 1);
		if (gui.getNumberPrintLines (objectIndex) > (bb.y2 - bb.y1) / fontClass.height ())
		{
			gui.setNumberPrintLines (objectIndex, (bb.y2 - bb.y1) / fontClass.height ());
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Move a scrollbox, or pause if required
void gui_scrollScrollBox (std::string whichScrollbox)
//----------------------------------------------------------------------------------------------------------------------
{
	int objectIndex;

	//
	// Find the index for this object
	objectIndex = gui.getIndex (GUI_OBJECT_SCROLLBOX, whichScrollbox);
	if (GUI_OBJECT_NOT_FOUND == objectIndex)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to locate object [ %s ]. Has it been created ?.", whichScrollbox.c_str ()));
		return;
	}

	if (gui.getScrollDelay (objectIndex) > 0)
	{
		gui.setScrollDelay (objectIndex, gui.getScrollDelay (objectIndex) - 1);
		if (gui.getScrollDelay (objectIndex) < 0)
			gui.setScrollDelay (objectIndex, 0);
	}
	else
	{
		gui_scrollOnePixel (objectIndex);
	}
}