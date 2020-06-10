#include <system/util.h>
#include "gui/guiImage.h"

//----------------------------------------------------------------------------------------------------------------------
//
// Render an image from the gui script
void gui_renderImage (int whichObject)
//----------------------------------------------------------------------------------------------------------------------
{
	__BOUNDING_BOX bb{};
	__PARA_COLOR   color{};
	SDL_Rect       renderPosition;

	//
	// Check to see if it's ready to use or not
	if (!gui.isReady (GUI_OBJECT_IMAGE, whichObject))
		return;

	//
	// Get the position to render the image
	bb = gui.getBB (GUI_OBJECT_IMAGE, whichObject);
	if ((bb.x1 == -1) || (bb.y1 == -1) || (bb.x2 == -1) || (bb.y2 == -1))
	{
		con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get position for image [ %i ]", whichObject));
		return;
	}

	try
	{
		renderPosition.x = bb.x1;
		renderPosition.y = bb.y1;
		renderPosition.w = textures.at (gui.getLabelText (GUI_OBJECT_IMAGE, whichObject)).getWidth ();
		renderPosition.h = textures.at (gui.getLabelText (GUI_OBJECT_IMAGE, whichObject)).getHeight ();

		color = gui.getColor (GUI_OBJECT_IMAGE, whichObject, GUI_COL_ACTIVE);
		if ((color.r == -1) || (color.g == -1) || (color.b == -1) || (color.a == -1))
		{
			con_addEvent (EVENT_ACTION_CONSOLE_ADD_CHAR_LINE, sys_getString ("Unable to get colors for image [ %i ]", whichObject));
			return;
		}

		textures.at (gui.getLabelText (GUI_OBJECT_IMAGE, whichObject)).render (&renderPosition);
	}

	catch (std::out_of_range &OutOfRange)
	{
		con_addEvent(EVENT_ACTION_CONSOLE_ADD_LINE, sys_getString("Unable to locate GUI image [ %s ] ", gui.getLabelText(GUI_OBJECT_IMAGE, whichObject).c_str()));
	}
}
