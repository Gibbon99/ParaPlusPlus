#include <box2d/b2_math.h>
#include <game/shipDecks.h>
#include <system/util.h>
#include <game/lightMaps.h>
#include <game/audio.h>
#include "game/alertLevel.h"

std::vector<b2Vec2> tileLocationsInPixels;
static int          currentAlertLevel = -1;

//------------------------------------------------------------------------------------------------------
//
// Change alert level background sound
void gam_changeAlertLevelSound(int oldAlertLevel, int newAlertLevel)
//------------------------------------------------------------------------------------------------------
{
	switch (oldAlertLevel)
	{
		case ALERT_GREEN_TILE:
			gam_addAudioEvent(EVENT_ACTION_AUDIO_STOP, false, 0, 0, "greenAlert");
			break;

		case ALERT_YELLOW_TILE:
			gam_addAudioEvent(EVENT_ACTION_AUDIO_STOP, false, 0, 0, "yellowAlert");
			break;

		case ALERT_RED_TILE:
			gam_addAudioEvent(EVENT_ACTION_AUDIO_STOP, false, 0, 0, "redlert");
			break;
	}

	switch (newAlertLevel)
	{
		case ALERT_GREEN_TILE:
			gam_addAudioEvent(EVENT_ACTION_AUDIO_PLAY, true, 128, 127, "greenAlert");
			break;

		case ALERT_YELLOW_TILE:
			gam_addAudioEvent(EVENT_ACTION_AUDIO_PLAY, true, 128, 127, "yellowAlert");
			break;

		case ALERT_RED_TILE:
			gam_addAudioEvent(EVENT_ACTION_AUDIO_PLAY, true, 128, 127, "redAlert");
			break;
	}

}

//------------------------------------------------------------------------------------------------------
//
// Set the new alert level
void gam_setAlertLevel (int newAlertLevel)
//------------------------------------------------------------------------------------------------------
{
	if (currentAlertLevel != newAlertLevel)
	{
		gam_changeAlertLevelSound(currentAlertLevel, newAlertLevel);
		currentAlertLevel = newAlertLevel;
		gam_changeAlertColor (newAlertLevel);
		gam_renderAlertTiles ();
	}
}

//------------------------------------------------------------------------------------------------------
//
// Get the current alert level
int gam_getCurrentAlertLevel ()
//------------------------------------------------------------------------------------------------------
{
	return currentAlertLevel;
}

//------------------------------------------------------------------------------------------------------
//
// Locate all the alert level tiles and store their location
void gam_locateAlertTiles ()
//------------------------------------------------------------------------------------------------------
{
	int    countX      = 0;
	int    countY      = 0;
	int    currentTile = 0;
	b2Vec2 tileWorldPos;

	if (tileLocationsInPixels.size () > 0)
		tileLocationsInPixels.clear ();

	for (auto index = 0; index < static_cast<int>(g_shipDeckItr->second.levelDimensions.x) * static_cast<int>(g_shipDeckItr->second.levelDimensions.y); index++)
	{
		currentTile = g_shipDeckItr->second.tiles[((countY * (static_cast<int>(g_shipDeckItr->second.levelDimensions.x))) + countX)];

		switch (currentTile)
		{
			case ALERT_GREEN_TILE:
			case ALERT_YELLOW_TILE:
			case ALERT_RED_TILE:
				tileWorldPos.x = (countX * tileSize);
				tileWorldPos.y = (countY * tileSize);
				tileLocationsInPixels.push_back (tileWorldPos);
				gam_addNewLightmap (sys_convertToMeters (tileWorldPos), LIGHTMAP_TYPE_ALERT, 0);
				break;

			default:
				break;
		}

		countX++;

		if (countX == static_cast<int>(g_shipDeckItr->second.levelDimensions.x))
		{
			countX = 0;
			countY++;
		}
	}
}

//------------------------------------------------------------------------------------------------------
//
// Render the alert level tiles according to the current alert level
void gam_renderAlertTiles ()
//------------------------------------------------------------------------------------------------------
{
	PARA_Texture *tempTexture;

	tempTexture = SDL_GetRenderTarget (renderer.renderer);

	SDL_SetRenderTarget (renderer.renderer, gam_getPlayfieldTexture ());

	for (const auto &alertTileItr : tileLocationsInPixels)
	{
		gam_renderSingleTile (static_cast<int>(alertTileItr.x), static_cast<int>(alertTileItr.y), currentAlertLevel);
	}

	SDL_SetRenderTarget (renderer.renderer, tempTexture);
}