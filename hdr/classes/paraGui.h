#ifndef PARA_PARAGUI_H
#define PARA_PARAGUI_H

#include <string>
#include <vector>

#define DEBUG_GUI_SETUP 1

typedef void (*funcPtrIntStr) (int, std::string);

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

struct __GUI_OBJECT
{
	bool ready                  = false;
	bool canFocus               = false;
	bool positionCalled         = false;

	std::string    ID;
	int            screenID     = -1;  // Which screen does this object belong to
	int            labelPos;
	int            cornerRadius = 0;
	int            gapSize      = 0;
	std::string    label;
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

	void AddRef ();

	void ReleaseRef();

	void init (funcPtrIntStr outputFunction, int newRenderWidth, int newRenderHeight);

	std::string int_getString (std::string format, ...);
	//
	// Set things
	void setOutputFunction (funcPtrIntStr outputFunction);

	void setRenderDimensions (int width, int height);
	//
	// Functions used in GUI script
	void setLabel (int objectType, std::string objectID, int gapSize, int newLabelPos, std::string newLabel);

	void setAction (int objectType, std::string objectID, std::string newAction);

	void setFontName (int objectType, std::string objectID, std::string newFontName);

	void setPosition (int objectType, std::string objectID, int newRadius, int coordType, double newPosX, double newPosY, double newWidth, double newHeight);

	void addToScreen (int objectType, std::string objectID, std::string screenID);

	void setColor (int objectType, std::string objectID, int whichColor, int red, int green, int blue, int alpha);

	void setActive (std::string objectID);

	void setReady (int objectType, std::string objectID, bool newState);

	void create (int objectType, std::string objectID);
	//
	// Used to get attributes when rendering
	int numElements();

	int selectedObject();

	int typeByIndex(int whichObject);

	int indexByIndex(int whichObject);

	int getIndex (int objectType, std::string objectID);

	int getRadius (int objectType, int objectIndex);

	int getGapSize (int objectType, int objectIndex);

	int getLabelPos (int objectType, int objectIndex);

	std::string getLabelText (int objectType, int objectIndex);

	__BOUNDING_BOX getBB (int objectType, int objectIndex);

	__PARA_COLOR getColor (int objectType, int objectIndex, int whichColor);

	std::string getFontName (int objectType, int objectIndex);

	void restart();

	bool isReady (int objectType, int objectIndex);

private:

	void setColorByIndex (int objectType, int objectIndex, int whichColor, int red, int green, int blue, int alpha);

	int                          currentScreen = 0;
	double                       renderWidth   = 0;
	double                       renderHeight  = 0;
	std::vector<__SCREEN_OBJECT> guiScreens;    // ** Add new vectors to restart for .clear()
	std::vector<__GUI_OBJECT>    guiButtons;
	funcPtrIntStr                funcOutput;
};

#endif //PARA_PARAGUI_H
