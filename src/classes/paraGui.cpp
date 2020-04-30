#include <cstdarg>
#include "classes/paraGui.h"
#include "../../data/scripts/enum.h"

void paraGui::AddRef ()
{

}

void paraGui::ReleaseRef ()
{

}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the index of the object index - by the passed in index
int paraGui::indexByIndex(int whichObject)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScreens[currentScreen].objectIDIndex[whichObject];
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the index into the objectType array by index
int paraGui::typeByIndex(int whichObject)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScreens[currentScreen].objectType[whichObject];
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the currently selected object on the current screen
int paraGui::selectedObject()
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScreens[currentScreen].selectedObject;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return how many elements are on the currently active screen
int paraGui::numElements()
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScreens[currentScreen].objectIDIndex.size();
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
// Set a function to call when displaying any output
void paraGui::setOutputFunction (funcPtrIntStr outputFunction)
//----------------------------------------------------------------------------------------------------------------------
{
	paraGui::funcOutput = outputFunction;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Init the GUI system
void paraGui::init (funcPtrIntStr outputFunction, int newRenderWidth, int newRenderHeight)
//----------------------------------------------------------------------------------------------------------------------
{
	setOutputFunction (outputFunction);
	setRenderDimensions (newRenderWidth, newRenderHeight);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Called after the scripting engine restarts
void paraGui::restart()
//----------------------------------------------------------------------------------------------------------------------
{
	guiScreens.clear();
	guiButtons.clear();
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
			guiButtons[objectIndex].gapSize = gapSize;
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

			if (guiButtons[objectIndex].label.size() == 0)
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