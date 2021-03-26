#include <cstdarg>
#include <physfs.h>
#include <iostream>
#include <utility>
#include <game/audio.h>
#include <io/console.h>
#include <system/util.h>
#include "classes/paraGui.h"


void paraGui::AddRef ()
{

}

void paraGui::ReleaseRef ()
{

}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the index of the current action object from the current dialogbox
int paraGui::getActiveObjectIndexDialogbox ()
//----------------------------------------------------------------------------------------------------------------------
{
	return guiDialogBoxes[currentDialogbox].objectIDIndex[guiDialogBoxes[currentDialogbox].selectedObject];
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the index of the current action object from the current screen
int paraGui::getActiveObjectIndex ()
//----------------------------------------------------------------------------------------------------------------------
{
#ifdef MY_GUI_DEBUG
	std::cout << "getActiveObjectIndex. CurrentScreen : " << currentScreen << std::endl;
#endif

	return guiScreens[currentScreen].objectIDIndex[guiScreens[currentScreen].selectedObject];
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the currently active dialogbox
int paraGui::getCurrentDialogbox ()
//----------------------------------------------------------------------------------------------------------------------
{
	return currentDialogbox;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the currently active screen
int paraGui::getCurrentScreen ()
//----------------------------------------------------------------------------------------------------------------------
{
	return currentScreen;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set a new active object for a dialogbox
void paraGui::setActiveObjectDialogbox (int whichDialogbox, int objectType, std::string objectID)
//----------------------------------------------------------------------------------------------------------------------
{
	int indexCount = 0;

	if (guiDialogBoxes.empty ())
		return;

	for (auto indexItr : guiDialogBoxes[whichDialogbox].objectIDIndex)
	{
		switch (guiDialogBoxes[whichDialogbox].objectType[indexCount])
		{
			case GUI_OBJECT_BUTTON:
				if (guiButtons[guiDialogBoxes[whichDialogbox].objectIDIndex[indexCount]].ID == objectID)
				{
					guiDialogBoxes[whichDialogbox].selectedObject = indexCount;
				}
				break;
		}
		indexCount++;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set a new active object
void paraGui::setActiveObject (int whichScreen, int objectType, std::string objectID)
//----------------------------------------------------------------------------------------------------------------------
{
	int indexCount = 0;

	if (guiScrollBoxes.empty ())
		return;

	for (auto indexItr : guiScreens[whichScreen].objectIDIndex)
	{
		switch (guiScreens[whichScreen].objectType[indexCount])
		{
			case GUI_OBJECT_BUTTON:
				if (guiButtons[guiScreens[whichScreen].objectIDIndex[indexCount]].ID == objectID)
				{
					guiScreens[whichScreen].selectedObject = indexCount;
				}
				break;

			case GUI_OBJECT_SLIDER:
				if (guiSliders[guiScreens[whichScreen].objectIDIndex[indexCount]].ID == objectID)
				{
					guiScreens[whichScreen].selectedObject = indexCount;
				}
				break;

			case GUI_OBJECT_CHECKBOX:
				if (guiCheckBoxes[guiScreens[whichScreen].objectIDIndex[indexCount]].ID == objectID)
				{
					guiScreens[whichScreen].selectedObject = indexCount;
				}
				break;
		}
		indexCount++;
	}

#ifdef MY_GUI_DEBUG
	std::cout << "setActiveObject : " << --indexCount << " whichScreen : " << whichScreen << " objectType : " << objectType << " ObjectID : " << objectID << std::endl;
#endif
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set a new dialog box
void paraGui::setCurrentDialogbox (int newDialogbox)
//----------------------------------------------------------------------------------------------------------------------
{
	currentDialogbox = newDialogbox;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Change to a new GUI screen
void paraGui::setCurrentScreen (int newScreen)
//----------------------------------------------------------------------------------------------------------------------
{
	currentScreen = newScreen;

#ifdef MY_GUI_DEBUG
	std::cout << "Setting currentScreen to : " << currentScreen << std::endl;
#endif
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the index of the object Index
int paraGui::indexByIndexDialogbox (int whichObject)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiDialogBoxes[currentDialogbox].objectIDIndex[whichObject];
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
int paraGui::typeByIndexDialogbox (int whichObject)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiDialogBoxes[currentDialogbox].objectType[whichObject];
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
// Return the currently selected object on the current dialobox
int paraGui::selectedObjectDialogbox ()
//----------------------------------------------------------------------------------------------------------------------
{
	return guiDialogBoxes[currentDialogbox].selectedObject;
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
// Return how many elements are on the currently active dialobox
int paraGui::numElementsDialogbox ()
//----------------------------------------------------------------------------------------------------------------------
{
	return guiDialogBoxes[currentDialogbox].objectIDIndex.size ();
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
// Set the dimensions of the screen the GUI is rendering to when in game mode
// Dialogboxes rendered in Gamemode are on lower resolution screen
void paraGui::setRenderDimensionsGameMode (int width, int height)
//----------------------------------------------------------------------------------------------------------------------
{
	renderWidthGame  = width;
	renderHeightGame = height;
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
	float ratioX;
	float ratioY;
	float tempX;
	float tempY;

	ratioX = static_cast<float>(gameWinWidth) / static_cast<float>(windowWidth);
	ratioY = static_cast<float>(gameWinHeight) / static_cast<float>(windowHeight);

	if (gui.getCurrentDialogbox () != NO_DIALOG_BOX)
	{
		checkBox.x1 *= ratioX;
		checkBox.y1 *= ratioY;
		checkBox.x2 *= ratioX;
		checkBox.y2 *= ratioY;

		tempX = static_cast<float>(x) * ratioX;
		tempY = static_cast<float>(y) * ratioY;

		x = static_cast<int>(tempX);
		y = static_cast<int>(tempY);
	}

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
void paraGui::init (funcPtrIntStr outputFunction, funcStrIn getStringFunc, int newRenderWidth, int newRenderHeight, int newRenderWidthGame, int newRenderHeightGame, std::string newFileName)
//----------------------------------------------------------------------------------------------------------------------
{
	setOutputFunction (outputFunction);
	setRenderDimensions (newRenderWidth, newRenderHeight);
	setRenderDimensionsGameMode (newRenderWidthGame, newRenderHeightGame);

	funcOutput    = outputFunction;
	funcGetString = getStringFunc;
	fileName      = std::move (newFileName);
	load ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Called after the scripting engine restarts
void paraGui::restart ()
//----------------------------------------------------------------------------------------------------------------------
{
	guiScreens.clear ();
	guiDialogBoxes.clear ();
	guiButtons.clear ();
	guiSliders.clear ();
	guiLabels.clear ();
	guiScrollBoxes.clear ();
	guiCheckBoxes.clear ();
	guiTextboxes.clear ();
	guiImages.clear ();
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
int paraGui::getIndex (int objectType, const std::string &objectID)
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
#ifdef MY_GUI_DEBUG
				std::cout << "getIndex : Looking for [ " << indexCounter << " ] match for " << objectID << " to [ " << itr.ID << " ]" << std::endl;
#endif
				if (itr.ID == objectID)
				{
#ifdef MY_GUI_DEBUG
					std::cout << "getIndex : Looking for [ " << indexCounter << " ] match for " << objectID << " to [ " << itr.ID << " ] - FOUND" << std::endl;
#endif
					return indexCounter;
				}
				indexCounter++;
			}
			return GUI_OBJECT_NOT_FOUND;
			break;

		case GUI_OBJECT_DIALOGBOX:
			for (const auto &itr : guiDialogBoxes)
			{
#ifdef MY_GUI_DEBUG
				std::cout << "getIndex : Looking for [ " << indexCounter << " ] match for " << objectID << " to [ " << itr.ID << " ]" << std::endl;
#endif
				if (itr.ID == objectID)
				{
#ifdef MY_GUI_DEBUG
					std::cout << "getIndex : Looking for [ " << indexCounter << " ] match for " << objectID << " to [ " << itr.ID << " ] - FOUND" << std::endl;
#endif
					return indexCounter;
				}

				indexCounter++;
			}
			return GUI_OBJECT_NOT_FOUND;
			break;

		case GUI_OBJECT_BUTTON:

			for (const auto &itr : guiButtons)
			{
#ifdef MY_GUI_DEBUG
				std::cout << "getIndex : Looking for [ " << indexCounter << " ] match for " << objectID << " to [ " << itr.ID << " ]" << std::endl;
#endif
				if (itr.ID == objectID)
				{
#ifdef MY_GUI_DEBUG
					std::cout << "getIndex : Looking for [ " << indexCounter << " ] match for " << objectID << " to [ " << itr.ID << " ] - FOUND" << std::endl;
#endif
					return indexCounter;
				}
				indexCounter++;
			}
			return GUI_OBJECT_NOT_FOUND;
			break;

		case GUI_OBJECT_SLIDER:
			for (const auto &itr : guiSliders)
			{
				if (itr.ID == objectID)
				{
					return indexCounter;
				}
				indexCounter++;
			}
			return GUI_OBJECT_NOT_FOUND;
			break;

		case GUI_OBJECT_LABEL:
			for (const auto &itr : guiLabels)
			{
				if (itr.ID == objectID)
					return indexCounter;

				indexCounter++;
			}
			return GUI_OBJECT_NOT_FOUND;
			break;

		case GUI_OBJECT_SCROLLBOX:
			for (const auto &itr : guiScrollBoxes)
			{
				if (itr.ID == objectID)
					return indexCounter;

				indexCounter++;
			}
			break;

		case GUI_OBJECT_CHECKBOX:
			for (const auto &itr : guiCheckBoxes)
			{
				if (itr.ID == objectID)
					return indexCounter;

				indexCounter++;
			}
			break;

		case GUI_OBJECT_TEXTBOX:
			for (const auto &itr : guiTextboxes)
			{
				if (itr.ID == objectID)
					return indexCounter;

				indexCounter++;
			}
			break;

		case GUI_OBJECT_IMAGE:
			for (const auto &itr : guiImages)
			{
				if (itr.ID == objectID)
					return indexCounter;

				indexCounter++;
			}

		default:
			return GUI_OBJECT_NOT_FOUND;
	}

	return GUI_OBJECT_NOT_FOUND;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Create a new object type
void paraGui::create (int objectType, std::string objectID)
//----------------------------------------------------------------------------------------------------------------------
{
	__SCREEN_OBJECT newScreen;
	__GUI_DIALOGBOX newDialogbox;
	__GUI_OBJECT    newButton;
	__GUI_SLIDER    newSlider;
	__GUI_OBJECT    newLabel;
	__GUI_SCROLLBOX newScrollbox;
	__GUI_CHECKBOX  newCheckBox;
	__GUI_OBJECT    newTextbox;
	__GUI_OBJECT    newImage;

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
			for (const auto& screenItr : guiScreens)
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

		case GUI_OBJECT_DIALOGBOX:
			newDialogbox.ID             = objectID;
			newDialogbox.selectedObject = 0;
			if (guiDialogBoxes.empty ())
			{
				guiDialogBoxes.push_back (newDialogbox);
				return;
			}
			//
			// Check it doesn't already exist
			for (const auto& dialogboxItr : guiDialogBoxes)
			{
				if (dialogboxItr.ID == objectID)
				{
					funcOutput (-1, int_getString ("Dialogbox [ %s ] already exists.", objectID.c_str ()));
					return;
				}
			}
			//
			// Now add it
			guiDialogBoxes.push_back (newDialogbox);
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
			for (const auto& buttonItr : guiButtons)
			{
				if (buttonItr.ID == objectID)
				{
					funcOutput (-1, int_getString ("Button [ %s ] already exists.", objectID.c_str ()));
					return;
				}
			}
			guiButtons.push_back (newButton);
			break;

		case GUI_OBJECT_SLIDER:
			newSlider.ID             = objectID;
			newSlider.canFocus       = true;
			newSlider.positionCalled = false;
			newSlider.currentStep    = 0;
			newSlider.numberSteps    = 0;

			if (guiSliders.empty ())
			{
				guiSliders.push_back (newSlider);
				return;
			}
			//
			// Check it doesnt already exists
			for (const auto& sliderItr : guiSliders)
			{
				if (sliderItr.ID == objectID)
				{
					funcOutput (-1, int_getString ("Slider [ %s ] already exists.", objectID.c_str ()));
					return;
				}
			}
			guiSliders.push_back (newSlider);
			break;

		case GUI_OBJECT_LABEL:
			newLabel.ID             = objectID;
			newLabel.canFocus       = false;
			newLabel.positionCalled = false;

			if (guiLabels.empty ())
			{
				guiLabels.push_back (newLabel);
				return;
			}
			for (const auto& labelItr : guiLabels)
			{
				if (labelItr.ID == objectID)
				{
					funcOutput (-1, int_getString ("Label [ %i ] already exists.", objectID.c_str ()));
					return;
				}
			}
			guiLabels.push_back (newLabel);
			break;

		case GUI_OBJECT_SCROLLBOX:
			newScrollbox.ID             = objectID;
			newScrollbox.canFocus       = false;
			newScrollbox.positionCalled = false;

			if (guiScrollBoxes.empty ())
			{
				guiScrollBoxes.push_back (newScrollbox);
				break;
			}
			for (const auto& scrollboxItr : guiScrollBoxes)
			{
				if (scrollboxItr.ID == objectID)
				{
					funcOutput (-1, int_getString ("Scrollbox [ %s ] already exists.", objectID.c_str ()));
					return;
				}
			}
			guiScrollBoxes.push_back (newScrollbox);
			break;

		case GUI_OBJECT_CHECKBOX:
			newCheckBox.ID             = objectID;
			newCheckBox.canFocus       = true;
			newCheckBox.positionCalled = false;

			if (guiCheckBoxes.empty ())
			{
				guiCheckBoxes.push_back (newCheckBox);
				break;
			}
			for (const auto& checkBoxItr : guiCheckBoxes)
			{
				if (checkBoxItr.ID == objectID)
				{
					funcOutput (-1, int_getString ("Checkbox [ %s ] already exists.", objectID.c_str ()));
					return;
				}
			}
			guiCheckBoxes.push_back (newCheckBox);
			break;

		case GUI_OBJECT_TEXTBOX:
			newTextbox.ID             = objectID;
			newTextbox.canFocus       = false;
			newTextbox.positionCalled = false;

			if (guiTextboxes.empty ())
			{
				guiTextboxes.push_back (newTextbox);
				break;
			}
			for (const auto& textboxItr : guiTextboxes)
			{
				if (textboxItr.ID == objectID)
				{
					funcOutput (-1, int_getString ("Textbox [ %s ] already exists.", objectID.c_str ()));
					return;
				}
			}
			guiTextboxes.push_back (newTextbox);
			break;

		case GUI_OBJECT_IMAGE:
			newImage.ID             = objectID;
			newImage.canFocus       = false;
			newImage.positionCalled = false;

			if (guiImages.empty ())
			{
				guiImages.push_back (newImage);
				break;
			}
			for (const auto& imageItr : guiImages)
			{
				if (imageItr.ID == objectID)
				{
					funcOutput (-1, int_getString ("Image [ %s ] already exists.", objectID.c_str ()));
					return;
				}
			}
			guiImages.push_back (newImage);
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
// Get the fade value of the bottom line
double paraGui::getLineFade (int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScrollBoxes[objectIndex].lineFade;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the fade value of the bottom line
void paraGui::setLineFade (int objectIndex, double newLineFade)
//----------------------------------------------------------------------------------------------------------------------
{
	guiScrollBoxes[objectIndex].lineFade = newLineFade;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the iterator pointing to the reverse entry
std::vector<std::string>::reverse_iterator paraGui::getREnd (int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScrollBoxes[objectIndex].scrollBoxText.rend ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the iterator pointing to the last entry
std::vector<std::string>::reverse_iterator paraGui::getRBegin (int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScrollBoxes[objectIndex].scrollBoxText.rbegin ();
}

//----------------------------------------------------------------------------------------------------------------------
//
// Reset a scrollbox to start
void paraGui::restartScrollBox (const string &objectID)
//----------------------------------------------------------------------------------------------------------------------
{
	int objectIndex;

	//
	// Find the index for this object
	objectIndex = getIndex (GUI_OBJECT_SCROLLBOX, objectID);
	if (GUI_OBJECT_NOT_FOUND == objectIndex)
	{
		funcOutput (-1, int_getString ("Unable to locate object to set scroll speed [ %s ]. Has it been created ?.", objectID.c_str ()));
		return;
	}
	guiScrollBoxes[objectIndex].scrollBoxText.clear ();
	guiScrollBoxes[objectIndex].scrollY         = 0.0;
	guiScrollBoxes[objectIndex].previousScrollY = -1.0;
	guiScrollBoxes[objectIndex].currentChar     = 0;
	guiScrollBoxes[objectIndex].numLinesToPrint = 0;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the next line of text to show in the scrollbox
void paraGui::getNextLineOfText (int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	std::string fontName;
	std::string nextLine;
	std::string nextWord;
	std::string nextChar;
	bool        foundLine     = false;
	int         previousSpace = 0;
	int         lineTextWidth = 0;

	foundLine = false;
	nextLine.clear ();
	nextWord.clear ();
	nextChar.clear ();
	previousSpace = 0;

	fontName = gui.getFontName (GUI_OBJECT_SCROLLBOX, objectIndex);
	if (fontName.size () == 0)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get fontname for scrollbox [ %i ]", objectIndex));
		return;
	}
	fontClass.use (fontName);

	while (!foundLine)
	{
		nextChar = guiScrollBoxes[objectIndex].label[guiScrollBoxes[objectIndex].currentChar];
		switch (nextChar.back ())
		{
			case ' ':
				lineTextWidth = fontClass.width (nextLine + " " + nextChar + nextWord);
				if (lineTextWidth < (guiScrollBoxes[objectIndex].boundingBox.x2 - guiScrollBoxes[objectIndex].boundingBox.x1) - guiScrollBoxes[objectIndex].gapSize)
				{
					previousSpace = guiScrollBoxes[objectIndex].currentChar;
					nextLine.append (" ");
					nextLine.append (nextWord);
					nextWord.clear ();
				}
				else
				{
					guiScrollBoxes[objectIndex].currentChar = previousSpace;
					foundLine = true;
				}
				break;

			case '!':
				nextLine += nextWord;
				nextWord.clear ();
				foundLine = true;
				guiScrollBoxes[objectIndex].currentChar++;
				break;

			case '#':
				nextLine += nextWord;
				nextWord.clear ();
				foundLine = true;
				guiScrollBoxes[objectIndex].currentChar = 0;
				break;

			case '^':
				nextLine += nextWord;
				nextWord.clear ();
				foundLine = true;
				guiScrollBoxes[objectIndex].currentChar++;
				guiScrollBoxes[objectIndex].scrollDelay = 40;
				break;

			default:
				nextWord += nextChar;
				break;
		}
		guiScrollBoxes[objectIndex].currentChar++;
		if (guiScrollBoxes[objectIndex].currentChar > static_cast<int>(guiScrollBoxes[objectIndex].label.size ()))
		{
			nextLine += nextWord;
			foundLine = true;
			guiScrollBoxes[objectIndex].currentChar = 0;
		}
	}
	guiScrollBoxes[objectIndex].scrollBoxText.push_back (nextLine);
	guiScrollBoxes[objectIndex].lineFade = 0.0;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the current number of lines to print in a scrollbox
int paraGui::getNumberPrintLines (int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScrollBoxes[objectIndex].numLinesToPrint;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the number of lines to print in a scrollbox
void paraGui::setNumberPrintLines (int objectIndex, int newNumberLines)
//----------------------------------------------------------------------------------------------------------------------
{
	guiScrollBoxes[objectIndex].numLinesToPrint = newNumberLines;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the current scrollY counter
double paraGui::getScrollY (int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScrollBoxes[objectIndex].scrollY;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the current scrollY counter
void paraGui::setScrollY (int objectIndex, double newScrollY)
//----------------------------------------------------------------------------------------------------------------------
{
	guiScrollBoxes[objectIndex].scrollY = newScrollY;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the current scrollY counter
double paraGui::getPreviousScrollY (int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScrollBoxes[objectIndex].previousScrollY;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the current scrollY counter
void paraGui::setPreviousScrollY (int objectIndex, double newScrollY)
//----------------------------------------------------------------------------------------------------------------------
{
	guiScrollBoxes[objectIndex].previousScrollY = newScrollY;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the current scrollbox delay counter
double paraGui::getScrollDelay (int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiScrollBoxes[objectIndex].scrollDelay;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the new scrolbox delay counter
void paraGui::setScrollDelay (int objectIndex, double newScrollDelay)
//----------------------------------------------------------------------------------------------------------------------
{
	guiScrollBoxes[objectIndex].scrollDelay = newScrollDelay;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the scrolling speed for a scrollbox
void paraGui::setScrollSpeed (int objectType, const std::string &objectID, double newScrollSpeed)
//----------------------------------------------------------------------------------------------------------------------
{
	int objectIndex = 0;

	//
	// Find the index for this object
	objectIndex = getIndex (GUI_OBJECT_SCROLLBOX, objectID);
	if (GUI_OBJECT_NOT_FOUND == objectIndex)
	{
		funcOutput (-1, int_getString ("Unable to locate object to set scroll speed [ %s ]. Has it been created ?.", objectID.c_str ()));
		return;
	}

	guiScrollBoxes[objectIndex].scrollSpeed = newScrollSpeed;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add an object to a dialogbox
void paraGui::addToDialogbox (int objectType, std::string objectID, std::string dialogboxID)
//----------------------------------------------------------------------------------------------------------------------
{
	int dialogboxIndex = 0;
	int objectIndex    = 0;
	//
	// Find the index for the dialogbox to have the object added to
	dialogboxIndex = getIndex (GUI_OBJECT_DIALOGBOX, dialogboxID);
	if (GUI_OBJECT_NOT_FOUND == dialogboxIndex)
	{
		funcOutput (-1, int_getString ("Unable to locate dialogbox [ %s ]. Had it been created ?.", dialogboxID.c_str ()));
		return;
	}

	objectIndex = getIndex (objectType, objectID);
	if (GUI_OBJECT_NOT_FOUND == objectIndex)
	{
		funcOutput (-1, int_getString ("Unable to locate object to add to dialogbox [ %s ]. Has it been created ?.", objectID.c_str ()));
		return;
	}

	guiDialogBoxes[dialogboxIndex].objectIDIndex.push_back (objectIndex);
	guiDialogBoxes[dialogboxIndex].objectType.push_back (objectType);

#ifdef DEBUG_GUI_SETUP
	funcOutput (-1, int_getString ("Added Object [ %s ] to Dialogbox [ %s ]", objectID.c_str (), dialogboxID.c_str ()));
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
		case GUI_OBJECT_DIALOGBOX:
			guiDialogBoxes[objectIndex].cornerRadius   = newRadius;

			if (GUI_COORD_PERCENT == coordType)
			{
				tempWidth  = renderWidth * (newWidth / 100.0);
				tempHeight = renderHeight * (newHeight / 100.0);

				guiDialogBoxes[objectIndex].boundingBox.x1 = renderWidthGame * (newPosX / 100.0);
				guiDialogBoxes[objectIndex].boundingBox.y1 = renderHeightGame * (newPosY / 100.0);

				guiDialogBoxes[objectIndex].boundingBox.x2 = guiDialogBoxes[objectIndex].boundingBox.x1 + tempWidth;
				guiDialogBoxes[objectIndex].boundingBox.y2 = guiDialogBoxes[objectIndex].boundingBox.y1 + tempHeight;
			}
			else
			{
				guiDialogBoxes[objectIndex].boundingBox.x1 = newPosX;
				guiDialogBoxes[objectIndex].boundingBox.y1 = newPosY;
				guiDialogBoxes[objectIndex].boundingBox.x2 = guiDialogBoxes[objectIndex].boundingBox.x1 + newWidth;
				guiDialogBoxes[objectIndex].boundingBox.y2 = guiDialogBoxes[objectIndex].boundingBox.y1 + newHeight;
			}
			guiDialogBoxes[objectIndex].positionCalled = true;
			break;

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

		case GUI_OBJECT_SLIDER:
			guiSliders[objectIndex].cornerRadius   = newRadius;

			if (GUI_COORD_PERCENT == coordType)
			{
				tempWidth  = renderWidth * (newWidth / 100.0);
				tempHeight = renderHeight * (newHeight / 100.0);

				guiSliders[objectIndex].boundingBox.x1 = renderWidth * (newPosX / 100.0);
				guiSliders[objectIndex].boundingBox.y1 = renderHeight * (newPosY / 100.0);

				guiSliders[objectIndex].boundingBox.x2 = guiSliders[objectIndex].boundingBox.x1 + tempWidth;
				guiSliders[objectIndex].boundingBox.y2 = guiSliders[objectIndex].boundingBox.y1 + tempHeight;
			}
			else
			{
				guiSliders[objectIndex].boundingBox.x1 = newPosX;
				guiSliders[objectIndex].boundingBox.y1 = newPosY;
				guiSliders[objectIndex].boundingBox.x2 = guiSliders[objectIndex].boundingBox.x1 + newWidth;
				guiSliders[objectIndex].boundingBox.y2 = guiSliders[objectIndex].boundingBox.y1 + newHeight;
			}
			guiSliders[objectIndex].positionCalled = true;
			break;

		case GUI_OBJECT_LABEL:
			guiLabels[objectIndex].cornerRadius   = -1;

			if (GUI_COORD_PERCENT == coordType)
			{
				tempWidth  = renderWidth * (newWidth / 100.0);
				tempHeight = renderHeight * (newHeight / 100.0);

				guiLabels[objectIndex].boundingBox.x1 = renderWidth * (newPosX / 100.0);
				guiLabels[objectIndex].boundingBox.y1 = renderHeight * (newPosY / 100.0);

				guiLabels[objectIndex].boundingBox.x2 = guiLabels[objectIndex].boundingBox.x1 + tempWidth;
				guiLabels[objectIndex].boundingBox.y2 = guiLabels[objectIndex].boundingBox.y1 + tempHeight;
			}
			else
			{
				guiLabels[objectIndex].boundingBox.x1 = newPosX;
				guiLabels[objectIndex].boundingBox.y1 = newPosY;
				guiLabels[objectIndex].boundingBox.x2 = guiLabels[objectIndex].boundingBox.x1 + newWidth;
				guiLabels[objectIndex].boundingBox.y2 = guiLabels[objectIndex].boundingBox.y1 + newHeight;
			}
			guiLabels[objectIndex].positionCalled = true;
			break;

		case GUI_OBJECT_SCROLLBOX:
			guiScrollBoxes[objectIndex].cornerRadius   = newRadius;

			if (GUI_COORD_PERCENT == coordType)
			{
				tempWidth  = renderWidth * (newWidth / 100.0);
				tempHeight = renderHeight * (newHeight / 100.0);

				guiScrollBoxes[objectIndex].boundingBox.x1 = renderWidth * (newPosX / 100.0);
				guiScrollBoxes[objectIndex].boundingBox.y1 = renderHeight * (newPosY / 100.0);

				guiScrollBoxes[objectIndex].boundingBox.x2 = guiScrollBoxes[objectIndex].boundingBox.x1 + tempWidth;
				guiScrollBoxes[objectIndex].boundingBox.y2 = guiScrollBoxes[objectIndex].boundingBox.y1 + tempHeight;
			}
			else
			{
				guiScrollBoxes[objectIndex].boundingBox.x1 = newPosX;
				guiScrollBoxes[objectIndex].boundingBox.y1 = newPosY;
				guiScrollBoxes[objectIndex].boundingBox.x2 = guiScrollBoxes[objectIndex].boundingBox.x1 + newWidth;
				guiScrollBoxes[objectIndex].boundingBox.y2 = guiScrollBoxes[objectIndex].boundingBox.y1 + newHeight;
			}
			guiScrollBoxes[objectIndex].positionCalled = true;
			break;

		case GUI_OBJECT_CHECKBOX:
			guiCheckBoxes[objectIndex].cornerRadius   = newRadius;

			if (GUI_COORD_PERCENT == coordType)
			{
				tempWidth  = renderWidth * (newWidth / 100.0);
				tempHeight = renderHeight * (newHeight / 100.0);

				guiCheckBoxes[objectIndex].boundingBox.x1 = renderWidth * (newPosX / 100.0);
				guiCheckBoxes[objectIndex].boundingBox.y1 = renderHeight * (newPosY / 100.0);

				guiCheckBoxes[objectIndex].boundingBox.x2 = guiCheckBoxes[objectIndex].boundingBox.x1 + tempWidth;
				guiCheckBoxes[objectIndex].boundingBox.y2 = guiCheckBoxes[objectIndex].boundingBox.y1 + tempHeight;
			}
			else
			{
				guiCheckBoxes[objectIndex].boundingBox.x1 = newPosX;
				guiCheckBoxes[objectIndex].boundingBox.y1 = newPosY;
				guiCheckBoxes[objectIndex].boundingBox.x2 = guiCheckBoxes[objectIndex].boundingBox.x1 + newWidth;
				guiCheckBoxes[objectIndex].boundingBox.y2 = guiCheckBoxes[objectIndex].boundingBox.y1 + newHeight;
			}
			guiCheckBoxes[objectIndex].positionCalled = true;
			break;

		case GUI_OBJECT_TEXTBOX:
			guiTextboxes[objectIndex].cornerRadius   = -1;

			if (GUI_COORD_PERCENT == coordType)
			{
				tempWidth  = renderWidth * (newWidth / 100.0);
				tempHeight = renderHeight * (newHeight / 100.0);

				guiTextboxes[objectIndex].boundingBox.x1 = renderWidth * (newPosX / 100.0);
				guiTextboxes[objectIndex].boundingBox.y1 = renderHeight * (newPosY / 100.0);

				guiTextboxes[objectIndex].boundingBox.x2 = guiTextboxes[objectIndex].boundingBox.x1 + tempWidth;
				guiTextboxes[objectIndex].boundingBox.y2 = guiTextboxes[objectIndex].boundingBox.y1 + tempHeight;
			}
			else
			{
				guiTextboxes[objectIndex].boundingBox.x1 = newPosX;
				guiTextboxes[objectIndex].boundingBox.y1 = newPosY;
				guiTextboxes[objectIndex].boundingBox.x2 = guiTextboxes[objectIndex].boundingBox.x1 + newWidth;
				guiTextboxes[objectIndex].boundingBox.y2 = guiTextboxes[objectIndex].boundingBox.y1 + newHeight;
			}
			guiTextboxes[objectIndex].positionCalled = true;
			break;

		case GUI_OBJECT_IMAGE:
			guiImages[objectIndex].cornerRadius   = -1;

			if (GUI_COORD_PERCENT == coordType)
			{
				tempWidth  = renderWidth * (newWidth / 100.0);
				tempHeight = renderHeight * (newHeight / 100.0);

				guiImages[objectIndex].boundingBox.x1 = (renderWidth * (newPosX / 100.0)) - (newWidth / 2);
				guiImages[objectIndex].boundingBox.y1 = (renderHeight * (newPosY / 100.0)) - (newHeight / 2);

				guiImages[objectIndex].boundingBox.x2 = guiImages[objectIndex].boundingBox.x1 + tempWidth;
				guiImages[objectIndex].boundingBox.y2 = guiImages[objectIndex].boundingBox.y1 + tempHeight;
			}
			else
			{
				guiImages[objectIndex].boundingBox.x1 = newPosX;
				guiImages[objectIndex].boundingBox.y1 = newPosY;
				guiImages[objectIndex].boundingBox.x2 = guiImages[objectIndex].boundingBox.x1 + newWidth;
				guiImages[objectIndex].boundingBox.y2 = guiImages[objectIndex].boundingBox.y1 + newHeight;
			}
			guiImages[objectIndex].positionCalled = true;
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
		case GUI_OBJECT_DIALOGBOX:
			if (guiDialogBoxes.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiDialogBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiDialogBoxes.size ())));
				return "";
			}

			return guiDialogBoxes[objectIndex].fontName;
			break;

		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiButtons.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiButtons.size ())));
				return "";
			}

			return guiButtons[objectIndex].fontName;
			break;

		case GUI_OBJECT_SLIDER:
			if (guiSliders.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiSliders.size ())))
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiSliders.size ())));
				return "";
			}
			return guiSliders[objectIndex].fontName;
			break;

		case GUI_OBJECT_LABEL:
			if (guiLabels.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiLabels.size ())))
			{
				funcOutput (-1, int_getString ("Label index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiLabels.size ())));
				return "";
			}
			return guiLabels[objectIndex].fontName;
			break;

		case GUI_OBJECT_SCROLLBOX:
			if (guiScrollBoxes.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiScrollBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Label index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiScrollBoxes.size ())));
				return "";
			}
			return guiScrollBoxes[objectIndex].fontName;
			break;

		case GUI_OBJECT_CHECKBOX:
			if (guiCheckBoxes.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiCheckBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Label index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiCheckBoxes.size ())));
				return "";
			}
			return guiCheckBoxes[objectIndex].fontName;
			break;

		case GUI_OBJECT_TEXTBOX:
			if (guiTextboxes.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiTextboxes.size ())))
			{
				funcOutput (-1, int_getString ("Label index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiTextboxes.size ())));
				return "";
			}
			return guiTextboxes[objectIndex].fontName;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get font."));
			return "Unknown object for fontName.";
			break;
	}

	return "Unknown object for fontName.";
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
		case GUI_OBJECT_DIALOGBOX:
			guiDialogBoxes[objectIndex].fontName = std::move (newFontName);
			break;

		case GUI_OBJECT_BUTTON:
			guiButtons[objectIndex].fontName = std::move (newFontName);
			break;

		case GUI_OBJECT_SLIDER:
			guiSliders[objectIndex].fontName = std::move (newFontName);
			break;

		case GUI_OBJECT_LABEL:
			guiLabels[objectIndex].fontName = std::move (newFontName);
			break;

		case GUI_OBJECT_SCROLLBOX:
			guiScrollBoxes[objectIndex].fontName = std::move (newFontName);
			break;

		case GUI_OBJECT_CHECKBOX:
			guiCheckBoxes[objectIndex].fontName = std::move (newFontName);
			break;

		case GUI_OBJECT_TEXTBOX:
			guiTextboxes[objectIndex].fontName = std::move (newFontName);
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to set font."));
			break;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the label for this object
void paraGui::setLabel (int objectType, std::string objectID, int newGapSize, int newLabelPos, std::string newLabel)
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
		case GUI_OBJECT_DIALOGBOX:
			guiDialogBoxes[objectIndex].label   = newLabel;
			guiDialogBoxes[objectIndex].gapSize = newGapSize;
			break;

		case GUI_OBJECT_BUTTON:
			guiButtons[objectIndex].label    = newLabel;
			guiButtons[objectIndex].labelPos = newLabelPos;
			guiButtons[objectIndex].gapSize  = newGapSize;
			break;

		case GUI_OBJECT_SLIDER:
			guiSliders[objectIndex].label    = newLabel;
			guiSliders[objectIndex].labelPos = newLabelPos;
			guiSliders[objectIndex].gapSize  = newGapSize;
			break;

		case GUI_OBJECT_LABEL:
			guiLabels[objectIndex].label    = newLabel;
			guiLabels[objectIndex].labelPos = newLabelPos;
			guiLabels[objectIndex].gapSize  = newGapSize;
			break;

		case GUI_OBJECT_SCROLLBOX:
			guiScrollBoxes[objectIndex].label    = newLabel;
			guiScrollBoxes[objectIndex].labelPos = newLabelPos;
			guiScrollBoxes[objectIndex].gapSize  = newGapSize;
			break;

		case GUI_OBJECT_CHECKBOX:
			guiCheckBoxes[objectIndex].label    = newLabel;
			guiCheckBoxes[objectIndex].labelPos = newLabelPos;
			guiCheckBoxes[objectIndex].gapSize  = newGapSize;
			break;

		case GUI_OBJECT_TEXTBOX:
			guiTextboxes[objectIndex].label    = newLabel;
			guiTextboxes[objectIndex].labelPos = newLabelPos;
			guiTextboxes[objectIndex].gapSize  = newGapSize;
			break;

		case GUI_OBJECT_IMAGE:
			guiImages[objectIndex].label    = newLabel;
			guiImages[objectIndex].labelPos = newLabelPos;
			guiImages[objectIndex].gapSize  = newGapSize;
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

		case GUI_OBJECT_SLIDER:
			guiSliders[objectIndex].action = newAction;
			break;

		case GUI_OBJECT_CHECKBOX:
			guiCheckBoxes[objectIndex].action = newAction;
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
		case GUI_OBJECT_DIALOGBOX:
			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					guiDialogBoxes[objectIndex].hasFocusColor.r = red;
					guiDialogBoxes[objectIndex].hasFocusColor.g = green;
					guiDialogBoxes[objectIndex].hasFocusColor.b = blue;
					guiDialogBoxes[objectIndex].hasFocusColor.a = alpha;
					break;

				case GUI_COL_ACTIVE_LABEL:
					guiDialogBoxes[objectIndex].labelFocusColor.r = red;
					guiDialogBoxes[objectIndex].labelFocusColor.g = green;
					guiDialogBoxes[objectIndex].labelFocusColor.b = blue;
					guiDialogBoxes[objectIndex].labelFocusColor.a = alpha;
					break;
			}
			break;

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

		case GUI_OBJECT_SLIDER:
			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					guiSliders[objectIndex].hasFocusColor.r = red;
					guiSliders[objectIndex].hasFocusColor.g = green;
					guiSliders[objectIndex].hasFocusColor.b = blue;
					guiSliders[objectIndex].hasFocusColor.a = alpha;
					break;

				case GUI_COL_INACTIVE:
					guiSliders[objectIndex].noFocusColor.r = red;
					guiSliders[objectIndex].noFocusColor.g = green;
					guiSliders[objectIndex].noFocusColor.b = blue;
					guiSliders[objectIndex].noFocusColor.a = alpha;
					break;

				case GUI_COL_ACTIVE_LABEL:
					guiSliders[objectIndex].labelFocusColor.r = red;
					guiSliders[objectIndex].labelFocusColor.g = green;
					guiSliders[objectIndex].labelFocusColor.b = blue;
					guiSliders[objectIndex].labelFocusColor.a = alpha;
					break;

				case GUI_COL_INACTIVE_LABEL:
					guiSliders[objectIndex].labelNoFocusColor.r = red;
					guiSliders[objectIndex].labelNoFocusColor.g = green;
					guiSliders[objectIndex].labelNoFocusColor.b = blue;
					guiSliders[objectIndex].labelNoFocusColor.a = alpha;
					break;

				default:
					funcOutput (-1, int_getString ("Unknown color type."));
					break;
			}
			break;

		case GUI_OBJECT_LABEL:
			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					guiLabels[objectIndex].hasFocusColor.r = red;
					guiLabels[objectIndex].hasFocusColor.g = green;
					guiLabels[objectIndex].hasFocusColor.b = blue;
					guiLabels[objectIndex].hasFocusColor.a = alpha;
					break;

				default:
					funcOutput (-1, int_getString ("Unknown color type."));
					break;
			}
			break;

		case GUI_OBJECT_SCROLLBOX:
			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					guiScrollBoxes[objectIndex].hasFocusColor.r = red;
					guiScrollBoxes[objectIndex].hasFocusColor.g = green;
					guiScrollBoxes[objectIndex].hasFocusColor.b = blue;
					guiScrollBoxes[objectIndex].hasFocusColor.a = alpha;
					break;

				case GUI_COL_ACTIVE_LABEL:
					guiScrollBoxes[objectIndex].labelFocusColor.r = red;
					guiScrollBoxes[objectIndex].labelFocusColor.g = green;
					guiScrollBoxes[objectIndex].labelFocusColor.b = blue;
					guiScrollBoxes[objectIndex].labelFocusColor.a = alpha;
					break;

				default:
					funcOutput (-1, int_getString ("Unknown color type."));
					break;
			}
			break;

		case GUI_OBJECT_CHECKBOX:
			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					guiCheckBoxes[objectIndex].hasFocusColor.r = red;
					guiCheckBoxes[objectIndex].hasFocusColor.g = green;
					guiCheckBoxes[objectIndex].hasFocusColor.b = blue;
					guiCheckBoxes[objectIndex].hasFocusColor.a = alpha;
					break;

				case GUI_COL_INACTIVE:
					guiCheckBoxes[objectIndex].noFocusColor.r = red;
					guiCheckBoxes[objectIndex].noFocusColor.g = green;
					guiCheckBoxes[objectIndex].noFocusColor.b = blue;
					guiCheckBoxes[objectIndex].noFocusColor.a = alpha;
					break;

				case GUI_COL_ACTIVE_LABEL:
					guiCheckBoxes[objectIndex].labelFocusColor.r = red;
					guiCheckBoxes[objectIndex].labelFocusColor.g = green;
					guiCheckBoxes[objectIndex].labelFocusColor.b = blue;
					guiCheckBoxes[objectIndex].labelFocusColor.a = alpha;
					break;

				case GUI_COL_INACTIVE_LABEL:
					guiCheckBoxes[objectIndex].labelNoFocusColor.r = red;
					guiCheckBoxes[objectIndex].labelNoFocusColor.g = green;
					guiCheckBoxes[objectIndex].labelNoFocusColor.b = blue;
					guiCheckBoxes[objectIndex].labelNoFocusColor.a = alpha;
					break;

				default:
					funcOutput (-1, int_getString ("Unknown color type."));
					break;
			}
			break;

		case GUI_OBJECT_TEXTBOX:
			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					guiTextboxes[objectIndex].hasFocusColor.r = red;
					guiTextboxes[objectIndex].hasFocusColor.g = green;
					guiTextboxes[objectIndex].hasFocusColor.b = blue;
					guiTextboxes[objectIndex].hasFocusColor.a = alpha;
					break;

				default:
					funcOutput (-1, int_getString ("Unknown color type."));
					break;
			}
			break;

		case GUI_OBJECT_IMAGE:
			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					guiImages[objectIndex].hasFocusColor.r = red;
					guiImages[objectIndex].hasFocusColor.g = green;
					guiImages[objectIndex].hasFocusColor.b = blue;
					guiImages[objectIndex].hasFocusColor.a = alpha;
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
	int numObjects = 0;

	if ("ALL" == objectID)
	{
		switch (objectType)
		{
			case GUI_OBJECT_DIALOGBOX:
				numObjects = guiDialogBoxes.size ();
				break;

			case GUI_OBJECT_BUTTON:
				numObjects = guiButtons.size ();
				break;

			case GUI_OBJECT_SLIDER:
				numObjects = guiSliders.size ();
				break;

			case GUI_OBJECT_LABEL:
				numObjects = guiLabels.size ();
				break;

			case GUI_OBJECT_SCROLLBOX:
				numObjects = guiScrollBoxes.size ();
				break;

			case GUI_OBJECT_CHECKBOX:
				numObjects = guiCheckBoxes.size ();
				break;

			case GUI_OBJECT_TEXTBOX:
				numObjects = guiTextboxes.size ();
				break;

			case GUI_OBJECT_IMAGE:
				numObjects = guiImages.size ();
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
		case GUI_OBJECT_DIALOGBOX:
			guiDialogBoxes[objectIndex].ready = newState;
			break;

		case GUI_OBJECT_BUTTON:
			guiButtons[objectIndex].ready = newState;
			break;

		case GUI_OBJECT_SLIDER:
			guiSliders[objectIndex].ready = newState;
			break;

		case GUI_OBJECT_LABEL:
			guiLabels[objectIndex].ready = newState;
			break;

		case GUI_OBJECT_SCROLLBOX:
			guiScrollBoxes[objectIndex].ready = newState;
			break;

		case GUI_OBJECT_CHECKBOX:
			guiCheckBoxes[objectIndex].ready = newState;
			break;

		case GUI_OBJECT_TEXTBOX:
			guiTextboxes[objectIndex].ready = newState;
			break;

		case GUI_OBJECT_IMAGE:
			guiImages[objectIndex].ready = newState;
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
		case GUI_OBJECT_DIALOGBOX:
			if (guiDialogBoxes.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiDialogBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Dialogbox index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiDialogBoxes.size ())));
				return "";
			}

			if (guiDialogBoxes[objectIndex].label.size () == 0)
			{
				funcOutput (-1, int_getString ("Dialogbox index [ %i ] - [ %s ] has no label set.", objectIndex, guiDialogBoxes[objectIndex].label.empty () ? "Unknown" : guiDialogBoxes[objectIndex].label.c_str ()));
				return "";
			}

			return guiDialogBoxes[objectIndex].label;
			break;

		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiButtons.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiButtons.size ())));
				return "";
			}

			if (guiButtons[objectIndex].label.size () == 0)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no label set.", objectIndex, guiButtons[objectIndex].label.empty () ? "Unknown" : guiButtons[objectIndex].label.c_str ()));
				return "";
			}

			return guiButtons[objectIndex].label;
			break;

		case GUI_OBJECT_SLIDER:
			if (guiSliders.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiSliders.size ())))
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiSliders.size ())));
				return "";
			}

			if (guiSliders[objectIndex].label.size () == 0)
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] - [ %s ] has no label set.", objectIndex, guiSliders[objectIndex].label.empty () ? "Unknown" : guiSliders[objectIndex].label.c_str ()));
				return "";
			}

			return guiSliders[objectIndex].label;
			break;

		case GUI_OBJECT_LABEL:
			if (guiLabels.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiLabels.size ())))
			{
				funcOutput (-1, int_getString ("Label index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiLabels.size ())));
				return "";
			}

			if (guiLabels[objectIndex].label.size () == 0)
			{
				funcOutput (-1, int_getString ("Label index [ %i ] - [ %s ] has no label set.", objectIndex, guiLabels[objectIndex].label.empty () ? "Unknown" : guiLabels[objectIndex].label.c_str ()));
				return "";
			}

			return guiLabels[objectIndex].label;
			break;

		case GUI_OBJECT_CHECKBOX:
			if (guiCheckBoxes.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiCheckBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Checkbox index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiCheckBoxes.size ())));
				return "";
			}

			if (guiCheckBoxes[objectIndex].label.size () == 0)
			{
				funcOutput (-1, int_getString ("Checkbox index [ %i ] - [ %s ] has no label set.", objectIndex, guiCheckBoxes[objectIndex].label.empty () ? "Unknown" : guiCheckBoxes[objectIndex].label.c_str ()));
				return "";
			}

			return guiCheckBoxes[objectIndex].label;
			break;

		case GUI_OBJECT_TEXTBOX:
			if (guiTextboxes.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiTextboxes.size ())))
			{
				funcOutput (-1, int_getString ("Textbox index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiTextboxes.size ())));
				return "";
			}

			if (guiTextboxes[objectIndex].label.size () == 0)
			{
				funcOutput (-1, int_getString ("Textbox index [ %i ] - [ %s ] has no label set.", objectIndex, guiTextboxes[objectIndex].label.empty () ? "Unknown" : guiTextboxes[objectIndex].label.c_str ()));
				return "";
			}

			return guiTextboxes[objectIndex].label;
			break;

		case GUI_OBJECT_IMAGE:
			if (guiImages.size () == 0)
				return "";

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiImages.size ())))
			{
				funcOutput (-1, int_getString ("Image index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiImages.size ())));
				return "";
			}

			if (guiImages[objectIndex].label.size () == 0)
			{
				funcOutput (-1, int_getString ("Image index [ %i ] - [ %s ] has no label set.", objectIndex, guiLabels[objectIndex].label.empty () ? "Unknown" : guiImages[objectIndex].label.c_str ()));
				return "";
			}

			return guiImages[objectIndex].label;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get label text."));
			return "Unknown object for fontName.";
			break;
	}
	return "Unknown object for fontName.";
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the gapSize for rendering the label
int paraGui::getGapSize (int objectType, int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_DIALOGBOX:
			if (guiDialogBoxes.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiDialogBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiDialogBoxes.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiDialogBoxes[objectIndex].gapSize;
			break;

		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiButtons.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiButtons.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiButtons[objectIndex].gapSize;
			break;

		case GUI_OBJECT_SLIDER:
			if (guiSliders.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiSliders.size ())))
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiSliders.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiSliders[objectIndex].gapSize;
			break;

		case GUI_OBJECT_LABEL:
			if (guiLabels.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiLabels.size ())))
			{
				funcOutput (-1, int_getString ("Label index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiLabels.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiLabels[objectIndex].gapSize;
			break;

		case GUI_OBJECT_CHECKBOX:
			if (guiCheckBoxes.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiCheckBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Checkbox index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiCheckBoxes.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiCheckBoxes[objectIndex].gapSize;
			break;

		case GUI_OBJECT_TEXTBOX:
			if (guiTextboxes.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiTextboxes.size ())))
			{
				funcOutput (-1, int_getString ("Textbox index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiTextboxes.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiTextboxes[objectIndex].gapSize;
			break;

		case GUI_OBJECT_IMAGE:
			if (guiImages.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiImages.size ())))
			{
				funcOutput (-1, int_getString ("Image index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiImages.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiImages[objectIndex].gapSize;
			break;


		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get gap size."));
			return -1;
			break;
	}

	return -1;
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

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiButtons.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiButtons.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			if (!guiButtons[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiButtons[objectIndex].label.empty () ? "Unknown" : guiButtons[objectIndex].label.c_str ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiButtons[objectIndex].labelPos;
			break;

		case GUI_OBJECT_SLIDER:
			if (guiSliders.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiSliders.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiSliders.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			if (!guiSliders[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiSliders[objectIndex].label.empty () ? "Unknown" : guiSliders[objectIndex].label.c_str ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiSliders[objectIndex].labelPos;
			break;

		case GUI_OBJECT_LABEL:
			if (guiLabels.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiLabels.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiLabels.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			if (!guiLabels[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiLabels[objectIndex].label.empty () ? "Unknown" : guiLabels[objectIndex].label.c_str ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiLabels[objectIndex].labelPos;
			break;

		case GUI_OBJECT_CHECKBOX:
			if (guiCheckBoxes.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiCheckBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiCheckBoxes.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			if (!guiCheckBoxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiCheckBoxes[objectIndex].label.empty () ? "Unknown" : guiLabels[objectIndex].label.c_str ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiCheckBoxes[objectIndex].labelPos;
			break;

		case GUI_OBJECT_TEXTBOX:
			if (guiTextboxes.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiTextboxes.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiTextboxes.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			if (!guiTextboxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiTextboxes[objectIndex].label.empty () ? "Unknown" : guiLabels[objectIndex].label.c_str ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiTextboxes[objectIndex].labelPos;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get label position."));
			return -1;
			break;
	}
	return -1;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the radius value
int paraGui::getRadius (int objectType, int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_DIALOGBOX:
			if (guiDialogBoxes.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiDialogBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiDialogBoxes.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			if (!guiDialogBoxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiDialogBoxes[objectIndex].label.empty () ? "Unknown" : guiDialogBoxes[objectIndex].label.c_str ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiDialogBoxes[objectIndex].cornerRadius;
			break;

		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiButtons.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiButtons.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			if (!guiButtons[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiButtons[objectIndex].label.empty () ? "Unknown" : guiButtons[objectIndex].label.c_str ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiButtons[objectIndex].cornerRadius;
			break;

		case GUI_OBJECT_SLIDER:
			if (guiSliders.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiSliders.size ())))
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiSliders.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			if (!guiSliders[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiSliders[objectIndex].label.empty () ? "Unknown" : guiSliders[objectIndex].label.c_str ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiSliders[objectIndex].cornerRadius;
			break;

		case GUI_OBJECT_SCROLLBOX:
			if (guiScrollBoxes.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiScrollBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiScrollBoxes.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			if (!guiSliders[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiScrollBoxes[objectIndex].label.empty () ? "Unknown" : guiScrollBoxes[objectIndex].label.c_str ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiScrollBoxes[objectIndex].cornerRadius;
			break;

		case GUI_OBJECT_CHECKBOX:
			if (guiCheckBoxes.size () == 0)
				return GUI_OBJECT_NOT_FOUND;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiCheckBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiCheckBoxes.size ())));
				return GUI_OBJECT_NOT_FOUND;
			}

			if (!guiCheckBoxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiCheckBoxes[objectIndex].label.empty () ? "Unknown" : guiCheckBoxes[objectIndex].label.c_str ()));
				return GUI_OBJECT_NOT_FOUND;
			}

			return guiCheckBoxes[objectIndex].cornerRadius;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get radius."));
			return GUI_OBJECT_NOT_FOUND;
			break;
	}

	return GUI_OBJECT_NOT_FOUND;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the colors for an object and the desired color type
__PARA_COLOR paraGui::getColor (int objectType, int objectIndex, int whichColor)
//----------------------------------------------------------------------------------------------------------------------
{
	__PARA_COLOR badColor;

	badColor.r = PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND;
	badColor.g = PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND;
	badColor.b = PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND;
	badColor.a = PARA_GUI_CODES::GUI_OBJECT_NOT_FOUND;

	switch (objectType)
	{
		case GUI_OBJECT_DIALOGBOX:
			if (guiDialogBoxes.size () == 0)
				return badColor;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiDialogBoxes.size ())))
			{
				funcOutput (-1, int_getString ("guiDialogBoxes index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiDialogBoxes.size ())));
				return badColor;
			}

			if (!guiDialogBoxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("guiDialogBoxes index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiDialogBoxes[objectIndex].label.empty () ? "Unknown" : guiDialogBoxes[objectIndex].label.c_str ()));
				return badColor;
			}

			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					return guiDialogBoxes[objectIndex].hasFocusColor;
					break;

				case GUI_COL_ACTIVE_LABEL:
					return guiDialogBoxes[objectIndex].labelFocusColor;
					break;

				default:
					funcOutput (-1, int_getString ("Invalid color type."));
					return badColor;
					break;
			}
			break;

		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return badColor;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiButtons.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiButtons.size ())));
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
			break;

		case GUI_OBJECT_SLIDER:
			if (guiSliders.size () == 0)
				return badColor;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiSliders.size ())))
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiSliders.size ())));
				return badColor;
			}

			if (!guiSliders[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiSliders[objectIndex].label.empty () ? "Unknown" : guiSliders[objectIndex].label.c_str ()));
				return badColor;
			}

			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					return guiSliders[objectIndex].hasFocusColor;
					break;

				case GUI_COL_INACTIVE:
					return guiSliders[objectIndex].noFocusColor;
					break;

				case GUI_COL_ACTIVE_LABEL:
					return guiSliders[objectIndex].labelFocusColor;
					break;

				case GUI_COL_INACTIVE_LABEL:
					return guiSliders[objectIndex].labelNoFocusColor;
					break;

				default:
					funcOutput (-1, int_getString ("Invalid color type."));
					return badColor;
					break;
			}

		case GUI_OBJECT_LABEL:
			if (guiLabels.size () == 0)
				return badColor;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiLabels.size ())))
			{
				funcOutput (-1, int_getString ("Label index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiLabels.size ())));
				return badColor;
			}

			if (!guiLabels[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Label index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiLabels[objectIndex].label.empty () ? "Unknown" : guiLabels[objectIndex].label.c_str ()));
				return badColor;
			}

			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					return guiLabels[objectIndex].hasFocusColor;
					break;

				default:
					funcOutput (-1, int_getString ("Invalid color type."));
					return badColor;
					break;
			}
			break;

		case GUI_OBJECT_SCROLLBOX:
			if (guiScrollBoxes.size () == 0)
				return badColor;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiScrollBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Scrollbox index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiScrollBoxes.size ())));
				return badColor;
			}

			if (!guiScrollBoxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Scrollbox index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiScrollBoxes[objectIndex].label.empty () ? "Unknown" : guiScrollBoxes[objectIndex].label.c_str ()));
				return badColor;
			}

			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					return guiScrollBoxes[objectIndex].hasFocusColor;
					break;

				case GUI_COL_ACTIVE_LABEL:
					return guiScrollBoxes[objectIndex].labelFocusColor;
					break;

				default:
					funcOutput (-1, int_getString ("Invalid color type."));
					return badColor;
					break;
			}
			break;

		case GUI_OBJECT_CHECKBOX:
			if (guiCheckBoxes.size () == 0)
				return badColor;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiCheckBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Checkbox index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiCheckBoxes.size ())));
				return badColor;
			}

			if (!guiCheckBoxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Checkbox index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiCheckBoxes[objectIndex].label.empty () ? "Unknown" : guiCheckBoxes[objectIndex].label.c_str ()));
				return badColor;
			}

			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					return guiCheckBoxes[objectIndex].hasFocusColor;
					break;

				case GUI_COL_INACTIVE:
					return guiCheckBoxes[objectIndex].noFocusColor;
					break;

				case GUI_COL_ACTIVE_LABEL:
					return guiCheckBoxes[objectIndex].labelFocusColor;
					break;

				case GUI_COL_INACTIVE_LABEL:
					return guiCheckBoxes[objectIndex].labelNoFocusColor;
					break;

				default:
					funcOutput (-1, int_getString ("Invalid color type."));
					return badColor;
					break;
			}
			break;

		case GUI_OBJECT_TEXTBOX:
			if (guiTextboxes.size () == 0)
				return badColor;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiTextboxes.size ())))
			{
				funcOutput (-1, int_getString ("Textbox index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiTextboxes.size ())));
				return badColor;
			}

			if (!guiTextboxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Textbox index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiTextboxes[objectIndex].label.empty () ? "Unknown" : guiTextboxes[objectIndex].label.c_str ()));
				return badColor;
			}

			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					return guiTextboxes[objectIndex].hasFocusColor;
					break;

				default:
					funcOutput (-1, int_getString ("Invalid color type."));
					return badColor;
					break;
			}
			break;

		case GUI_OBJECT_IMAGE:
			if (guiImages.size () == 0)
				return badColor;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiImages.size ())))
			{
				funcOutput (-1, int_getString ("Image index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiImages.size ())));
				return badColor;
			}

			if (!guiImages[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Image index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiImages[objectIndex].label.empty () ? "Unknown" : guiImages[objectIndex].label.c_str ()));
				return badColor;
			}

			switch (whichColor)
			{
				case GUI_COL_ACTIVE:
					return guiImages[objectIndex].hasFocusColor;
					break;

				default:
					funcOutput (-1, int_getString ("Invalid color type."));
					return badColor;
					break;
			}
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get color."));
			return badColor;
			break;
	}
	return badColor;
}


//----------------------------------------------------------------------------------------------------------------------
//
// Return the bounding box coordinates for the object
__BOUNDING_BOX paraGui::getBB (int objectType, int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	__BOUNDING_BOX badBox{};

	badBox.x1 = GUI_OBJECT_NOT_FOUND;
	badBox.y1 = GUI_OBJECT_NOT_FOUND;
	badBox.x2 = GUI_OBJECT_NOT_FOUND;
	badBox.y2 = GUI_OBJECT_NOT_FOUND;

	switch (objectType)
	{
		case GUI_OBJECT_DIALOGBOX:
			if (guiDialogBoxes.size () == 0)
				return badBox;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiDialogBoxes.size ())))
			{
				funcOutput (-1, int_getString ("guiDialogBoxes index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiDialogBoxes.size ())));
				return badBox;
			}

			if (!guiDialogBoxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("guiDialogBoxes index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiDialogBoxes[objectIndex].label.empty () ? "Unknown" : guiDialogBoxes[objectIndex].label.c_str ()));
				return badBox;
			}

			return guiDialogBoxes[objectIndex].boundingBox;
			break;

		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return badBox;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiButtons.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiButtons.size ())));
				return badBox;
			}

			if (!guiButtons[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiButtons[objectIndex].label.empty () ? "Unknown" : guiButtons[objectIndex].label.c_str ()));
				return badBox;
			}

			return guiButtons[objectIndex].boundingBox;
			break;

		case GUI_OBJECT_SLIDER:
			if (guiSliders.size () == 0)
				return badBox;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiSliders.size ())))
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiSliders.size ())));
				return badBox;
			}

			if (!guiSliders[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiSliders[objectIndex].label.empty () ? "Unknown" : guiSliders[objectIndex].label.c_str ()));
				return badBox;
			}

			return guiSliders[objectIndex].boundingBox;
			break;

		case GUI_OBJECT_LABEL:
			if (guiLabels.size () == 0)
				return badBox;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiLabels.size ())))
			{
				funcOutput (-1, int_getString ("Label index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiLabels.size ())));
				return badBox;
			}

			if (!guiLabels[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Label index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiLabels[objectIndex].label.empty () ? "Unknown" : guiLabels[objectIndex].label.c_str ()));
				return badBox;
			}

			return guiLabels[objectIndex].boundingBox;
			break;

		case GUI_OBJECT_SCROLLBOX:
			if (guiScrollBoxes.size () == 0)
				return badBox;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiScrollBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Scrollbox index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiScrollBoxes.size ())));
				return badBox;
			}

			if (!guiScrollBoxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Scrollbox index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiScrollBoxes[objectIndex].label.empty () ? "Unknown" : guiScrollBoxes[objectIndex].label.c_str ()));
				return badBox;
			}

			return guiScrollBoxes[objectIndex].boundingBox;
			break;

		case GUI_OBJECT_CHECKBOX:
			if (guiCheckBoxes.size () == 0)
				return badBox;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiCheckBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Checkbox index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiCheckBoxes.size ())));
				return badBox;
			}

			if (!guiCheckBoxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Checkbox index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiCheckBoxes[objectIndex].label.empty () ? "Unknown" : guiCheckBoxes[objectIndex].label.c_str ()));
				return badBox;
			}

			return guiCheckBoxes[objectIndex].boundingBox;
			break;

		case GUI_OBJECT_TEXTBOX:
			if (guiTextboxes.size () == 0)
				return badBox;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiTextboxes.size ())))
			{
				funcOutput (-1, int_getString ("Textbox index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiTextboxes.size ())));
				return badBox;
			}

			if (!guiTextboxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Textbox index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiTextboxes[objectIndex].label.empty () ? "Unknown" : guiLabels[objectIndex].label.c_str ()));
				return badBox;
			}

			return guiTextboxes[objectIndex].boundingBox;
			break;

		case GUI_OBJECT_IMAGE:
			if (guiImages.size () == 0)
				return badBox;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiImages.size ())))
			{
				funcOutput (-1, int_getString ("Image index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiImages.size ())));
				return badBox;
			}

			if (!guiImages[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Image index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiLabels[objectIndex].label.empty () ? "Unknown" : guiImages[objectIndex].label.c_str ()));
				return badBox;
			}

			return guiImages[objectIndex].boundingBox;
			break;

		default:
			funcOutput (-1, int_getString ("Invalid object type. Unable to get dimensions."));
			break;
	}
	return badBox;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Find the object on the current dialogbox and make it active
void paraGui::setActiveDialogbox (std::string objectID)
//----------------------------------------------------------------------------------------------------------------------
{
	unsigned long indexCount = 0;

	for (indexCount = 0; indexCount != guiDialogBoxes[currentDialogbox].objectIDIndex.size (); indexCount++)
	{
		switch (guiDialogBoxes[currentDialogbox].objectType[indexCount])
		{
			case GUI_OBJECT_BUTTON:
				if (guiButtons[guiDialogBoxes[currentDialogbox].objectIDIndex[indexCount]].ID == objectID)
				{
					guiDialogBoxes[currentDialogbox].selectedObject = (int) indexCount;
					return;
				}
				break;
		}
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
				break;

			case GUI_OBJECT_SLIDER:
				if (guiSliders[guiScreens[currentScreen].objectIDIndex[indexCount]].ID == objectID)
				{
					guiScreens[currentScreen].selectedObject = (int) indexCount;
					return;
				}
				break;

			case GUI_OBJECT_CHECKBOX:
				if (guiCheckBoxes[guiScreens[currentScreen].objectIDIndex[indexCount]].ID == objectID)
				{
					guiScreens[currentScreen].selectedObject = (int) indexCount;
					return;
				}
				break;
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
		case GUI_OBJECT_DIALOGBOX:
			if (guiDialogBoxes.size () == 0)
				return false;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiDialogBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiDialogBoxes.size ())));
				return false;
			}

			if (!guiDialogBoxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiDialogBoxes[objectIndex].label.empty () ? "Unknown" : guiDialogBoxes[objectIndex].label.c_str ()));
				return false;
			}

			return guiDialogBoxes[objectIndex].ready;
			break;

		case GUI_OBJECT_BUTTON:
			if (guiButtons.size () == 0)
				return false;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiButtons.size ())))
			{
				funcOutput (-1, int_getString ("Button index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiButtons.size ())));
				return false;
			}

			if (!guiButtons[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Button index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiButtons[objectIndex].label.empty () ? "Unknown" : guiButtons[objectIndex].label.c_str ()));
				return false;
			}

			return guiButtons[objectIndex].ready;
			break;

		case GUI_OBJECT_SLIDER:
			if (guiSliders.size () == 0)
				return false;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiSliders.size ())))
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiSliders.size ())));
				return false;
			}

			if (!guiSliders[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Slider index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiSliders[objectIndex].label.empty () ? "Unknown" : guiSliders[objectIndex].label.c_str ()));
				return false;
			}

			return guiSliders[objectIndex].ready;
			break;

		case GUI_OBJECT_LABEL:
			if (guiLabels.size () == 0)
				return false;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiLabels.size ())))
			{
				funcOutput (-1, int_getString ("Label index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiLabels.size ())));
				return false;
			}

			if (!guiLabels[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Label index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiLabels[objectIndex].label.empty () ? "Unknown" : guiLabels[objectIndex].label.c_str ()));
				return false;
			}

			return guiLabels[objectIndex].ready;
			break;

		case GUI_OBJECT_CHECKBOX:
			if (guiCheckBoxes.size () == 0)
				return false;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiCheckBoxes.size ())))
			{
				funcOutput (-1, int_getString ("Checkbox index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiCheckBoxes.size ())));
				return false;
			}

			if (!guiCheckBoxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Checkbox index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiCheckBoxes[objectIndex].label.empty () ? "Unknown" : guiCheckBoxes[objectIndex].label.c_str ()));
				return false;
			}

			return guiCheckBoxes[objectIndex].ready;
			break;

		case GUI_OBJECT_TEXTBOX:
			if (guiTextboxes.size () == 0)
				return false;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiTextboxes.size ())))
			{
				funcOutput (-1, int_getString ("Textbox index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiTextboxes.size ())));
				return false;
			}

			if (!guiTextboxes[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Textbox index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiTextboxes[objectIndex].label.empty () ? "Unknown" : guiTextboxes[objectIndex].label.c_str ()));
				return false;
			}

			return guiTextboxes[objectIndex].ready;
			break;

		case GUI_OBJECT_IMAGE:
			if (guiImages.size () == 0)
				return false;

			if ((objectIndex < 0) || (objectIndex > static_cast<int>(guiImages.size ())))
			{
				funcOutput (-1, int_getString ("Image index [ %i ] is out of range. Between [ 0 ] and [ %i ].", static_cast<int>(guiImages.size ())));
				return false;
			}

			if (!guiImages[objectIndex].positionCalled)
			{
				funcOutput (-1, int_getString ("Image index [ %i ] - [ %s ] has no coordinates set.", objectIndex, guiImages[objectIndex].label.empty () ? "Unknown" : guiImages[objectIndex].label.c_str ()));
				return false;
			}

			return guiImages[objectIndex].ready;
			break;

		default:
			funcOutput (-1, int_getString ("Unknown object type. Unable to get ready state."));
			return false;
			break;
	}
}

//-----------------------------------------------------------------------------
//
// Return if an object can be selected or not
bool paraGui::canBeSelected (int objectType, int whichObject)
//-----------------------------------------------------------------------------
{
	switch (objectType)
	{
		case GUI_OBJECT_BUTTON:
		case GUI_OBJECT_CHECKBOX:
		case GUI_OBJECT_SLIDER:
//		case GUI_OBJECT_KEYCODE:
			return true;

		default:
			return false;
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Check if the mouse is inside an elements bounding box on a dialogbox - active it if so
void paraGui::processMousePositionDialogbox ()
//----------------------------------------------------------------------------------------------------------------------
{
	static int previousElement = 0;

	if (guiDialogBoxes.size () == 0)
		return;

	for (auto index = 0; index < static_cast<int>(guiDialogBoxes[currentDialogbox].objectType.size ()); index++)
	{
		if (canBeSelected (guiDialogBoxes[currentDialogbox].objectType[index], 0))
		{
			switch (guiDialogBoxes[currentDialogbox].objectType[index])
			{
				case GUI_OBJECT_BUTTON:
					if (pointInBox (mouseX, mouseY, guiButtons[guiDialogBoxes[currentDialogbox].objectIDIndex[index]].boundingBox))
					{
						guiDialogBoxes[currentDialogbox].selectedObject = index;
						if (previousElement != index)
						{
							gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
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
// Check if the mouse is inside an elements bounding box - activate it if it is
void paraGui::processMousePosition ()
//----------------------------------------------------------------------------------------------------------------------
{
	static int previousElement = 0;

	if (guiScreens.size () == 0)     // size() == 0 is faster than .empty()
		return;

	for (auto index = 0; index < static_cast<int>(guiScreens[currentScreen].objectType.size ()); index++)
	{
		if (canBeSelected (guiScreens[currentScreen].objectType[index], 0))
		{
			switch (guiScreens[currentScreen].objectType[index])
			{
				case GUI_OBJECT_BUTTON:
					if (pointInBox (mouseX, mouseY, guiButtons[guiScreens[currentScreen].objectIDIndex[index]].boundingBox))
					{
						guiScreens[currentScreen].selectedObject = index;
						if (previousElement != index)
						{
							gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
							previousElement = index;
						}
					}
					break;

				case GUI_OBJECT_SLIDER:
					if (pointInBox (mouseX, mouseY, guiSliders[guiScreens[currentScreen].objectIDIndex[index]].boundingBox))
					{
						guiScreens[currentScreen].selectedObject = index;
						if (previousElement != index)
						{
							gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
							previousElement = index;
						}
					}
					break;

				case GUI_OBJECT_CHECKBOX:
					if (pointInBox (mouseX, mouseY, guiCheckBoxes[guiScreens[currentScreen].objectIDIndex[index]].boundingBox))
					{
						guiScreens[currentScreen].selectedObject = index;
						if (previousElement != index)
						{
							gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
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
void paraGui::processMovementKeysDialogbox ()
//----------------------------------------------------------------------------------------------------------------------
{
	int indexCount     = 1;
	int selectedSlider = guiDialogBoxes[currentDialogbox].objectIDIndex[guiDialogBoxes[currentDialogbox].selectedObject];

	if (keyBinding[KEY_RIGHT].active)
	{
		keyBinding[KEY_DOWN].active = false;
		if (guiDialogBoxes[currentDialogbox].selectedObject != static_cast<int>(guiDialogBoxes[currentDialogbox].objectIDIndex.size ()) - 1)    // Don't go past number on dialogbox
		{
			while (!canBeSelected (guiDialogBoxes[currentDialogbox].objectType[guiDialogBoxes[currentDialogbox].selectedObject + indexCount], guiDialogBoxes[currentDialogbox].selectedObject + indexCount))
			{
				indexCount++;
			}

			guiDialogBoxes[currentDialogbox].selectedObject += indexCount;
			if (indexCount > static_cast<int>(guiDialogBoxes[currentDialogbox].objectIDIndex.size ()))
			{
				indexCount = static_cast<int>(guiDialogBoxes[currentDialogbox].objectIDIndex.size ());
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
			}
			else
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
		}
		else
		{
			// play bad sound
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
		}
	}

	if (keyBinding[KEY_LEFT].active)
	{
		keyBinding[KEY_UP].active = false;
		indexCount = 1;
		if (guiDialogBoxes[currentDialogbox].selectedObject > 0)
		{
			while (!canBeSelected (guiDialogBoxes[currentDialogbox].objectType[guiDialogBoxes[currentDialogbox].selectedObject - indexCount], 0))
			{
				indexCount++;
				if ((guiDialogBoxes[currentDialogbox].selectedObject - indexCount) < 0)        // Stop out of bounds
				{
					gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
					return;
				}
			}

			if (guiDialogBoxes[currentDialogbox].selectedObject - indexCount == 0)
			{
				guiDialogBoxes[currentDialogbox].selectedObject = 0;
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
				return;
			}

			if (guiDialogBoxes[currentDialogbox].selectedObject - indexCount < 0)
			{
				guiDialogBoxes[currentDialogbox].selectedObject = 0;
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
				return;
			}

			guiDialogBoxes[currentDialogbox].selectedObject -= indexCount;
			// Play good sound
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
			if (guiDialogBoxes[currentDialogbox].selectedObject < 0)
				guiDialogBoxes[currentDialogbox].selectedObject = 0;
		}
		else if (guiDialogBoxes[currentDialogbox].selectedObject == 0)
		{
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Check movement actions
void paraGui::processMovementKeys ()
//----------------------------------------------------------------------------------------------------------------------
{
	int indexCount     = 1;
	int selectedSlider = guiScreens[currentScreen].objectIDIndex[guiScreens[currentScreen].selectedObject];

	//
	// Use scroll wheel over a focused slider to change value
	if ((guiScreens[currentScreen].objectType[guiScreens[currentScreen].selectedObject] == GUI_OBJECT_SLIDER) && (pointInBox (mouseX, mouseY, guiSliders[selectedSlider].boundingBox)))
	{
		if (keyBinding[KEY_DOWN].active)
		{
			keyBinding[KEY_DOWN].active = false;
			keyBinding[KEY_LEFT].active = true;
		}
		if (keyBinding[KEY_UP].active)
		{
			keyBinding[KEY_UP].active    = false;
			keyBinding[KEY_RIGHT].active = true;
		}
	}

	if (keyBinding[KEY_DOWN].active)
	{
		keyBinding[KEY_DOWN].active = false;
		if (guiScreens[currentScreen].selectedObject != static_cast<int>(guiScreens[currentScreen].objectIDIndex.size ()) - 1)    // Don't go past number on screen
		{
			while (!canBeSelected (guiScreens[currentScreen].objectType[guiScreens[currentScreen].selectedObject + indexCount], guiScreens[currentScreen].selectedObject + indexCount))
			{
				indexCount++;
			}

			guiScreens[currentScreen].selectedObject += indexCount;
			if (indexCount > static_cast<int>(guiScreens[currentScreen].objectIDIndex.size ()))
			{
				indexCount = static_cast<int>(guiScreens[currentScreen].objectIDIndex.size ());
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
			}
			else
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
		}
		else
		{
			// play bad sound
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
		}
	}

	if (keyBinding[KEY_UP].active)
	{
		keyBinding[KEY_UP].active = false;
		indexCount = 1;
		if (guiScreens[currentScreen].selectedObject > 0)
		{
			while (!canBeSelected (guiScreens[currentScreen].objectType[guiScreens[currentScreen].selectedObject - indexCount], 0))
			{
				indexCount++;
				if ((guiScreens[currentScreen].selectedObject - indexCount) < 0)        // Stop out of bounds
				{
					gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
					return;
				}
			}

			if (guiScreens[currentScreen].selectedObject - indexCount == 0)
			{
				guiScreens[currentScreen].selectedObject = 0;
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
				return;
			}

			if (guiScreens[currentScreen].selectedObject - indexCount < 0)
			{
				guiScreens[currentScreen].selectedObject = 0;
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
				return;
			}

			guiScreens[currentScreen].selectedObject -= indexCount;
			// Play good sound
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
			if (guiScreens[currentScreen].selectedObject < 0)
				guiScreens[currentScreen].selectedObject = 0;
		}
		else if (guiScreens[currentScreen].selectedObject == 0)
		{
			gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
		}
	}

	if (keyBinding[KEY_LEFT].active)
	{
		keyBinding[KEY_LEFT].active = false;
		switch (guiScreens[currentScreen].objectType[guiScreens[currentScreen].selectedObject])
		{
			case GUI_OBJECT_SLIDER:
				if (guiSliders[selectedSlider].currentStep == 0)
				{
					gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
					return;
				}
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
				guiSliders[selectedSlider].currentStep -= 1;
				if (guiSliders[selectedSlider].currentStep < 0)
					guiSliders[selectedSlider].currentStep = 0;

//				paraScriptInstance.run (guiSliders[guiScreens[currentScreen].objectIDIndex[guiScreens[currentScreen].selectedObject]].action, "");

				break;
		}
		return;
	}

	if (keyBinding[KEY_RIGHT].active)
	{
		keyBinding[KEY_RIGHT].active = false;
		switch (guiScreens[currentScreen].objectType[guiScreens[currentScreen].selectedObject])
		{
			case GUI_OBJECT_SLIDER:
				if (guiSliders[selectedSlider].currentStep == static_cast<int>(guiSliders[selectedSlider].element.size () - 1 ))
				{
					gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressBad");
					return;
				}
				gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");
				guiSliders[selectedSlider].currentStep += 1;
				if (guiSliders[selectedSlider].currentStep > static_cast<int>(guiSliders[selectedSlider].element.size () - 1))
					guiSliders[selectedSlider].currentStep = static_cast<int>(guiSliders[selectedSlider].element.size () - 1);

//				paraScriptInstance.run (guiSliders[guiScreens[currentScreen].objectIDIndex[guiScreens[currentScreen].selectedObject]].action, "");
				break;
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process action key for current dialog
void paraGui::processActionDialogbox ()
//----------------------------------------------------------------------------------------------------------------------
{
	int currentElement;

	if (keyBinding[KEY_ACTION].active)
	{
		keyBinding[KEY_ACTION].active = false;

		currentElement = guiDialogBoxes[currentDialogbox].objectIDIndex[guiDialogBoxes[currentDialogbox].selectedObject];
		//
		// Play good sound
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");

		switch (guiDialogBoxes[currentDialogbox].objectType[guiDialogBoxes[currentDialogbox].selectedObject])
		{
			case GUI_OBJECT_BUTTON:
				if (actionSource == KEY_ACTION_MOUSE)
				{
					if (pointInBox (mouseX, mouseY, guiButtons[currentElement].boundingBox))
					{
						paraScriptInstance.run (guiButtons[guiDialogBoxes[currentDialogbox].objectIDIndex[guiDialogBoxes[currentDialogbox].selectedObject]].action, "");
					}
				}
				else
				{
					paraScriptInstance.run (guiButtons[guiDialogBoxes[currentDialogbox].objectIDIndex[guiDialogBoxes[currentDialogbox].selectedObject]].action, "");
				}
				break;
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

	if (keyBinding[KEY_ACTION].active)
	{
		keyBinding[KEY_ACTION].active = false;

		currentElement = guiScreens[currentScreen].objectIDIndex[guiScreens[currentScreen].selectedObject];

		// Play good sound
		gam_addAudioEvent (EVENT_ACTION_AUDIO_PLAY, false, 0, 127, "keyPressGood");

		switch (guiScreens[currentScreen].objectType[guiScreens[currentScreen].selectedObject])
		{
			case GUI_OBJECT_BUTTON:
				if (actionSource == KEY_ACTION_MOUSE)
				{
					if (pointInBox (mouseX, mouseY, guiButtons[currentElement].boundingBox))
					{
						paraScriptInstance.run (guiButtons[guiScreens[currentScreen].objectIDIndex[guiScreens[currentScreen].selectedObject]].action, "");
					}
				}
				else
				{
					paraScriptInstance.run (guiButtons[guiScreens[currentScreen].objectIDIndex[guiScreens[currentScreen].selectedObject]].action, "");
				}
				break;

			case GUI_OBJECT_SLIDER:
				if (actionSource == KEY_ACTION_MOUSE)
				{
					if (pointInBox (mouseX, mouseY, guiSliders[currentElement].boundingBox))
					{
//						paraScriptInstance.run (guiSliders[guiScreens[currentScreen].objectIDIndex[guiScreens[currentScreen].selectedObject]].action, "");
					}
				}
				else
				{
//					paraScriptInstance.run (guiSliders[guiScreens[currentScreen].objectIDIndex[guiScreens[currentScreen].selectedObject]].action, "");
				}
				break;

			case GUI_OBJECT_CHECKBOX:
				if (actionSource == KEY_ACTION_MOUSE)
				{
					if (pointInBox (mouseX, mouseY, guiCheckBoxes[currentElement].boundingBox))
					{
						paraScriptInstance.run (guiCheckBoxes[guiScreens[currentScreen].objectIDIndex[guiScreens[currentScreen].selectedObject]].action, "");
					}
				}
				else
				{
					paraScriptInstance.run (guiCheckBoxes[guiScreens[currentScreen].objectIDIndex[guiScreens[currentScreen].selectedObject]].action, "");
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
		if (returnCode < static_cast<PHYSFS_sint64>(sizeof (keyBinding[keyIndex].keyValue)))
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

	return keyBinding[whichKey].active;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the state of a key based on PARA_Key index. This will be overwritten next frame when the array is updated
void paraGui::setState (int whichKey, bool newState, int newActionSource)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((whichKey < KEY_FIRST) || (whichKey > KEY_NUMBER_ACTIONS))
	{
		funcOutput (-1, "Invalid key index setting key state.");
		return;
	}

	keyBinding[whichKey].active = newState;
	if (whichKey == KEY_ACTION)
		actionSource = newActionSource;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Assign the text description for each key
void paraGui::setKeyDescription ()
//----------------------------------------------------------------------------------------------------------------------
{

	return;

#ifdef WIN_32
	return;
	keyBinding[KEY_LEFT].text       = funcGetString ((string1)"gameLeft");
	keyBinding[KEY_RIGHT].text      = funcGetString ((string1)"gameRight");
	keyBinding[KEY_DOWN].text       = funcGetString ((string1)"gameDown");
	keyBinding[KEY_UP].text         = funcGetString ((string1)"gameUp");
	keyBinding[KEY_PAUSE].text      = funcGetString ((string1)"gamePause");
	keyBinding[KEY_ACTION].text     = funcGetString ((string1)"gameAction");
	keyBinding[KEY_ESCAPE].text     = funcGetString ((string1)"gameEscape");
	keyBinding[KEY_CONSOLE].text    = funcGetString ((string1)"consoleAction");
	keyBinding[KEY_SCREENSHOT].text = funcGetString ((string1)"gameScreenShot");
#else
	keyBinding[KEY_LEFT].text       = funcGetString ("gameLeft");
	keyBinding[KEY_RIGHT].text      = funcGetString ("gameRight");
	keyBinding[KEY_DOWN].text       = funcGetString ("gameDown");
	keyBinding[KEY_UP].text         = funcGetString ("gameUp");
	keyBinding[KEY_PAUSE].text      = funcGetString ("gamePause");
	keyBinding[KEY_ACTION].text     = funcGetString ("gameAction");
	keyBinding[KEY_ESCAPE].text     = funcGetString ("gameEscape");
	keyBinding[KEY_CONSOLE].text    = funcGetString ("consoleAction");
	keyBinding[KEY_SCREENSHOT].text = funcGetString ("gameScreenShot");
#endif
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the text name of the key
string paraGui::getKeyName(int keyIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	string keyName;

	if ((keyIndex < 0) || (keyIndex > KEY_NUMBER_ACTIONS))
		return "Invalid keyIndex";

	keyName = SDL_GetKeyName(SDL_GetKeyFromScancode(keyBinding[keyIndex].keyValue));

	return keyName;
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
// Get the repeat state for keys
int paraGui::getRepeatOff ()
//----------------------------------------------------------------------------------------------------------------------
{
	return repeatOff;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set whether keys repeat when held down or not
void paraGui::setRepeatOff (bool newState)
//----------------------------------------------------------------------------------------------------------------------
{
	repeatOff = newState;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set a new scancode for a key setting
void paraGui::setScancode(int whichKey, int newScancode)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((whichKey < 0) || (whichKey > KEY_NUMBER_ACTIONS))
		return;

	keyBinding[whichKey].keyValue = static_cast<SDL_Scancode>(newScancode);

	keyBinding[whichKey].text = SDL_GetKeyName(SDL_GetKeyFromScancode(keyBinding[whichKey].keyValue));
}

//----------------------------------------------------------------------------------------------------------------------
//
// Return the current scancode for a key setting
int paraGui::getScancode (int whichKey)
//----------------------------------------------------------------------------------------------------------------------
{
	if ((whichKey < 0) || (whichKey > KEY_NUMBER_ACTIONS))
		return -1;

	return keyBinding[whichKey].keyValue;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Update the state of the keyboard mappings from the system keyboard state. Don't repeat keys if repeatOff is true
void paraGui::update ()
//----------------------------------------------------------------------------------------------------------------------
{
	return;

	if (repeatOff)
	{
		for (auto keyCounter = 0; keyCounter != KEY_NUMBER_ACTIONS; keyCounter++)
		{
			if (keyboardState[keyBinding[keyCounter].keyValue])  // Key is down
			{
				if (keyBinding[keyCounter].active && (keyBinding[keyCounter].state == PARA_KEY_UP))
				{
					keyBinding[keyCounter].active = true;
					keyBinding[keyCounter].state  = PARA_KEY_DOWN;
				}
				else if (keyBinding[keyCounter].active && (keyBinding[keyCounter].state == PARA_KEY_DOWN))
				{
					keyBinding[keyCounter].active = false;
				}
			}
			else
			{
				keyBinding[keyCounter].active = false;
				keyBinding[keyCounter].state  = PARA_KEY_UP;
			}
		}
	}
	else
	{

	}
}

//-----------------------------------------------------------------------------
//
// Return how many elements are in this slider
int paraGui::getNumElements (int whichSlider)
//-----------------------------------------------------------------------------
{
	return guiSliders[whichSlider].element.size ();
}

//-----------------------------------------------------------------------------
//
// Get the label for the current step in a slider
std::string paraGui::sliderElementLabel (int whichSlider)
//-----------------------------------------------------------------------------
{
	if (whichSlider > static_cast<int>(guiSliders.size () - 1))
		sys_shutdownWithError ("Invalid index passed to sliderElementLabel");

	return guiSliders[whichSlider].element[guiSliders[whichSlider].currentStep].value;
}

//-----------------------------------------------------------------------------
//
// Return the number of elements in a slider
int paraGui::sliderSize (int whichSlider)
//-----------------------------------------------------------------------------
{
	return guiSliders[whichSlider].element.size ();
}

//-----------------------------------------------------------------------------
//
// Return the selector step position for a slider
int paraGui::getSelectPosition (int whichSlider)
//-----------------------------------------------------------------------------
{
	if ((whichSlider < 0) || (whichSlider > static_cast<int>(guiSliders.size () - 1)))
		return -1;

	return guiSliders[whichSlider].currentStep;
}

//-----------------------------------------------------------------------------
//
// Get the value from the current step of a slider
std::string paraGui::getSliderValue (const std::string &objectID)
//-----------------------------------------------------------------------------
{
	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = getIndex (GUI_OBJECT_SLIDER, objectID);
	if (-1 == objectIndex)
	{
		funcOutput (-1, int_getString ("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str ()));
		return "";
	}

	return guiSliders[objectIndex].element[guiSliders[objectIndex].currentStep].value;
}

//-----------------------------------------------------------------------------
//
// Set the slider to the passed in value
void paraGui::setSliderValue (const std::string &objectID, const std::string &value)
//-----------------------------------------------------------------------------
{
	int indexCount = 0;

	for (auto &sliderItr : guiSliders)
	{
		if (sliderItr.ID == objectID)
		{
			for (auto &stepItr : sliderItr.element)
			{
				switch (stepItr.type)
				{
					case SLIDER_TYPE_INT:
						if (stepItr.value == value)
						{
							sliderItr.currentStep = indexCount;
							return;
						}
						break;

					case SLIDER_TYPE_STRING:
						if (stepItr.value == value)
						{
							sliderItr.currentStep = indexCount;
							return;
						}
						break;
				}
				indexCount += 1;
			}
		}
	}
}

//----------------------------------------------------------------------------------------------------------------------
//
// Add an element to a slider
void paraGui::addNewElement (const std::string &objectID, const std::string &newLabel, const std::string &newValue, int type)
//----------------------------------------------------------------------------------------------------------------------
{
	_sliderElement tmpElement;

	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex = getIndex (GUI_OBJECT_SLIDER, objectID);
	if (-1 == objectIndex)
	{
		funcOutput (-1, int_getString ("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str ()));
		return;
	}

	tmpElement.label = newLabel;
	tmpElement.value = newValue;
	tmpElement.type  = type;

	guiSliders[objectIndex].element.push_back (tmpElement);
}

//----------------------------------------------------------------------------------------------------------------------
//
// Get the ticked status of a checkbox
bool paraGui::getTickedStatus (int objectIndex)
//----------------------------------------------------------------------------------------------------------------------
{
	return guiCheckBoxes[objectIndex].checked;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Set the ticked status of a checkbox - unset all others in the same group
void paraGui::setTickedStatus (const std::string &objectID, int whichGroup, bool newValue)
//----------------------------------------------------------------------------------------------------------------------
{
	int objectIndex = 0;
	//
	// Find the index for this object
	objectIndex                        = getIndex (GUI_OBJECT_CHECKBOX, objectID);
	if (-1 == objectIndex)
	{
		funcOutput (-1, int_getString ("ERROR: Couldn't find GUI object index [ %s ]", objectID.c_str ()));
		return;
	}

	if (-1 == whichGroup)
	{
		guiCheckBoxes[objectIndex].checked = newValue;
		return;
	}

	for (auto &boxItr : guiCheckBoxes)
	{
		if (whichGroup == boxItr.group)
			boxItr.checked = false;
	}
	guiCheckBoxes[objectIndex].checked = newValue;
}

//----------------------------------------------------------------------------------------------------------------------
//
// Process the key events
void paraGui::processGuiInput ()
//----------------------------------------------------------------------------------------------------------------------
{
	if (currentDialogbox == NO_DIALOG_BOX)
	{
		processMousePosition ();
		processMovementKeys ();
		processAction ();
	}
	else
	{
		processMousePositionDialogbox ();
		processMovementKeysDialogbox ();
		processActionDialogbox();
	}

}