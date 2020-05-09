#include <io/console.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include "gui/guiSlider.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Render a slider element
void gui_renderSlider (int whichSlider, bool hasFocus)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string    fontName;
	__BOUNDING_BOX bb{};
	__PARA_COLOR   color{};
	__PARA_COLOR   fontColor{};
	int            cornerRadius;
	double         selectorX;
	double         sliderLabelPosX, sliderLabelPosY;
	double         sliderValuePosX, sliderValuePosY;
	double         gapSize;

	//
	// Check to see if it's ready to use or not
	if (!gui.isReady (GUI_OBJECT_SLIDER, whichSlider))
		return;

	//
	// Get the font to use to render the label
	fontName = gui.getFontName (GUI_OBJECT_SLIDER, whichSlider);
	if (fontName.size () == 0)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get font for slider [ %i ]", whichSlider));
		return;
	}
	fontClass.use (fontName);

	//
	// Get the gap size
	gapSize = gui.getGapSize (GUI_OBJECT_SLIDER, whichSlider);
	if (gapSize == GUI_OBJECT_NOT_FOUND)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get gapSize for slider [ %i ]", whichSlider));
		return;

	}
	//
	// Get the position to render the slider
	bb = gui.getBB (GUI_OBJECT_SLIDER, whichSlider);
	if ((bb.x1 == -1) || (bb.y1 == -1) || (bb.x2 == -1) || (bb.y2 == -1))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get position for slider [ %i ]", whichSlider));
		return;
	}

	//
	// Get the radius used to draw the line ends
	cornerRadius = gui.getRadius (GUI_OBJECT_SLIDER, whichSlider);
	if (PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND == cornerRadius)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get radius for slider [ %i ]", whichSlider));
		return;
	}

	//
	// Get the color for active or inactive focus
	if (hasFocus)
	{
		color = gui.getColor (GUI_OBJECT_SLIDER, whichSlider, GUI_COL_ACTIVE);
		if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for slider [ %i ]", whichSlider));
			return;
		}
	}
	else
	{
		color = gui.getColor (GUI_OBJECT_SLIDER, whichSlider, GUI_COL_INACTIVE);
		if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for slider [ %i ]", whichSlider));
			return;
		}
	}

	//
	// Get the color for active or inactive focus label color
	if (hasFocus)
	{
		fontColor = gui.getColor (GUI_OBJECT_SLIDER, whichSlider, GUI_COL_ACTIVE_LABEL);
		if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for slider label [ %i ]", whichSlider));
			return;
		}
	}
	else
	{
		fontColor = gui.getColor (GUI_OBJECT_SLIDER, whichSlider, GUI_COL_INACTIVE_LABEL);
		if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for slider label [ %i ]", whichSlider));
			return;
		}
	}

	if (gui.getSelectPosition (whichSlider) == 0)
		selectorX = bb.x1;      // position at the start
	else if (gui.getSelectPosition (whichSlider) == gui.sliderSize (whichSlider))
		selectorX = bb.x2;    // position at the end
	else
	{
		selectorX = ( ((bb.x2 - bb.x1) / (gui.getNumElements (whichSlider) - 1 )) * gui.getSelectPosition (whichSlider) ) + bb.x1;
	}

	sliderLabelPosX = bb.x1 + (((bb.x2 - bb.x1) - fontClass.width (gui.getLabelText (GUI_OBJECT_SLIDER, whichSlider))) / 2);
	sliderValuePosX = bb.x1 + (((bb.x2 - bb.x1) - fontClass.width (gui.sliderElementLabel (whichSlider))) / 2);

	sliderLabelPosY = bb.y2 + gapSize;
	sliderValuePosY = bb.y1 - (fontClass.height () + gapSize);

	fontClass.render (renderer.renderer, sliderLabelPosX, sliderLabelPosY, fontColor.r, fontColor.g, fontColor.b, fontColor.a, gui.getLabelText (GUI_OBJECT_SLIDER, whichSlider));
	fontClass.render (renderer.renderer, sliderValuePosX, sliderValuePosY, fontColor.r, fontColor.g, fontColor.b, fontColor.a, gui.sliderElementLabel (whichSlider));
	//
	// Render the selector
	filledCircleRGBA (renderer.renderer, selectorX, bb.y1 + ((bb.y2 - bb.y1) / 2), 20, color.r, color.g, color.b, color.a);
	circleRGBA (renderer.renderer, selectorX, bb.y1 + ((bb.y2 - bb.y1) / 2), 18, 0, 0, 0, color.a);
	circleRGBA (renderer.renderer, selectorX, bb.y1 + ((bb.y2 - bb.y1) / 2), 17, 0, 0, 0, color.a);
	//
	// Render the line
	roundedBoxRGBA (renderer.renderer, bb.x1, bb.y1, bb.x2, bb.y2, cornerRadius, color.r, color.g, color.b, color.a);
}

