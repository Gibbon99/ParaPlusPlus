#include <cstdarg>
#include <physfs.h>
#include <iostream>
#include <game/audio.h>
#include "classes/paraGui.h"

void paraGui::AddRef ()
{

}

void paraGui::ReleaseRef ()
{

}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the index of the object index - by the passed in index
int paraGui::indexByIndex (int whichObject)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScreens[currentScreen].objectIDIndex[whichObject];
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the index into the objectType array by index
int paraGui::typeByIndex (int whichObject)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScreens[currentScreen].objectType[whichObject];
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the currently selected object on the current screen
int paraGui::selectedObject ()
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScreens[currentScreen].selectedObject;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return how many elements are on the currently active screen
int paraGui::numElements ()
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScreens[currentScreen].objectIDIndex.size ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the dimensions of the screen the GUI is rendering to
void paraGui::setRenderDimensions (int width, int height)
//----------------------------------------------------------------------------------------------------------------------
{
	renderWidth  = width;
	renderHeight = height;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Check if a mouse point is inside a bounding box
bool paraGui::pointInBox (int x, int y, __BOUNDING_BOX checkBox)
//----------------------------------------------------------------------------------------------------------------------
{
	if (x < checkBox.x1)
		return false;
	if (y < checkBox.y1)
		return false;
	if (x > checkBox.x2)
		return false;
	if (y > checkBox.y2)
		return false;

	return true;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set a function to call when displaying any output
void paraGui::setOutputFunction (funcPtrIntStr outputFunction)
//----------------------------------------------------------------------------------------------------------------------
{
	paraGui::funcOutput = outputFunction;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Init the GUI system
void paraGui::init (funcPtrIntStr outputFunction, funcStrIn getStringFunc, int newRenderWidth, int newRenderHeight, std::string newFileName)
//----------------------------------------------------------------------------------------------------------------------
{
	setOutputFunction (outputFunction);
	setRenderDimensions (newRenderWidth, newRenderHeight);

	funcOutput    = outputFunction;
	funcGetString = getStringFunc;
	fileName      = newFileName;
	load ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Called after the scripting engine restarts
void paraGui::restart ()
//----------------------------------------------------------------------------------------------------------------------
{
	guiScreens.clear ();
	guiButtons.clear ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Pass in string and parameters to format and return a string
// https://stackoverflow.com/questions/19009094/c-variable-arguments-with-stdstring-only
std::string paraGui::int_getString (std::string format, ...)
//----------------------------------------------------------------------------------------------------------------------
{
	const char *const zcFormat = format.c_str ();

	// initialize use of the variable argument array
	va_list vaArgs;
	va_start(vaArgs, format);

	// reliably acquire the size from a copy of the variable argument array
	// and a functionally reliable call to mock the formatting
	va_list vaCopy;
	va_copy(vaCopy, vaArgs);
	const int iLen = std::vsnprintf (nullptr, 0, zcFormat, vaCopy);
	va_end(vaCopy);

	// return a formatted string without risking memory mismanagement  and without assuming any compiler
	// or platform specific behavior
	std::vector<char> zc (iLen + 1);
	std::vsnprintf (zc.data (), zc.size (), zcFormat, vaArgs);
	va_end(vaArgs);

	return std::string (zc.data ());
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the index of a object type from its array
int paraGui::getIndex (int objectType, std::string objectID)
//----------------------------------------------------------------------------------------------------------------------
{
	int indexCounter = 0;

	if (objectID.empty ())
		return GUI_OBJECT_NOT_FOUND;

	switch (objectType)
	{
		case GUI_OBJECT_SCREEN:
			for (const auto &itr : guiScreens)
			{
				if (itr.ID == objectID)
					return indexCounter;

				indexCounter++;
			}
			return GUI_OBJECT_NOT_FOUND;
			break;

		case GUI_OBJECT_BUTTON:
			for (const auto &itr : guiButtons)
			{
				if (itr.ID == objectID)
					return indexCounter;

				indexCounter++;
			}
			return GUI_OBJECT_NOT_FOUND;
			break;

		default:
			return GUI_OBJECT_NOT_FOUND;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a new object type
void paraGui::create (int objectType, std::string objectID)
//----------------------------------------------------------------------------------------------------------------------
{
	__SCREEN_OBJECT newScreen;
	__GUI_OBJECT    newButton;

	if (objectID.empty ())
	{
		funcOutput (-1, "Unable to create object with empty ID string.");
		return;
	}

	switch (objectType)
	{
		case GUI_OBJECT_SCREEN:
			newScreen.ID             = objectID;
			newScreen.selectedObject = 0;
			if (guiScreens.empty ())
			{
				guiScreens.push_back (newScreen);
				return;
			}
			//
			// Check it doesn't already exist
			for (auto screenItr : guiScreens)
			{
				if (screenItr.ID == objectID)
				{
					funcOutput (-1, int_getString ("Screen [ %s ] already exists.", objectID.c_str ()));
					return;
				}
			}
			//
			// Now add it
			guiScreens.push_back (newScreen);
			break;

		case GUI_OBJECT_BUTTON:
			newButton.ID             = objectID;
			newButton.canFocus       = true;
			newButton.positionCalled = false;
			if (guiButtons.empty ())
			{
				guiButtons.push_back (newButton);
				return;
			}
			//
			// Check it doesn't already exist
			for (auto buttonItr : guiButtons)
			{
				if (buttonItr.ID == objectID)
				{
					funcOutput (-1, int_getString ("Button [ %s ] already exists.", objectID.c_str ()));
					return;
				}
			}
			guiButtons.push_back (newButton);
			break;

		default:
			funcOutput (-1, int_getString ("Unable to create object. Invalid objectType [ %i ]. Must be between [ %i ] and [ %i ]", objectType, GUI_OBJECT_SCREEN, GUI_OBJECT_LAST));
			break;
	}
#ifdef DEBUG_GUI_SETUP
	funcOutput (-1, int_getString ("Created new object [ %s ]", objectID.c_str ()));
#endif
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add an object to a screen
void paraGui::addToScreen (int objectType, std::string objectID, std::string screenID)
//----------------------------------------------------------------------------------------------------------------------
{
	int screenIndex = 0;
	int objectIndex = 0;
	//
	// Find the index for the screen to have the object added to
	screenIndex = getIndex (GUI_OBJECT_SCREEN, screenID);
	if (GUI_OBJECT_NOT_FOUND == screenIndex)
	{
		funcOutput (-1, int_getString ("Unable to locate screenID [ %s ]. Has it been created ?.", screenID.c_str ()));
		return;
	}
	//
	// Find the index for this object
	objectIndex = getIndex (objectType, objectID);
	if (GUI_OBJECT_NOT_FOUND == objectIndex)
	{
		funcOutput (-1, int_getString ("Unable to locate object to add to screen [ %s ]. Has it been created ?.", objectID.c_str ()));
		return;
	}

	guiScreens[screenIndex].objectIDIndex.push_back (objectIndex);
	guiScreens[screenIndex].objectType.push_back (objectType);

#ifdef DEBUG_GUI_SETUP
	funcOutput (-1, int_getString ("Added Object [ %s ] to Screen [ %s ]", objectID.c_str (), screenID.c_str ()));
#endif
}

//----------------------------------------------------------------------------------------------------------------------
//
// Setup the position for an object
void paraGui::setPosition (int objectType, std::string objectID, int newRadius, int coordType, double newPosX, double newPosY, double newWidth, double newHeight)
//----------------------------------------------------------------------------------------------------------------------
{
	int    objectIndex = 0;
	double tempWidth   = 0;
	double tempHeight  = 0;

	if (objectID.empty ())
	{
		funcOutput (-1, int_getString ("No objectID name supplied to set position"));
		return;
	}

	objectIndex = getIndex (objectType, objectID);
	if (GUI_OBJECT_NOT_FOUND == objectIndex)
	{
		funcOutput (-1, int_getString ("Unable to set position for [ %s ] - Object not found.", objectID.c_str ()));
		return;
	}

	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			guiButtons[objectIndex].cornerRadius   = newRadius;

			if (GUI_COORD_PERCENT == coordType)
			{
				tempWidth  = renderWidth * (newWidth / 100.0);
				tempHeight = renderHeight * (newHeight / 100.0);

				guiButtons[objectIndex].boundingBox.x1 = renderWidth * (newPosX / 100.0);
				guiButtons[objectIndex].boundingBox.y1 = renderHeight * (newPosY / 100.0);

				guiButtons[objectIndex].boundingBox.x2 = guiButtons[objectIndex].boundingBox.x1 + tempWidth;
				guiButtons[objectIndex].boundingBox.y2 = guiButtons[objectIndex].boundingBox.y1 + tempHeight;
			}
			else
			{
				guiButtons[objectIndex].boundingBox.x1 = newPosX;
				guiButtons[objectIndex].boundingBox.y1 = newPosY;
				guiButtons[objectIndex].boundingBox.x2 = guiButtons[objectIndex].boundingBox.x1 + newWidth;
				guiButtons[objectIndex].boundingBox.y2 = guiButtons[objectIndex].boundingBox.y1 + newHeight;
			}
			guiButtons[objectIndex].positionCalled = true;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to set position."));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the fontname to use for this object
std::string paraGui::getFontName (int objectType, int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > (int) guiButtons.size ()))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", (int) guiButtons.size ()));
				return "";
			}

			return guiButtons[objectIndex].fontName;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get font."));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the fontClass name to render text with for this object
void paraGui::setFontName (int objectType, std::string objectID, std::string newFontName)
//----------------------------------------------------------------------------------------------------------------------
{
	int objectIndex;

	if ((objectID.empty ()) || (newFontName.empty ()))
	{
		funcOutput (-1, int_getString ("Object name or font name cannot be empty."));
		return;
	}

	objectIndex = getIndex (objectType, objectID);
	if (GUI_OBJECT_NOT_FOUND == objectIndex)
	{
		funcOutput (-1, int_getString ("Unable to set font for [ %s ] - Object not found.", objectID.c_str ()));
		return;
	}

	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			guiButtons[objectIndex].fontName = std::move (newFontName);
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to set font."));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the label for this object
void paraGui::setLabel (int objectType, std::string objectID, int gapSize, int newLabelPos, std::string newLabel)
//----------------------------------------------------------------------------------------------------------------------
{
	int objectIndex = 0;

	if ((objectID.empty ()) || (newLabel.empty ()))
	{
		funcOutput (-1, int_getString ("Object name or object label cannot be empty."));
		return;
	}

	if ((newLabelPos != GUI_LABEL_LEFT) && (newLabelPos != GUI_LABEL_CENTER) && (newLabelPos != GUI_LABEL_RIGHT))
	{
		funcOutput (-1, int_getString ("Invalid label position used."));
		newLabelPos = GUI_LABEL_CENTER;
	}

	objectIndex = getIndex (objectType, objectID);
	if (GUI_OBJECT_NOT_FOUND == objectIndex)
	{
		funcOutput (-1, int_getString ("Unable to set label for [ %s ] - Object not found.", objectID.c_str ()));
		return;
	}

	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			guiButtons[objectIndex].label    = newLabel;
			guiButtons[objectIndex].labelPos = newLabelPos;
			guiButtons[objectIndex].gapSize  = gapSize;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to set label."));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the name of the script function to be run when the object is activated
void paraGui::setAction (int objectType, std::string objectID, std::string newAction)
//----------------------------------------------------------------------------------------------------------------------
{
	int objectIndex = 0;

	if ((objectID.empty ()) || (newAction.empty ()))
	{
		funcOutput (-1, int_getString ("Object name or function string cannot be empty."));
		return;
	}

	objectIndex = getIndex (objectType, objectID);
	if (GUI_OBJECT_NOT_FOUND == objectIndex)
	{
		funcOutput (-1, int_getString ("Unable to set action for [ %s ] - Object not found.", objectID.c_str ()));
		return;
	}

	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			guiButtons[objectIndex].action = newAction;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to set action function."));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the colors for this object - by index. Private function - colors are set by type
void paraGui::setColorByIndex (int objectType, int objectIndex, int whichColor, int red, int green, int blue, int alpha)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					guiButtons[objectIndex].hasFocusColor.r = red;
					guiButtons[objectIndex].hasFocusColor.g = green;
					guiButtons[objectIndex].hasFocusColor.b = blue;
					guiButtons[objectIndex].hasFocusColor.a = alpha;
					break;

				case GUI_COL_INACTIVE:
					guiButtons[objectIndex].noFocusColor.r = red;
					guiButtons[objectIndex].noFocusColor.g = green;
					guiButtons[objectIndex].noFocusColor.b = blue;
					guiButtons[objectIndex].noFocusColor.a = alpha;
					break;

				case GUI_COL_ACTIVE_LABEL:
					guiButtons[objectIndex].labelFocusColor.r = red;
					guiButtons[objectIndex].labelFocusColor.g = green;
					guiButtons[objectIndex].labelFocusColor.b = blue;
					guiButtons[objectIndex].labelFocusColor.a = alpha;
					break;

				case GUI_COL_INACTIVE_LABEL:
					guiButtons[objectIndex].labelNoFocusColor.r = red;
					guiButtons[objectIndex].labelNoFocusColor.g = green;
					guiButtons[objectIndex].labelNoFocusColor.b = blue;
					guiButtons[objectIndex].labelNoFocusColor.a = alpha;
					break;

				default:
					funcOutput (-1, int_getString ("Unknown color type."));
					break;
			}
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to set colors."));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set a object types color
void paraGui::setColor (int objectType, std::string objectID, int whichColor, int red, int green, int blue, int alpha)
//----------------------------------------------------------------------------------------------------------------------
{
	int objectIndex;
	int numObjects;

	if ("ALL" == objectID)
	{
		switch (objectType)
		{
			case GUI_OBJECT_BUTTON:
				numObjects = guiButtons.size ();
				break;

			default:
				funcOutput (-1, int_getString ("Invalid object type. Unable to set colors."));
				break;
		}

		if (0 == numObjects)
			return;

		for (auto index = 0; index < numObjects; index++)
		{
			setColorByIndex (objectType, index, whichColor, red, green, blue, alpha);
		}
	}
	else
	{
		objectIndex = getIndex (objectType, objectID);
		if (GUI_OBJECT_NOT_FOUND == objectIndex)
		{
			funcOutput (-1, int_getString ("Unable to set color for [ %s ] - Object not found.", objectID.c_str ()));
			return;
		}
		setColorByIndex (objectType, objectIndex, whichColor, red, green, blue, alpha);
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Final step to say the object is ready to use
void paraGui::setReady (int objectType, std::string objectID, bool newState)
//----------------------------------------------------------------------------------------------------------------------
{
	int objectIndex;

	if (objectID.empty ())
	{
		funcOutput (-1, int_getString ("Object name cannot be empty. Unable to set state."));
		return;
	}

	objectIndex = getIndex (objectType, objectID);
	if (GUI_OBJECT_NOT_FOUND == objectIndex)
	{
		funcOutput (-1, int_getString ("Unable to set state for [ %s ] - Object not found.", objectID.c_str ()));
		return;
	}

	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			guiButtons[objectIndex].ready = newState;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to set state."));
			break;
	}

}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the text for the label
std::string paraGui::getLabelText (int objectType, int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > (int) guiButtons.size ()))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", (int) guiButtons.size ()));
				return "";
			}

			if (guiButtons[objectIndex].label.size () == 0)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no label set.", objectIndex, guiButtons[objectIndex].label.empty () ? "Unknown" : guiButtons[objectIndex].label.c_str ()));
				return "";
			}

			return guiButtons[objectIndex].label;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get label text."));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the gapSize for rendering the label
int paraGui::getGapSize (int objectType, int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > (int) guiButtons.size ()))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", (int) guiButtons.size ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiButtons[objectIndex].gapSize;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get gap size."));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the position of the label
int paraGui::getLabelPos (int objectType, int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > (int) guiButtons.size ()))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", (int) guiButtons.size ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			if (!guiButtons[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiButtons[objectIndex].label.empty () ? "Unknown" : guiButtons[objectIndex].label.c_str ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiButtons[objectIndex].labelPos;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get label position."));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the radius value
int paraGui::getRadius (int objectType, int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > (int) guiButtons.size ()))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", (int) guiButtons.size ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			if (!guiButtons[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiButtons[objectIndex].label.empty () ? "Unknown" : guiButtons[objectIndex].label.c_str ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiButtons[objectIndex].cornerRadius;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get radius."));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the colors for an object and the desired color type
__PARA_COLOR paraGui::getColor (int objectType, int objectIndex, int whichColor)
//----------------------------------------------------------------------------------------------------------------------
{
	__PARA_COLOR badColor;

	badColor.r = -1;
	badColor.g = -1;
	badColor.b = -1;
	badColor.a = -1;

	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return badColor;

			if ((objectIndex < 0) || (objectIndex > (int) guiButtons.size ()))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", (int) guiButtons.size ()));
				return badColor;
			}

			if (!guiButtons[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiButtons[objectIndex].label.empty () ? "Unknown" : guiButtons[objectIndex].label.c_str ()));
				return badColor;
			}

			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					return guiButtons[objectIndex].hasFocusColor;
					break;

				case GUI_COL_INACTIVE:
					return guiButtons[objectIndex].noFocusColor;
					break;

				case GUI_COL_ACTIVE_LABEL:
					return guiButtons[objectIndex].labelFocusColor;
					break;

				case GUI_COL_INACTIVE_LABEL:
					return guiButtons[objectIndex].labelNoFocusColor;
					break;

				default:
					funcOutput (-1, int_getString ("Invalid color type."));
					return badColor;
					break;
			}
		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get dimensions."));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the bounding box coordinates for the object
__BOUNDING_BOX paraGui::getBB (int objectType, int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	__BOUNDING_BOX badBox;

	badBox.x1 = -1;
	badBox.y1 = -1;
	badBox.x2 = -1;
	badBox.y2 = -1;

	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return badBox;

			if ((objectIndex < 0) || (objectIndex > (int) guiButtons.size ()))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", (int) guiButtons.size ()));
				return badBox;
			}

			if (!guiButtons[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiButtons[objectIndex].label.empty () ? "Unknown" : guiButtons[objectIndex].label.c_str ()));
				return badBox;
			}

			return guiButtons[objectIndex].boundingBox;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get dimensions."));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Find the object on the current screen and make it active
void paraGui::setActive (std::string objectID)
//----------------------------------------------------------------------------------------------------------------------
{
	unsigned long indexCount = 0;

	for (indexCount = 0; indexCount != guiScreens[currentScreen].objectIDIndex.size (); indexCount++)
	{
		switch (guiScreens[currentScreen].objectType[indexCount])
		{
			case GUI_OBJECT_BUTTON:
				if (guiButtons[guiScreens[currentScreen].objectIDIndex[indexCount]].ID == objectID)
				{
					guiScreens[currentScreen].selectedObject = (int) indexCount;
					return;
				}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Is the passed in object ready for use
bool paraGui::isReady (int objectType, int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return false;

			if ((objectIndex < 0) || (objectIndex > (int) guiButtons.size ()))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", (int) guiButtons.size ()));
				return false;
			}

			if (!guiButtons[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiButtons[objectIndex].label.empty () ? "Unknown" : guiButtons[objectIndex].label.c_str ()));
				return false;
			}

			return guiButtons[objectIndex].ready;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to set state."));
			return false;
			break;
	}
}

//-----------------------------------------------------------------------------
//
// Return if an object can be selected or not
bool paraGui::canBeSelected (int objectType)
//-----------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
//		case GUI_OBJECT_CHECKBOX:
//		case GUI_OBJECT_SLIDER:
//		case GUI_OBJECT_KEYCODE:
			return true;

		default:
			return false;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Check if the mouse is inside an elements bounding box - activate it if it is
void paraGui::checkMousePosition ()
//----------------------------------------------------------------------------------------------------------------------
{
	static int previousElement = 0;

	for (auto index = 0; index != guiScreens[currentScreen].objectType.size (); index++)
	{
		if (canBeSelected (guiScreens[currentScreen].objectType[index]))
		{
			switch (guiScreens[currentScreen].objectType[index])
			{
				case GUI_OBJECT_BUTTON:
					if (pointInBox (mouseX, mouseY, guiButtons[guiScreens[currentScreen].objectIDIndex[index]].boundingBox))
					{
						guiScreens[currentScreen].selectedObject = index;
						if (previousElement != index)
						{
							gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood.wav");
							previousElement = index;
						}
					}
					break;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Check movement actions
void paraGui::checkMovementActions ()
//----------------------------------------------------------------------------------------------------------------------
{
	int indexCount = 1;

	if (keyBinding[KEY_DOWN].currentlyPressed)
	{
		if (guiScreens[currentScreen].selectedObject != (int) guiScreens[currentScreen].objectIDIndex.size () - 1)    // Don't go past number on screen
		{
			while (!canBeSelected (guiScreens[currentScreen].objectType[guiScreens[currentScreen].selectedObject + indexCount]))
			{
				indexCount++;
			}

			guiScreens[currentScreen].selectedObject += indexCount;
			if (indexCount > (int) guiScreens[currentScreen].objectIDIndex.size ())
			{
				indexCount = (int) guiScreens[currentScreen].objectIDIndex.size ();
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad.wav");
			}
			else
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood.wav");
		}
		else
		{
			// play bad sound
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad.wav");
		}
	}

	if (keyBinding[KEY_UP].currentlyPressed)
	{
		indexCount = 1;
		if (guiScreens[currentScreen].selectedObject > 0)
		{
			while (!canBeSelected (guiScreens[currentScreen].objectType[guiScreens[currentScreen].selectedObject - indexCount]))
			{
				indexCount++;
			}

			if (guiScreens[currentScreen].selectedObject - indexCount == 0)
			{
				guiScreens[currentScreen].selectedObject = 0;
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood.wav");
				return;
			}

			if (guiScreens[currentScreen].selectedObject - indexCount < 0)
			{
				guiScreens[currentScreen].selectedObject = 0;
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad.wav");
				return;
			}
			guiScreens[currentScreen].selectedObject -= indexCount;
			// Play good sound
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood.wav");
			if (guiScreens[currentScreen].selectedObject < 0)
				guiScreens[currentScreen].selectedObject = 0;
		}
		else if (guiScreens[currentScreen].selectedObject == 0)
		{
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad.wav");
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process any actions keys
void paraGui::processAction ()
//----------------------------------------------------------------------------------------------------------------------
{
	int currentElement;

	if (keyBinding[KEY_ACTION].currentlyPressed)
	{
		currentElement = guiScreens[currentScreen].selectedObject;
		// Play good sound
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood.wav");

		switch (guiScreens[currentScreen].objectType[currentElement])
		{
			case GUI_OBJECT_BUTTON:
				if (actionSource == KEY_ACTION_MOUSE)
				{
					if (pointInBox (mouseX, mouseY, guiButtons[currentElement].boundingBox))
					{
						std::cout << "Run function : " << guiButtons[currentElement].action << std::endl;
					}
				}
				else
				{
					std::cout << "Run function : " << guiButtons[currentElement].action << std::endl;
				}
				break;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Update the mouse positions
void paraGui::setMouse (int newPosX, int newPosY)
//----------------------------------------------------------------------------------------------------------------------
{
	mouseX = newPosX;
	mouseY = newPosY;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Write the keyboard layout to disk - currently using physfs library - assuming its been started
//
// TODO: read / write to platforms native byte order
void paraGui::save ()
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_file   *fileHandle;
	PHYSFS_sint64 returnCode;

	if (!PHYSFS_isInit ())
	{
		funcOutput (-1, int_getString ("PHYSFS has not been started."));
		return;
	}

	fileHandle = PHYSFS_openWrite (fileName.c_str ());
	if (nullptr == fileHandle)
	{
		funcOutput (-1, int_getString ("Unable to open keybinding file for writing [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		return;
	}

	for (auto keyIndex = 0; keyIndex != KEY_NUMBER_ACTIONS; keyIndex++)
	{
		returnCode = PHYSFS_writeBytes (fileHandle, &keyBinding[keyIndex].keyValue, sizeof (keyBinding[keyIndex].keyValue));
		if (returnCode < (PHYSFS_sint64) sizeof (keyBinding[keyIndex].keyValue))
			funcOutput (-1, int_getString ("Unable to write keybinding file [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
	}

	if (PHYSFS_close (fileHandle) == 0)
		funcOutput (-1, int_getString ("Unable to close keybinding file [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Read in the keyboard binding file - currently using physfs library - assuming its been started
void paraGui::load ()
//----------------------------------------------------------------------------------------------------------------------
{
	PHYSFS_file   *fileHandle;
	PHYSFS_sint64 returnCode;

	if (!PHYSFS_isInit ())
	{
		funcOutput (-1, int_getString ("PHYSFS has not been started."));
		return;
	}

	fileHandle = PHYSFS_openRead (fileName.c_str ());
	if (nullptr == fileHandle)
	{
		funcOutput (-1, int_getString ("Unable to open keybinding file [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
		setDefaultKeybindings ();
		setKeyDescription ();
		return;
	}

	for (auto keyIndex = 0; keyIndex != KEY_NUMBER_ACTIONS; keyIndex++)
	{
		returnCode = PHYSFS_readBytes (fileHandle, &keyBinding[keyIndex].keyValue, sizeof (keyBinding[keyIndex].keyValue));
		if (returnCode < 0)
		{
			funcOutput (-1, int_getString ("Unable to read keybinding file [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
			return;
		}
	}

	setKeyDescription ();   // Get text for current language after loading the key values

	if (PHYSFS_close (fileHandle) == 0)
		funcOutput (-1, int_getString ("Unable to close keybinding file [ %s ] - [ %s ]", fileName.c_str (), PHYSFS_getErrorByCode (PHYSFS_getLastErrorCode ())));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Display the keybinding table
void paraGui::print ()
//----------------------------------------------------------------------------------------------------------------------
{
	funcOutput (-1, int_getString ("--- Key Binding Table ---"));

	for (auto keyCounter = 0; keyCounter != KEY_NUMBER_ACTIONS; keyCounter++)
	{
		funcOutput (-1, int_getString ("Key [ %i ] - [ %s ]", keyBinding[keyCounter].keyValue, keyBinding[keyCounter].text.c_str ()));
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the state of a key based on PARA_KEY
bool paraGui::keyDown (int whichKey)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((whichKey < KEY_FIRST) || (whichKey > KEY_NUMBER_ACTIONS))
	{
		funcOutput (-1, "Invalid key index requesting key state.");
		return false;
	}

	return keyBinding[whichKey].currentlyPressed;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the state of a key based on PARA_Key index. This will be overwritten next frame when the array is updated
void paraGui::setState (int whichKey, bool newState, int newActionSource)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((whichKey < KEY_FIRST) || (whichKey > KEY_NUMBER_ACTIONS))
	{
		funcOutput (-1, "Invalid key index requesting key state.");
		return;
	}

	keyBinding[whichKey].currentlyPressed = newState;
	if (whichKey == KEY_ACTION)
		newActionSource = newActionSource;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Assign the text description for each key
void paraGui::setKeyDescription ()
//----------------------------------------------------------------------------------------------------------------------
{
	keyBinding[KEY_LEFT].text       = funcGetString ("gameLeft");
	keyBinding[KEY_RIGHT].text      = funcGetString ("gameRight");
	keyBinding[KEY_DOWN].text       = funcGetString ("gameDown");
	keyBinding[KEY_UP].text         = funcGetString ("gameUp");
	keyBinding[KEY_PAUSE].text      = funcGetString ("gamePause");
	keyBinding[KEY_ACTION].text     = funcGetString ("gameAction");
	keyBinding[KEY_ESCAPE].text     = funcGetString ("gameEscape");
	keyBinding[KEY_CONSOLE].text    = funcGetString ("consoleAction");
	keyBinding[KEY_SCREENSHOT].text = funcGetString ("gameScreenShot");
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the default values for keybindings
void paraGui::setDefaultKeybindings ()
//----------------------------------------------------------------------------------------------------------------------
{
	keyBinding[KEY_LEFT].keyValue       = SDL_SCANCODE_LEFT;
	keyBinding[KEY_RIGHT].keyValue      = SDL_SCANCODE_RIGHT;
	keyBinding[KEY_DOWN].keyValue       = SDL_SCANCODE_DOWN;
	keyBinding[KEY_UP].keyValue         = SDL_SCANCODE_UP;
	keyBinding[KEY_PAUSE].keyValue      = SDL_SCANCODE_P;
	keyBinding[KEY_ACTION].keyValue     = SDL_SCANCODE_LCTRL;
	keyBinding[KEY_ESCAPE].keyValue     = SDL_SCANCODE_ESCAPE;
	keyBinding[KEY_CONSOLE].keyValue    = SDL_SCANCODE_GRAVE;
	keyBinding[KEY_SCREENSHOT].keyValue = SDL_SCANCODE_S;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Update the state of the keyboard mappings from the system keyboard state
void paraGui::update ()
//----------------------------------------------------------------------------------------------------------------------
{
	for (auto keyCounter = 0; keyCounter != KEY_NUMBER_ACTIONS; keyCounter++)
	{
		keyboardState[keyBinding[keyCounter].keyValue] ? keyBinding[keyCounter].currentlyPressed = true : keyBinding[keyCounter].currentlyPressed = false;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process the key events
void paraGui::process ()
//----------------------------------------------------------------------------------------------------------------------
{
	checkMousePosition ();
	checkMovementActions ();
	processAction ();
}