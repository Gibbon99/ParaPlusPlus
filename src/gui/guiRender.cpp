#include <gui/guiSlider.h>
#include <gui/guiLabel.h>
#include <gui/guiCheckbox.h>
#include "gui/guiRender.h"
#include "gui/guiButton.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Draw an element
void gui_drawObject (int objectType, int whichObject, bool hasFocus)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			gui_renderButton (whichObject, hasFocus);
			break;

		case GUI_OBJECT_SLIDER:
			gui_renderSlider(whichObject, hasFocus);
			break;

		case GUI_OBJECT_LABEL:
			gui_renderLabel(whichObject, hasFocus);
			break;

		case GUI_OBJECT_CHECKBOX:
			gui_renderCheckbox(whichObject, hasFocus);
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Entry function for drawing the GUI elements
void gui_renderGUI ()
//----------------------------------------------------------------------------------------------------------------------
{
	int           indexCount = 0;
	Uint8         r, g, b, a;
	SDL_BlendMode tempMode;

	//
	// SDL2_gfx changes the blend mode and draw color
	// as part of its rendering - remember so we can change it back
	SDL_GetRenderDrawColor (renderer.renderer, &r, &g, &b, &a);
	SDL_GetRenderDrawBlendMode (renderer.renderer, &tempMode);

	for (indexCount = 0; indexCount != gui.numElements (); indexCount++)
	{
		if (gui.selectedObject () == indexCount)
			gui_drawObject (gui.typeByIndex (indexCount), gui.indexByIndex (indexCount), true);
		else
			gui_drawObject (gui.typeByIndex (indexCount), gui.indexByIndex (indexCount), false);
	}

	SDL_SetRenderDrawColor (renderer.renderer, r, g, b, a);
	SDL_SetRenderDrawBlendMode (renderer.renderer, tempMode);
}