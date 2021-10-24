#pragma once

#include <string>
#include <vector>
#include <wrapper.h>
#include "../../data/scripts/enum.h"

#define DEBUG_GUI_SETUP 1

typedef void (*funcPtrIntStr)(int, std::string);

typedef std::string &string1;

typedef std::string (*funcStrIn)(std::string);     // Function to provide the key descriptions

struct __BOUNDING_BOX
{
	double x1;
	double y1;
	double x2;
	double y2;
};

struct __PARA_COLOR
{
	int r;
	int g;
	int b;
	int a;
};

struct __SCREEN_OBJECT
{
	int              selectedObject;    // Whats currently active on this screen
	std::string      ID;                // Name of this screen
	std::vector<int> objectIDIndex;     // Index into object array
	std::vector<int> objectType;        // Which object array
};

struct __GUI_DIALOGBOX
{
	bool             ready          = false;
	bool             positionCalled = false;
	int              selectedObject;    // Whats currently active on this screen
	std::string      ID;                // Name of this screen
	std::vector<int> objectIDIndex;     // Index into object array
	std::vector<int> objectType;        // Which object array
	std::string      label;
	std::string      fontName;
	int              cornerRadius;
	int              gapSize;
	__PARA_COLOR     hasFocusColor;
	__PARA_COLOR     labelFocusColor;
	__BOUNDING_BOX   boundingBox;
};

struct __GUI_OBJECT
{
	bool           ready          = false;
	bool           canFocus       = false;
	bool           positionCalled = false;
	std::string    ID;
	int            screenID       = -1;  // Which screen does this object belong to
	int            labelPos;
	int            cornerRadius   = 0;
	int            gapSize        = 0;
	std::string    label;
	std::string    fontName;
	std::string    action;
	__PARA_COLOR   hasFocusColor;
	__PARA_COLOR   noFocusColor;
	__PARA_COLOR   labelFocusColor;
	__PARA_COLOR   labelNoFocusColor;
	__BOUNDING_BOX boundingBox;
};

struct __KeyBindings
{
	std::string text;
	PARA_Scancode keyValue;
	bool        active;
	int         state;
};

struct _sliderElement
{
	std::string label;    // Text to display
	std::string value;    // Value of this element
	int         type;    // What type of element - convert on query
};

struct __GUI_SLIDER
{
	bool                        ready        = false;
	bool                        canFocus;
	bool                        positionCalled;
	std::string                 ID;
	int                         labelPos;
	int                         gapSize;
	int                         cornerRadius = 0;
	int                         coordType;
	int                         currentStep;  // What is selected
	int                         numberSteps;  // How many elements
	int                         barThickness;  // Thickness for drawing the bar
	std::string                 label;
	std::string                 fontName;
	std::string                 action;
	__PARA_COLOR                hasFocusColor;
	__PARA_COLOR                noFocusColor;
	__PARA_COLOR                labelFocusColor;
	__PARA_COLOR                labelNoFocusColor;
	__BOUNDING_BOX              boundingBox;
	std::vector<_sliderElement> element;    // Value for this step
};

struct __GUI_SCROLLBOX
{
	bool                     ready        = false;
	bool                     canFocus;
	bool                     positionCalled;
	std::string              ID;
	int                      labelPos;
	int                      gapSize;
	int                      cornerRadius = 0;
	int                      coordType;
	int                      currentChar  = 0;
	int                      numLinesToPrint;
	double                   lineFade;
	double                   scrollDelay  = 0.2;
	double                   scrollY;
	double                   previousScrollY;
	double                   scrollSpeed;
	std::string              label;
	std::string              fontName;
	std::string              action;
	__PARA_COLOR             hasFocusColor;
	__PARA_COLOR             labelFocusColor;
	__BOUNDING_BOX           boundingBox;
	std::vector<std::string> scrollBoxText;
};

struct __GUI_CHECKBOX
{
	bool           ready          = false;
	bool           canFocus       = false;
	bool           positionCalled = false;
	bool           checked        = false;
	std::string    ID;
	int            screenID       = -1;  // Which screen does this object belong to
	int            labelPos;
	int            cornerRadius   = 0;
	int            gapSize        = 0;
	int            group          = -1;
	std::string    label;                   // Image KeyName for ImageType
	std::string    fontName;
	std::string    action;
	__PARA_COLOR   hasFocusColor;
	__PARA_COLOR   noFocusColor;
	__PARA_COLOR   labelFocusColor;
	__PARA_COLOR   labelNoFocusColor;
	__BOUNDING_BOX boundingBox;
};

//----------------------------------------------------------------------------------------------------------------------
//
// Class definition for GUI
class paraGui
//----------------------------------------------------------------------------------------------------------------------
{
public:

	void AddRef();

	void ReleaseRef();

	void init(funcPtrIntStr outputFunction, funcStrIn getStringFunc, int newRenderWidth, int newRenderHeight, int newRenderWidthGame, int newRenderHeightGame, std::string newFileName);

	std::string int_getString(std::string format, ...);

	//
	// Set things
	void setOutputFunction(funcPtrIntStr outputFunction);

	void setRenderDimensions(int width, int height);

	//
	// Functions used in GUI script
	void setLabel(int objectType, std::string objectID, int newGapSize, int newLabelPos, std::string newLabel);

	void setAction(int objectType, std::string objectID, std::string newAction);

	void setFontName(int objectType, std::string objectID, std::string newFontName);

	void setPosition(int objectType, std::string objectID, int newRadius, int coordType, double newPosX, double newPosY, double newWidth, double newHeight);

	void addToScreen(int objectType, std::string objectID, std::string screenID);

	void setColor(int objectType, std::string objectID, int whichColor, int red, int green, int blue, int alpha);

	void setActive(const std::string &objectID);

	void setReady(int objectType, std::string objectID, bool newState);

	void create(int objectType, std::string objectID);

	int getCurrentScreen();

	void setCurrentScreen(int newScreen);

	void setScrollSpeed(int objectType, const std::string &objectID, double newScrollSpeed);

	void getNextLineOfText(int objectIndex);

	double getScrollDelay(int objectIndex);

	void setScrollDelay(int objectIndex, double newScrollDelay);

	double getScrollY(int objectIndex);

	void setScrollY(int objectIndex, double newScrollY);

	void restartScrollBox(const std::string &objectID);

	double getPreviousScrollY(int objectIndex);

	int getNumberPrintLines(int objectIndex);

	void setNumberPrintLines(int objectIndex, int newNumberLines);

	void setPreviousScrollY(int objectIndex, double newScrollY);

	bool getTickedStatus(int objectIndex);

	void setTickedStatus(const std::string &objectID, int whichGroup, bool newValue);

	void setActiveObject(int whichScreen, int objectType, std::string objectID);

	int getActiveObjectIndex();

	std::vector<std::string>::reverse_iterator getRBegin(int objectIndex);

	std::vector<std::string>::reverse_iterator getREnd(int objectIndex);

	double getLineFade(int objectIndex);

	void setLineFade(int objectIndex, double newLineFade);

	void addNewElement(const std::string &objectID, const std::string &newLabel, const std::string &newValue, int type);

	void setSliderValue(const std::string &objectID, const std::string &value);

	int getSelectPosition(int whichSlider);

	int sliderSize(int whichSlider);

	std::string sliderElementLabel(int whichSlider);

	std::string getSliderValue(const std::string &objectID);

	int getNumElements(int whichSlider);

	//
	// Used to get attributes when rendering
	int numElements();

	int selectedObject();

	int typeByIndex(int whichObject);

	int indexByIndex(int whichObject);

	int getIndex(int objectType, const std::string &objectID);

	int getRadius(int objectType, int objectIndex);

	int getGapSize(int objectType, int objectIndex);

	int getLabelPos(int objectType, int objectIndex);

	std::string getLabelText(int objectType, int objectIndex);

	__BOUNDING_BOX getBB(int objectType, int objectIndex);

	__PARA_COLOR getColor(int objectType, int objectIndex, int whichColor);

	std::string getFontName(int objectType, int objectIndex);

	void restart();

	bool isReady(int objectType, int objectIndex);

	bool pointInBox(int x, int y, __BOUNDING_BOX checkBox);

	bool canBeSelected(int objectType, int whichObject);

	void processMousePosition();

	void processMovementKeys();

	void processAction();

	void processGuiInput();

	//
	// Dialogbox routines
	//
	int getActiveObjectIndexDialogbox();

	int getCurrentDialogbox();

	void setActiveObjectDialogbox(int whichDialogbox, int objectType, std::string objectID);

	void setCurrentDialogbox(int newDialogbox);

	int indexByIndexDialogbox(int whichObject);

	int typeByIndexDialogbox(int whichObject);

	int selectedObjectDialogbox();

	int numElementsDialogbox();

	void addToDialogbox(int objectType, std::string objectID, std::string dialogboxID);

	void setActiveDialogbox(std::string objectID);

	void processMousePositionDialogbox();

	void processMovementKeysDialogbox();

	void setRenderDimensionsGameMode(int width, int height);

	void processActionDialogbox();

	//
	// Input related functions
	//
	void setRepeatOff(bool newState);

	int getRepeatOff() const;

	void setDefaultKeybindings();

	void setKeyDescription();

	void load();

	void save();

	void update();

	void print();

	bool keyDown(int whichKey);

	void setState(int whichKey, bool newState, int newActionSource);

	void setMouse(int newPosX, int newPosY);

	int getScancode(int whichKey);

	std::string getKeyName(int keyIndex);

	void setScancode(int whichKey, int newScancode);

	const Uint8 *keyboardState;

private:

	void setColorByIndex(int objectType, int objectIndex, int whichColor, int red, int green, int blue, int alpha);

	int                          mouseX;
	int                          mouseY;
	int                          actionSource;
	__KeyBindings                keyBinding[KEY_NUMBER_ACTIONS];
	std::string                  fileName;
	funcStrIn                    funcGetString;
	bool                         repeatOff        = true;
	int                          currentScreen    = 0;
	int                          currentDialogbox = NO_DIALOG_BOX;
	double                       renderWidth      = 0;
	double                       renderHeight     = 0;
	int                          renderWidthGame  = 0;
	int                          renderHeightGame = 0;
	std::vector<__SCREEN_OBJECT> guiScreens;    // ** Add new vectors to restart() for .clear() when adding a new element
	std::vector<__GUI_DIALOGBOX> guiDialogBoxes;
	std::vector<__GUI_OBJECT>    guiButtons;
	std::vector<__GUI_SLIDER>    guiSliders;
	std::vector<__GUI_OBJECT>    guiLabels;
	std::vector<__GUI_OBJECT>    guiTextboxes;
	std::vector<__GUI_OBJECT>    guiImages;
	std::vector<__GUI_SCROLLBOX> guiScrollBoxes;
	std::vector<__GUI_CHECKBOX>  guiCheckBoxes;
	funcPtrIntStr                funcOutput;
};