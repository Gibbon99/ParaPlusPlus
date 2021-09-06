#include <io/fileSystem.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <io/logFile.h>
#include <system/util.h>
#include <game/lifts.h>
#include <game/alertLevel.h>
#include "gui/guiSideview.h"

_basicTunnel        tunnel[NUM_OF_TUNNELS];
_sideviewBasicLevel sideviewLevels[MAX_LEVELS];
_sideviewColors     sideviewColors[SIDEVIEW_NUM_COLORS];
float               sideviewDrawScale;        // From config file

// ----------------------------------------------------------------------------
//
// Wrap drawing a rectangle into our own function
void gui_sideviewDrawRect (float x1, float y1, float x2, float y2, __PARA_COLOR whichColor)
// ----------------------------------------------------------------------------
{
	roundedBoxRGBA (renderer.renderer, x1, y1, x2, y2, 3, whichColor.r, whichColor.g, whichColor.b, whichColor.a);

	roundedRectangleRGBA (renderer.renderer, x1, y1, x2, y2, 3, 0, 0, 0, whichColor.a);
}

//-----------------------------------------------------------------------------
//
// Create a color into the sideview array
void gui_createSideViewColor (int index, int red, int green, int blue, int alpha)
//-----------------------------------------------------------------------------
{
	if ((index < 0) || (index > SIDEVIEW_NUM_COLORS))
		return;

	sideviewColors[index].color.r = red;
	sideviewColors[index].color.g = green;
	sideviewColors[index].color.b = blue;
	sideviewColors[index].color.a = alpha;
}

//-----------------------------------------------------------------------------
//
// setup the way the tunnels are linked to the levels
void gui_setupTunnels ()
//-----------------------------------------------------------------------------
{
	tunnel[0].top    = 19;
	tunnel[0].bottom = 3;
	tunnel[0].decks[0] = 3;
	tunnel[0].decks[1] = 4;
	tunnel[0].decks[2] = 15;
	tunnel[0].decks[3] = 16;
	tunnel[0].decks[4] = 19;
	tunnel[0].current_deck = 0;
	tunnel[0].current      = tunnel[0].decks[tunnel[0].current_deck];

	tunnel[1].top    = 6;
	tunnel[1].bottom = 0;
	tunnel[1].decks[0] = 0;
	tunnel[1].decks[1] = 5;
	tunnel[1].decks[2] = 6;
	tunnel[1].current_deck = 0;
	tunnel[1].current      = tunnel[1].decks[tunnel[1].current_deck];

	tunnel[2].top    = 17;
	tunnel[2].bottom = 15;
	tunnel[2].decks[0] = 15;
	tunnel[2].decks[1] = 17;
	tunnel[2].current_deck = 0;
	tunnel[2].current      = tunnel[2].decks[tunnel[2].current_deck];

	tunnel[3].top    = 20;
	tunnel[3].bottom = 12;
	tunnel[3].decks[0] = 12;
	tunnel[3].decks[1] = 13;
	tunnel[3].decks[2] = 14;
	tunnel[3].decks[3] = 17;
	tunnel[3].decks[4] = 20;
	tunnel[3].current_deck = 0;
	tunnel[3].current      = tunnel[3].decks[tunnel[3].current_deck];

	tunnel[4].top    = 12;
	tunnel[4].bottom = 1;
	tunnel[4].decks[0] = 1;
	tunnel[4].decks[1] = 5;
	tunnel[4].decks[2] = 6;
	tunnel[4].decks[3] = 12;
	tunnel[4].current_deck = 0;
	tunnel[4].current      = tunnel[4].decks[tunnel[4].current_deck];

	tunnel[5].top    = 9;
	tunnel[5].bottom = 2;
	tunnel[5].decks[0] = 2;
	tunnel[5].decks[1] = 8;
	tunnel[5].decks[2] = 9;
	tunnel[5].current_deck = 0;
	tunnel[5].current      = tunnel[5].decks[tunnel[5].current_deck];

	tunnel[6].top    = 14;
	tunnel[6].bottom = 9;
	tunnel[6].decks[0] = 9;
	tunnel[6].decks[1] = 10;
	tunnel[6].decks[2] = 11;
	tunnel[6].decks[3] = 12;
	tunnel[6].decks[4] = 13;
	tunnel[6].decks[5] = 14;
	tunnel[6].current_deck = 0;
	tunnel[6].current      = tunnel[6].decks[tunnel[6].current_deck];

	tunnel[7].top    = 18;
	tunnel[7].bottom = 14;
	tunnel[7].decks[0] = 14;
	tunnel[7].decks[1] = 18;
	tunnel[7].current_deck = 0;
	tunnel[7].current      = tunnel[7].decks[tunnel[7].current_deck];
}

// ----------------------------------------------------------------------------
//
// load the sideview data from the external file
bool gui_loadSideViewData (std::string sideviewFileName)
// ----------------------------------------------------------------------------
{
	int           fileLength;
	char          *fileBuffer = nullptr;
	int           numberLevels;
	int           count;
	int           buf[1];
	unsigned char levelCount[1];
	SDL_RWops     *fp;

	float smallX, largeX;

	smallX = 1000;
	largeX = 0;

	fileLength = (int) fileSystem.getFileSize (sideviewFileName);
	if (fileLength < 0)
		sys_shutdownWithError (sys_getString ("Fatal error getting level file size [ %s ].", sideviewFileName.c_str ()));

	fileBuffer = sys_malloc (fileLength, sideviewFileName);
	if (nullptr == fileBuffer)
		sys_shutdownWithError (sys_getString ("Fatal memory allocation error when loading level file [ %s ].", sideviewFileName.c_str ()));

	if (-1 == fileSystem.getFileIntoMemory (sideviewFileName, fileBuffer))
		sys_shutdownWithError (sys_getString ("Fatal memory allocation when loading file [ %s ].", sideviewFileName.c_str ()));

	fp = io_openMemFile (fileBuffer, fileLength);
	if (nullptr == fp)
		sys_shutdownWithError (sys_getString ("Mapping memory to file failed for file [ %s ]", sideviewFileName.c_str ()));

	SDL_RWread (fp, &levelCount, sizeof (levelCount), 1);
	numberLevels = levelCount[0];

	if (MAX_LEVELS != numberLevels)
		sys_shutdownWithError (sys_getString ("Bad format or date when reading file [ %s ]", sideviewFileName.c_str ()));

	for (count = 0; count != numberLevels; count++)
	{
		float temp;

		SDL_RWread (fp, (void *) &buf, sizeof (sideviewLevels[count].x1), 1);
		temp = (float) buf[0] * sideviewDrawScale;
		sideviewLevels[count].x1 = temp;

		if (sideviewLevels[count].x1 < smallX)
		{
			smallX = sideviewLevels[count].x1;
		}

		SDL_RWread (fp, (void *) &buf, sizeof (sideviewLevels[count].y1), 1);
		temp = (float) (buf[0] - 100.0f) * sideviewDrawScale;
		sideviewLevels[count].y1 = temp;

		SDL_RWread (fp, (void *) &buf, sizeof (sideviewLevels[count].x2), 1);
		temp = (float) buf[0] * sideviewDrawScale;
		sideviewLevels[count].x2 = temp;

		if (sideviewLevels[count].x2 > largeX)
		{
			largeX = sideviewLevels[count].x2;
		}

		SDL_RWread (fp, (void *) &buf, sizeof (sideviewLevels[count].y2), 1);
		temp = static_cast<float>((buf[0]) - 100.0f) * sideviewDrawScale;
		sideviewLevels[count].y2 = temp;
	}
	SDL_RWclose (fp);

	gui_setupTunnels ();

	return true;
}

// ----------------------------------------------------------------------------
//
// Show the ship in its sideview on the screen
void gui_renderSideView ()
// ----------------------------------------------------------------------------
{
	int           count;
	int           lifts   = 8;
	int           toLifts = 0;
	float         x1;
	float         y1;
	double        sideViewTextPosX;
	double        sideViewTextPosY;
	Uint8         r, g, b, a;
	SDL_BlendMode tempMode;
	__PARA_COLOR  tempAlert{};

	fontClass.use ("guiFont");

	sideViewTextPosX = 5;
	sideViewTextPosY = renderer.renderHeight () - (fontClass.height ());
	//
	// SDL2_gfx changes the blend mode and draw color
	// as part of its rendering - remember so we can change it back
	//
//	SDL_GetRenderDrawColor (renderer.renderer, &r, &g, &b, &a);
//	SDL_GetRenderDrawBlendMode (renderer.renderer, &tempMode);

	sideviewStarfield.render ();

	try
	{
		textures.at ("planetAlpha").render ();
	}
	catch (std::out_of_range &outOfRange)
	{
		std::cout << "Can not find texture [ planetAlpha ] to render." << std::endl;
	}



	//
	// Draw hold level
	//
	count = 0;
	x1    = sideviewLevels[count].x2;
	y1    = sideviewLevels[count].y2;
	gui_sideviewDrawRect (x1, y1, sideviewLevels[count].x1, sideviewLevels[count].y1, sideviewColors[SIDEVIEW_SHIP_COLOR].color);
	toLifts++;

	//
	// Draw all the decks in normal color
	//
	for (count = 1; count != MAX_LEVELS - lifts; count++)
	{
		gui_sideviewDrawRect (sideviewLevels[count].x1, sideviewLevels[count].y1, sideviewLevels[count].x2, sideviewLevels[count].y2, sideviewColors[SIDEVIEW_SHIP_COLOR].color);
		toLifts++;
	}

	//
	// Highlite the current deck in use for lift mode
	//
	if (currentMode == MODE_GUI_LIFTVIEW)
	{
		//
		// Highlite current level
		if (0 == currentDeckNumber)
		{
			gui_sideviewDrawRect (sideviewLevels[currentDeckNumber].x2, sideviewLevels[currentDeckNumber].y2, sideviewLevels[currentDeckNumber].x1, sideviewLevels[currentDeckNumber].y1, sideviewColors[SIDEVIEW_ACTIVE_DECK_COLOR].color);
		}
		else
		{
			gui_sideviewDrawRect (sideviewLevels[currentDeckNumber].x1, sideviewLevels[currentDeckNumber].y1, sideviewLevels[currentDeckNumber].x2, sideviewLevels[currentDeckNumber].y2, sideviewColors[SIDEVIEW_ACTIVE_DECK_COLOR].color);
		}
	}
	else    // Static view of ship from terminal
	{
		switch (gam_getCurrentAlertLevel ())
		{
			case ALERT_GREEN_TILE:
				tempAlert.r = 0;
				tempAlert.g = 255;
				tempAlert.b = 0;
				tempAlert.a = 255;
				break;

			case ALERT_YELLOW_TILE:
				tempAlert.r = 255;
				tempAlert.g = 255;
				tempAlert.b = 0;
				tempAlert.a = 255;
				break;

			case ALERT_RED_TILE:
				tempAlert.r = 255;
				tempAlert.g = 0;
				tempAlert.b = 0;
				tempAlert.a = 255;
				break;

			default:
				tempAlert.r = 255;
				tempAlert.g = 255;
				tempAlert.b = 255;
				tempAlert.a = 255;
				break;
		}

		if (0 == currentDeckNumber)
		{
			gui_sideviewDrawRect (sideviewLevels[currentDeckNumber].x2, sideviewLevels[currentDeckNumber].y2, sideviewLevels[currentDeckNumber].x1, sideviewLevels[currentDeckNumber].y1, tempAlert);
		}
		else
		{
			gui_sideviewDrawRect (sideviewLevels[currentDeckNumber].x1, sideviewLevels[currentDeckNumber].y1, sideviewLevels[currentDeckNumber].x2, sideviewLevels[currentDeckNumber].y2, tempAlert);
		}
	}

	//
	// Redraw the level and tunnel that overlap
	//
	if (currentDeckNumber == 13)
		gui_sideviewDrawRect (sideviewLevels[13].x1, sideviewLevels[13].y1, sideviewLevels[13].x2, sideviewLevels[13].y2, sideviewColors[SIDEVIEW_ACTIVE_DECK_COLOR].color);
	else
	{
		count = 13;
		gui_sideviewDrawRect (sideviewLevels[count].x1, sideviewLevels[count].y1, sideviewLevels[count].x2, sideviewLevels[count].y2, sideviewColors[SIDEVIEW_SHIP_COLOR].color);
	}

	if (currentDeckNumber == 3)
	{
		gui_sideviewDrawRect (sideviewLevels[3].x1, sideviewLevels[3].y1, sideviewLevels[3].x2, sideviewLevels[3].y2, sideviewColors[SIDEVIEW_ACTIVE_DECK_COLOR].color);
	}
	else
	{
		count = 3;
		gui_sideviewDrawRect (sideviewLevels[count].x1, sideviewLevels[count].y1, sideviewLevels[count].x2, sideviewLevels[count].y2, sideviewColors[SIDEVIEW_SHIP_COLOR].color);
	}
	//
	// fill in engine part
	//
	gui_sideviewDrawRect (sideviewLevels[7].x1, sideviewLevels[7].y1, sideviewLevels[7].x2, sideviewLevels[7].y2, sideviewColors[SIDEVIEW_ENGINE_COLOR].color);
	//
	// draw the lifts
	//
	for (count = 0; count != lifts; count++)
	{
		if (currentMode == MODE_GUI_LIFTVIEW)      // Only draw highlighted tunnel in lift view
		{
			if (gam_getCurrentTunnel () == count) // Draw currentTunnel in use
				gui_sideviewDrawRect (sideviewLevels[count + toLifts].x1, sideviewLevels[count + toLifts].y1, sideviewLevels[count + toLifts].x2, sideviewLevels[count + toLifts].y2, sideviewColors[SIDEVIEW_ACTIVE_LIFT_COLOR].color);
			else
				gui_sideviewDrawRect (sideviewLevels[count + toLifts].x1, sideviewLevels[count + toLifts].y1, sideviewLevels[count + toLifts].x2, sideviewLevels[count + toLifts].y2, sideviewColors[SIDEVIEW_LIFT_COLOR].color);
		}
		else
			gui_sideviewDrawRect (sideviewLevels[count + toLifts].x1, sideviewLevels[count + toLifts].y1, sideviewLevels[count + toLifts].x2, sideviewLevels[count + toLifts].y2, sideviewColors[SIDEVIEW_LIFT_COLOR].color);
	}

	fontClass.render (renderer.renderer, sideViewTextPosX, sideViewTextPosY, 255, 255, 255, 255, sys_getString ("Deck [ %s ]", gam_returnLevelNameFromDeck (currentDeckNumber).c_str ()));

//	SDL_SetRenderDrawColor (renderer.renderer, r, g, b, a);
//	SDL_SetRenderDrawBlendMode (renderer.renderer, tempMode);
}