#include <gui/guiDialogBox.h>
#include "gui/guiRender.h"
#include "gui/guiButton.h"
#include "gui/guiSlider.h"
#include "gui/guiLabel.h"
#include "gui/guiCheckbox.h"
#include "gui/guiTextbox.h"
#include "gui/guiImage.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Draw an element
void gui_drawObject(int objectType, int whichObject, bool hasFocus)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			gui_renderButton (whichObject, hasFocus);
			break;

		case GUI_OBJECT_SLIDER:
			gui_renderSlider (whichObject, hasFocus);
			break;

		case GUI_OBJECT_LABEL:
			gui_renderLabel (whichObject);
			break;

		case GUI_OBJECT_CHECKBOX:
			gui_renderCheckbox (whichObject, hasFocus);
			break;

		case GUI_OBJECT_TEXTBOX:
			gui_renderTextbox (whichObject);
			break;

		case GUI_OBJECT_IMAGE:
			gui_renderImage (whichObject);
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Entry function for drawing the current dialogbox
void gui_renderActiveDialogbox()
//----------------------------------------------------------------------------------------------------------------------
{
	gui_renderDialogbox (gui.getCurrentDialogbox ());

	for (int indexCount = 0; indexCount != gui.numElementsDialogbox (); indexCount++)
	{
		if (gui.selectedObjectDialogbox () == indexCount)
			gui_drawObject (gui.typeByIndexDialogbox (indexCount), gui.indexByIndexDialogbox (indexCount), true);
		else
			gui_drawObject (gui.typeByIndexDialogbox (indexCount), gui.indexByIndexDialogbox (indexCount), false);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Entry function for drawing the GUI elements
void gui_renderGUI()
//----------------------------------------------------------------------------------------------------------------------
{
	for (int indexCount = 0; indexCount != gui.numElements (); indexCount++)
	{
		if (gui.selectedObject () == indexCount)
			gui_drawObject (gui.typeByIndex (indexCount), gui.indexByIndex (indexCount), true);
		else
			gui_drawObject (gui.typeByIndex (indexCount), gui.indexByIndex (indexCount), false);
	}
}