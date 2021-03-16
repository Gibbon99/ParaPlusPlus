#include "system/util.h"
#include "gui/guiTextbox.h"

//-------------------------------------------------------------------------------------------------------------------
//
// Render the text within the confines of a text box
void gui_renderTextbox (int whichObject)
//-------------------------------------------------------------------------------------------------------------------
{
	__BOUNDING_BOX bb{};
	__PARA_COLOR   color{};
	std::string    textCopy;
	std::string    nextLine;
	std::string    fontName;
	float          printStartY = 0.0f;
	float          textLength  = 0.0f;
	char           *p          = nullptr;

	//
	// Check to see if it's ready to use or not
	if (!gui.isReady (GUI_OBJECT_TEXTBOX, whichObject))
		return;
	//
	// Get the font to use to render the textbox
	fontName = gui.getFontName (GUI_OBJECT_TEXTBOX, whichObject);
	if (fontName.size () == 0)
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get font for textbox [ %i ]", whichObject));
		return;
	}
	fontClass.use (fontName);
	//
	// Get the position to render the textbox
	bb = gui.getBB (GUI_OBJECT_TEXTBOX, whichObject);
	if ((bb.x1 == -1) || (bb.y1 == -1) || (bb.x2 == -1) || (bb.y2 == -1))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get position for textbox [ %i ]", whichObject));
		return;
	}
	color = gui.getColor (GUI_OBJECT_TEXTBOX, whichObject, GUI_COL_ACTIVE);
	if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for textbox [ %i ]", whichObject));
		return;
	}

	nextLine.clear ();
	textLength = 0;

	printStartY = bb.y1;
	//
	// strtok modifies the string - make a copy
	textCopy    = gui.getLabelText (GUI_OBJECT_TEXTBOX, whichObject);
	p           = strtok ((char *) textCopy.c_str (), " ");

	while (p != nullptr)
	{
		textLength = fontClass.width(nextLine + " " + p);
		if (*p == '!')
			textLength = bb.x2 - bb.x1;

		if (textLength < (bb.x2 - bb.x1))
		{
			nextLine.append(p);
			nextLine.append(" ");
		}
		else
		{
			fontClass.render(renderer.renderer, bb.x1, printStartY, color.r, color.g, color.b, color.a, nextLine);
			printStartY += fontClass.height();
			nextLine.clear();
			if (*p != '!')
				nextLine.append (p);

			nextLine.append(" ");
		}
		p = strtok (nullptr, " ");
	}
	fontClass.render(renderer.renderer, bb.x1, printStartY, color.r, color.g, color.b, color.a, nextLine);
}