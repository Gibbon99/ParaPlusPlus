#include <system/util.h>
#include <sdl2_gfx/SDL2_gfxPrimitives.h>
#include <game/transferDroidAI.h>
#include <game/player.h>
#include <game/transferPlayer.h>
#include "game/transferRender.h"

#define filledBoxRGBA boxRGBA

int                         transferBorderThickness;
int                         numberTransferRows;
int                         transferBitmapWidth;
int                         transferBitmapHeight;
float                       transferRowStartY;
float                       transferRowHeight;
float                       activeTokenPosX;
float                       transferSidebarGap;
float                       transferSidebarWidth;
float                       transferLineThickness;
float                       transferStatusTabHeight;
float                       transferStatusTabWidth;
float                       transferBackgroundStartX;
float                       transferBackgroundStartY;
float                       transferBackgroundWidth;
float                       transferBackgroundHeight;
float                       transferRowCellWidth;
float                       gapWidth;

PARA_Color transferColorLeft;
PARA_Color transferColorRight;

PARA_Color transferColorStatusCell;
PARA_Color transferColorBorder;
PARA_Color transferColorLeftActive;
PARA_Color transferColorRightActive;
PARA_Color transferColorBackground;

//-------------------------------------------------------------------------------------------------------------------
//
// Return the row type by string
std::string trn_getRowName (int rowType)
//-------------------------------------------------------------------------------------------------------------------
{
	switch (rowType)
	{
		case TRANSFER_ROW_FULL_LINE:
			return "TRANSFER_ROW_FULL_LINE";
			break;

		case TRANSFER_ROW_HALF_LINE:
			return "TRANSFER_ROW_HALF_LINE";
			break;

		case TRANSFER_ROW_3_4_LINE:
			return "TRANSFER_ROW_3_4_LINE";
			break;

		case TRANSFER_ROW_FULL_LINE_1:
			return "TRANSFER_ROW_FULL_LINE_1";
			break;

		case TRANSFER_ROW_QUARTER_LINE:
			return "TRANSFER_ROW_QUARTER_LINE";
			break;

		case TRANSFER_ROW_REPEAT_HALF:
			return "TRANSFER_ROW_REPEAT_HALF";
			break;

		case TRANSFER_ROW_REPEAT_QUARTER:
			return "TRANSFER_ROW_REPEAT_QUARTER";
			break;

		case TRANSFER_ROW_FULL_LINE_2:
			return "TRANSFER_ROW_FULL_LINE_2";
			break;

		case TRANSFER_ROW_REVERSE_HALF:
			return "TRANSFER_ROW_REVERSE_HALF";
			break;

		case TRANSFER_ROW_REVERSE_QUARTER:
			return "TRANSFER_ROW_REVERSE_QUARTER";
			break;

		case TRANSFER_ROW_TWO_INTO_ONE_MIDDLE:
			return "TRANSFER_ROW_TWO_INTO_ONE_MIDDLE";
			break;

		case TRANSFER_ROW_ONE_INTO_TWO_MIDDLE:
			return "TRANSFER_ROW_ONE_INTO_TWO_MIDDLE";
			break;

		case TRANSFER_ROW_FULL_LINE_3:
			return "TRANSFER_ROW_FULL_LINE_3";
			break;

		case TRANSFER_ROW_ONE_INTO_TWO_TOP:
		case TRANSFER_ROW_ONE_INTO_TWO_BOTTOM:
		case TRANSFER_ROW_TWO_INTO_ONE_TOP:
		case TRANSFER_ROW_TWO_INTO_ONE_BOTTOM:
			return "Split row";
			break;

		default:
			return sys_getString ("Unknown Row Line [ %i ]", rowType);
			break;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//
// Render a transfer row bar
void trn_renderRowBar (float startX, float middleY, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
//-------------------------------------------------------------------------------------------------------------------
{
	middleY++;

	filledBoxRGBA (renderer.renderer, startX,
	               (middleY - transferRowHeight) - 1,
			startX + (BLOCK_WIDTH * 1.5),
			(middleY) + transferRowHeight + 4, red, green, blue, alpha);

	rectangleRGBA (renderer.renderer, startX,
	               (middleY - transferRowHeight) - 1,
	               startX + (BLOCK_WIDTH * 1.5) + 1,
	               (middleY - 1) + transferRowHeight + transferLineThickness + 1, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
}

//-------------------------------------------------------------------------------------------------------------------
//
// Render a token
void trn_renderToken (int tokenDirection, float startX, float startY, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
//-------------------------------------------------------------------------------------------------------------------
{

	if (tokenDirection == TOKEN_DIRECTION_RIGHT)
	{
		filledBoxRGBA (renderer.renderer, startX, startY, startX + BLOCK_WIDTH, startY + BLOCK_HEIGHT, red, green, blue, alpha);

		filledBoxRGBA (renderer.renderer, startX + BLOCK_WIDTH, startY + BLOCK_STEP, startX + (BLOCK_WIDTH * 2), startY + (BLOCK_HEIGHT - BLOCK_STEP), red, green, blue, alpha);

		rectangleRGBA (renderer.renderer, startX, startY, startX + BLOCK_WIDTH + 1, startY + BLOCK_HEIGHT + 1, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

		rectangleRGBA (renderer.renderer,
		               startX + BLOCK_WIDTH, startY + BLOCK_STEP, startX + (BLOCK_WIDTH * 2) + 1, startY + (BLOCK_HEIGHT - BLOCK_STEP) + 1, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
	}

	if (tokenDirection == TOKEN_DIRECTION_LEFT)
	{
		filledBoxRGBA (renderer.renderer, startX, startY, startX - BLOCK_WIDTH, startY + BLOCK_HEIGHT, red, green, blue, alpha);

		filledBoxRGBA (renderer.renderer, startX - BLOCK_WIDTH, startY + BLOCK_STEP, startX - (BLOCK_WIDTH * 2), startY + (BLOCK_HEIGHT - BLOCK_STEP), red, green, blue, alpha);

		// Big block
		rectangleRGBA (renderer.renderer, startX + 1, startY, (startX - BLOCK_WIDTH), (startY + BLOCK_HEIGHT) + 1, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

		// Small block
		rectangleRGBA (renderer.renderer, (startX - BLOCK_WIDTH), (startY + BLOCK_STEP), (startX - (BLOCK_WIDTH * 2)),
		               (startY + (BLOCK_HEIGHT - BLOCK_STEP)) + 1, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

	}
}

//-------------------------------------------------------------------------------------------------------------------
//
// Render the types of lines for each row
void trn_renderLineForRow (__TRANSFER_ROW whichRow, float lineLength, int special)
//-------------------------------------------------------------------------------------------------------------------
{
	float leftLineStartX  = 0.0f;
	float rightLineStartX = 0.0f;
	float lineStartY      = 0.0f;
	float tokenStartY     = 0.0f;
	float tokenOffsetY    = (transferRowHeight - BLOCK_HEIGHT) / 2;

	leftLineStartX  = transferSidebarGap + transferSidebarWidth;
	rightLineStartX = transferBitmapWidth - (transferSidebarGap + transferSidebarWidth);
	lineStartY      = whichRow.startY + ((transferRowHeight - transferLineThickness) / 2);
	tokenStartY     = whichRow.startY + tokenOffsetY;

	if (TRANSFER_ROW_LAUNCHER_LEFT == special)
	{
		lineStartY -= transferRowHeight;
		filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + (lineLength / 4), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
		trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (lineLength / 4), tokenStartY - transferRowHeight, 0, 0, 0, 255);
		return;
	}

	if (TRANSFER_ROW_LAUNCHER_RIGHT == special)
	{
		lineStartY -= transferRowHeight;
		filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY, rightLineStartX - (lineLength / 4), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
		trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (lineLength / 4), tokenStartY - transferRowHeight, 0, 0, 0, 255);
		return;
	}

	if (TRANSFER_ROW_LAUNCHER_RIGHT_COLOR == special)
	{
		lineStartY -= transferRowHeight;
		filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY, rightLineStartX - (lineLength / 4), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
		trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (lineLength / 4), tokenStartY - transferRowHeight, 0, 255, 255, 255);
		return;
	}

	if (TRANSFER_ROW_LAUNCHER_LEFT_COLOR == special)
	{
		lineStartY -= transferRowHeight;
		filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + (lineLength / 4), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
		trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (lineLength / 4), tokenStartY - transferRowHeight, 255, 255, 0, 255);
		return;
	}

	switch (whichRow.leftSideType)
	{
		case TRANSFER_ROW_FULL_LINE:
		case TRANSFER_ROW_FULL_LINE_1:
		case TRANSFER_ROW_FULL_LINE_2:
		case TRANSFER_ROW_FULL_LINE_3:
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.leftSideActive)
			{
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}
			break;

		case TRANSFER_ROW_3_4_LINE:
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + (lineLength * 0.75f), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.leftSideActive)
			{
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, leftLineStartX + (lineLength * 0.75f), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}
			trn_renderToken (TOKEN_DIRECTION_LEFT, leftLineStartX + (lineLength * 0.75f), tokenStartY, 255, 255, 0, 255);
			break;

		case TRANSFER_ROW_HALF_LINE:
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + (lineLength * 0.5f), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.leftSideActive)
			{
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, leftLineStartX + (lineLength * 0.5f), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}
			trn_renderToken (TOKEN_DIRECTION_LEFT, leftLineStartX + (lineLength * 0.5f), tokenStartY, 255, 255, 0, 255);
			break;

		case TRANSFER_ROW_QUARTER_LINE:
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.leftSideActive)
			{
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, leftLineStartX + (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}
			trn_renderToken (TOKEN_DIRECTION_LEFT, leftLineStartX + (lineLength * 0.25f), tokenStartY, 255, 255, 0, 255);
			break;


		case TRANSFER_ROW_REPEAT_HALF:
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.leftSideActive)
			{
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, leftLineStartX + (lineLength * 0.5f), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}
			if (whichRow.leftSideActiveIsOn)
			{
				filledBoxRGBA(renderer.renderer, leftLineStartX + (lineLength * 0.5f), lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
			}
			trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (lineLength * 0.5f), tokenStartY, 255, 255, 0, 255);
			break;

		case TRANSFER_ROW_REPEAT_QUARTER:
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.leftSideActive)
			{
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, leftLineStartX + (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}
			if (whichRow.leftSideActiveIsOn)
			{
				filledBoxRGBA(renderer.renderer, leftLineStartX + (lineLength * 0.25f), lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
			}
			trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (lineLength * 0.25f), tokenStartY, 255, 255, 0, 255);
			break;

		case TRANSFER_ROW_REVERSE_HALF:
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.leftSideActive)
			{
				// Yellow part
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, leftLineStartX + (lineLength / 2), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				// Blue part
				filledBoxRGBA (renderer.renderer, leftLineStartX + (lineLength / 2), lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}
			trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (lineLength / 2), tokenStartY, 0, 255, 255, 255);
			break;

		case TRANSFER_ROW_REVERSE_QUARTER:
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.leftSideActive)
			{
				// Yellow part
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, leftLineStartX + (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				// Blue part
				filledBoxRGBA (renderer.renderer, leftLineStartX + (lineLength * 0.25f), lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}
			trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (lineLength * 0.25f), tokenStartY, 0, 255, 255, 255);
			break;

		case TRANSFER_ROW_TWO_INTO_ONE_TOP:
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, (leftLineStartX + (lineLength / 2)) + (BLOCK_WIDTH * 1.5), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.leftSideActive)
			{
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, (leftLineStartX + (lineLength / 2)) + (BLOCK_WIDTH * 1.5), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}
			break;

		case TRANSFER_ROW_TWO_INTO_ONE_BOTTOM:
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, (leftLineStartX + (lineLength / 2)) + (BLOCK_WIDTH * 1.5), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.leftSideActive)
			{
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, (leftLineStartX + (lineLength / 2)) + (BLOCK_WIDTH * 1.5), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}
			trn_renderRowBar (leftLineStartX + (lineLength / 2), lineStartY - transferRowHeight, 255, 255, 0, 255);
			break;

		case TRANSFER_ROW_TWO_INTO_ONE_MIDDLE:
			//
			// Little circuit in the middle
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			filledBoxRGBA (renderer.renderer, leftLineStartX + (lineLength / 2), lineStartY, leftLineStartX + (lineLength), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

			if (whichRow.leftSideActive)
			{
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, leftLineStartX + (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}

			if ((transferRows[whichRow.index - 1].leftSideActive) && (transferRows[whichRow.index + 1].leftSideActive))
			{
				filledBoxRGBA (renderer.renderer, leftLineStartX + (lineLength / 2), lineStartY, leftLineStartX + (lineLength), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
			}
			trn_renderToken (TOKEN_DIRECTION_LEFT, leftLineStartX + (lineLength * 0.25f), tokenStartY, 255, 255, 0, 255);
			break;

		case TRANSFER_ROW_ONE_INTO_TWO_TOP:
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			filledBoxRGBA (renderer.renderer, leftLineStartX + (lineLength * 0.5f), lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.leftSideActive)
			{
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, leftLineStartX + (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}
			trn_renderToken (TOKEN_DIRECTION_LEFT, leftLineStartX + (lineLength * 0.25f), tokenStartY, 255, 255, 0, 255);
			break;

		case TRANSFER_ROW_ONE_INTO_TWO_BOTTOM:
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			filledBoxRGBA (renderer.renderer, leftLineStartX + (lineLength * 0.5f), lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.leftSideActive)
			{
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, leftLineStartX + (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}

			if (transferRows[whichRow.index - 1].leftSideActive)
			{
				filledBoxRGBA (renderer.renderer, leftLineStartX + (lineLength * 0.5f), lineStartY, leftLineStartX + lineLength, lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
			}
			trn_renderToken (TOKEN_DIRECTION_LEFT, leftLineStartX + (lineLength * 0.25f), tokenStartY, 255, 255, 0, 255);
			trn_renderRowBar (leftLineStartX + (lineLength * 0.5f), lineStartY - transferRowHeight, 255, 255, 0, 255);
			break;

		// from here down to go
		case TRANSFER_ROW_ONE_INTO_TWO_MIDDLE:
			//
			// Middle line
			filledBoxRGBA (renderer.renderer, leftLineStartX, lineStartY, leftLineStartX + (lineLength * 0.5f), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

			if (whichRow.leftSideActive)
			{
				//
				// Top Line
				filledBoxRGBA (renderer.renderer, leftLineStartX + (lineLength * 0.5f), lineStartY - transferRowHeight, leftLineStartX + lineLength, (lineStartY - transferRowHeight) + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				//
				// Bottom line
//				filledBoxRGBA (renderer.renderer, leftLineStartX + (lineLength * 0.5f), lineStartY + transferRowHeight, leftLineStartX + lineLength, (lineStartY + transferRowHeight) + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				//
				// Middle line
				filledBoxRGBA (renderer.renderer, leftLineStartX + (activeTokenPosX * 2), lineStartY, (leftLineStartX + (lineLength * 0.5f)) + (BLOCK_WIDTH * 1.5), lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));

				trn_renderToken (TOKEN_DIRECTION_RIGHT, leftLineStartX + (activeTokenPosX * 2), tokenStartY, 255, 255, 0, 255);
			}
			break;

		default:
//				printf ("Unknown left row [ %f ] value [ %s ]\n", whichRow.startY / transferRowHeight, trn_getRowName (whichRow.leftSideType).c_str());
			break;
	}

	switch (whichRow.rightSideType)
	{
		case TRANSFER_ROW_FULL_LINE:
		case TRANSFER_ROW_FULL_LINE_1:
		case TRANSFER_ROW_FULL_LINE_2:
		case TRANSFER_ROW_FULL_LINE_3:
			filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.rightSideActive)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 255);
			}
			break;

		case TRANSFER_ROW_3_4_LINE:
			filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY, rightLineStartX - (lineLength * 0.75f), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.rightSideActive)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - (lineLength * 0.75f), lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 255);
			}
			trn_renderToken (TOKEN_DIRECTION_RIGHT, rightLineStartX - (lineLength * 0.75f) - transferBorderThickness, tokenStartY, 0, 255, 255, 255);
			break;

		case TRANSFER_ROW_HALF_LINE:
			filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY, rightLineStartX - (lineLength * 0.5f), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

			if (whichRow.rightSideActive)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - (lineLength * 0.5f), lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 255);
			}
			trn_renderToken (TOKEN_DIRECTION_RIGHT, rightLineStartX - (lineLength * 0.5f) - transferBorderThickness, tokenStartY, 0, 255, 255, 255);
			break;

		case TRANSFER_ROW_QUARTER_LINE:
			filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY, rightLineStartX - (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

			if (whichRow.rightSideActive)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 0);
			}
			trn_renderToken (TOKEN_DIRECTION_RIGHT, rightLineStartX - (lineLength * 0.25f) - transferBorderThickness, tokenStartY, 0, 255, 255, 255);
			break;

		case TRANSFER_ROW_REPEAT_HALF:
			filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.rightSideActive)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 255);
			}
			//
			// Keep the power on for repeater
			if (whichRow.rightSideActiveIsOn)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (lineLength / 2) - transferBorderThickness - (BLOCK_WIDTH * 2), lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
			}
			trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (lineLength / 2) - transferBorderThickness, tokenStartY, 0, 255, 255, 255);
			break;

		case TRANSFER_ROW_REPEAT_QUARTER:
			filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.rightSideActive)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, whichRow.rightSideActiveAlphaColor, transferColorRightActive.g, transferColorRightActive.b, 255);
				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 255);
			}
			//
			// Keep the power on for repeater
			if (whichRow.rightSideActiveIsOn)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (lineLength * 0.25f) - transferBorderThickness - (BLOCK_WIDTH * 2), lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
			}
			trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (lineLength * 0.25f) - transferBorderThickness, tokenStartY, 255, 255, 0, 255);
			break;

		case TRANSFER_ROW_REVERSE_HALF:
			filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.rightSideActive)
			{
				// Yellow part
				filledBoxRGBA (renderer.renderer, rightLineStartX - (lineLength * 0.5f), lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));
				// Blue part
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - (lineLength * 0.5f), lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 255);
			}
			trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (lineLength * 0.5f), tokenStartY, 255, 255, 0, 255);
			break;

		case TRANSFER_ROW_REVERSE_QUARTER:
			filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.rightSideActive)
			{
				// Blue part
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - (activeTokenPosX * 2) - (lineLength * 0.75f), lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				// Yellow part
				filledBoxRGBA (renderer.renderer, rightLineStartX - (lineLength * 0.25f), lineStartY, rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorLeftActive.r, transferColorLeftActive.g, transferColorLeftActive.b, static_cast<Uint8>(whichRow.leftSideActiveAlphaColor));

				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 255);
			}
			trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (lineLength * 0.25f) - transferBorderThickness, tokenStartY, 255, 255, 0, 255);
			break;

		case TRANSFER_ROW_TWO_INTO_ONE_TOP:
			filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY, rightLineStartX - (lineLength / 2), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.rightSideActive)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - (lineLength * 0.5f), lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 255);
			}
			break;

		case TRANSFER_ROW_TWO_INTO_ONE_BOTTOM:
			filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY, rightLineStartX - (lineLength / 2), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.rightSideActive)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - (lineLength * 0.5f), lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 255);
			}
			trn_renderRowBar (rightLineStartX - (lineLength / 2), lineStartY - transferRowHeight, 0, 255, 255, 255);
			break;

		case TRANSFER_ROW_TWO_INTO_ONE_MIDDLE:
			//
			// Little circuit in the middle
			filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY, rightLineStartX - (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			// Main middle circuit
			filledBoxRGBA (renderer.renderer, rightLineStartX - (lineLength / 2), lineStartY, rightLineStartX - (lineLength), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

			if (whichRow.rightSideActive)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 255);
			}

			if ((transferRows[whichRow.index - 1].rightSideActive) && (transferRows[whichRow.index + 1].rightSideActive))
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (lineLength / 2), lineStartY, rightLineStartX - (lineLength), lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
			}
			trn_renderToken (TOKEN_DIRECTION_RIGHT, rightLineStartX - (lineLength * 0.25f), tokenStartY, 0, 255, 255, 255);
			break;

		case TRANSFER_ROW_ONE_INTO_TWO_TOP:
			// Little circuit
			filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY,  rightLineStartX - (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			// Main circuit
			filledBoxRGBA (renderer.renderer, rightLineStartX - (lineLength * 0.5f), lineStartY,  rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.rightSideActive)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 255);
			}
			trn_renderToken (TOKEN_DIRECTION_RIGHT, rightLineStartX - (lineLength * 0.25f), tokenStartY, 0, 255, 255, 255);
			break;

		case TRANSFER_ROW_ONE_INTO_TWO_BOTTOM:
			// Little circuit
			filledBoxRGBA (renderer.renderer, rightLineStartX, lineStartY,  rightLineStartX - (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			// Main circuit
			filledBoxRGBA (renderer.renderer, rightLineStartX - (lineLength * 0.5f), lineStartY,  rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
			if (whichRow.rightSideActive)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - (lineLength * 0.25f), lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 255);
			}

			if (transferRows[whichRow.index - 1].rightSideActive)
			{
				filledBoxRGBA (renderer.renderer, rightLineStartX - (lineLength * 0.5f), lineStartY,  rightLineStartX - lineLength, lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
			}
			trn_renderToken (TOKEN_DIRECTION_RIGHT, rightLineStartX - (lineLength * 0.25f), tokenStartY, 0, 255, 255, 255);
			trn_renderRowBar (rightLineStartX - (lineLength * 0.5f), lineStartY - transferRowHeight, 0, 255, 255, 255);
			break;

		case TRANSFER_ROW_ONE_INTO_TWO_MIDDLE:
			//
			// Middle black line
			filledBoxRGBA (renderer.renderer, rightLineStartX - (lineLength * 0.5f), lineStartY, rightLineStartX, lineStartY + transferLineThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

			if (whichRow.rightSideActive)
			{
				//
				// Top Line
				filledBoxRGBA (renderer.renderer, rightLineStartX - (lineLength * 0.5f), lineStartY - transferRowHeight,  rightLineStartX - lineLength, (lineStartY - transferRowHeight) + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				//
				// Bottom line
				filledBoxRGBA (renderer.renderer, rightLineStartX - (lineLength * 0.5f), lineStartY + transferRowHeight,  rightLineStartX - lineLength, (lineStartY + transferRowHeight) + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));
				//
				// Middle line
				filledBoxRGBA (renderer.renderer, rightLineStartX - (activeTokenPosX * 2), lineStartY, rightLineStartX - (lineLength * 0.5f), lineStartY + transferLineThickness, transferColorRightActive.r, transferColorRightActive.g, transferColorRightActive.b, static_cast<Uint8>(whichRow.rightSideActiveAlphaColor));

				trn_renderToken (TOKEN_DIRECTION_LEFT, rightLineStartX - (activeTokenPosX * 2), tokenStartY, 0, 255, 255, 255);
			}

			break;

		default:
//				printf("Unknown right row value [ %i ]\n", whichRow.rightSideType);
			break;
	}
}

//-------------------------------------------------------------------------------------------------------------------
//
// Render the current transfer game elements
void trn_renderTransferGame ()
//-------------------------------------------------------------------------------------------------------------------
{
	PARA_Color rowColor;
	float      droidX;
	float      droidY;
	float      droidDrawScale = 2.5f;
	float      tokenOffsetY   = (transferRowHeight - BLOCK_HEIGHT) / 2;

	if (transferRows.empty ())
		return;

	filledBoxRGBA (renderer.renderer, transferBackgroundStartX, transferBackgroundStartY,
	               transferBackgroundStartX + transferBackgroundWidth, transferBackgroundStartY + transferBackgroundHeight, transferColorBackground.r, transferColorBackground.g, transferColorBackground.b, transferColorBackground.a);

	for (auto transferIndex : transferRows)
	{
		trn_renderLineForRow (transferIndex, gapWidth * 2, 0);
		trn_renderToken (TOKEN_DIRECTION_RIGHT, transferIndex.startX + transferRowCellWidth - (BLOCK_WIDTH * 3), transferIndex.startY + tokenOffsetY, transferColorRight.r, transferColorRight.g, transferColorRight.b, transferColorRight.a);
		trn_renderToken (TOKEN_DIRECTION_LEFT, transferIndex.startX - (BLOCK_WIDTH * 3), transferIndex.startY + tokenOffsetY, transferColorLeft.r, transferColorLeft.g, transferColorLeft.b, transferColorLeft.a);

		if (transferIndex.currentColor == TRANSFER_COLOR_LEFT)
		{
			rowColor = transferColorLeft;
		}
		else
		{
			rowColor = transferColorRight;
		}

		filledBoxRGBA (renderer.renderer,
		               transferIndex.startX - (transferRowCellWidth / 2), transferIndex.startY, transferIndex.startX + (transferRowCellWidth / 2), transferIndex.startY + transferRowHeight, rowColor.r, rowColor.g, rowColor.b, rowColor.a);

		rectangleRGBA (renderer.renderer,
		               transferIndex.startX - (transferRowCellWidth / 2), transferIndex.startY,
		               (transferIndex.startX + (transferRowCellWidth / 2)) + 1, (transferIndex.startY + transferRowHeight) + 1, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
	}

	//
	// Render the status cell
	filledBoxRGBA (renderer.renderer,
	               transferRows[0].startX - (transferRowCellWidth / 2),
	               transferRows[0].startY - (transferRowHeight * 2),
	               transferRows[0].startX + (transferRowCellWidth / 2), transferRows[0].startY, transferColorStatusCell.r, transferColorStatusCell.g, transferColorStatusCell.b, transferColorStatusCell.a);

	rectangleRGBA (renderer.renderer,
	               transferRows[0].startX - (transferRowCellWidth / 2),
	               transferRows[0].startY - (transferRowHeight * 2), transferRows[0].startX + (transferRowCellWidth / 2) + 1, transferRows[0].startY + 1, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

	rectangleRGBA (renderer.renderer,
	               transferRows[0].startX - (transferRowCellWidth / 2) + transferStatusTabWidth,
	               transferRows[0].startY - (transferRowHeight * 2) - 2,
	               transferRows[0].startX - (transferRowCellWidth / 2) + transferStatusTabWidth + transferStatusTabWidth,
	               transferRows[0].startY - (transferRowHeight * 2), transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
	//
	// Draw tabs around status cell
	filledBoxRGBA (renderer.renderer,
	               transferRows[0].startX - (transferRowCellWidth / 2) - transferBorderThickness,
	               transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2),
	               transferRows[0].startX - (transferRowCellWidth / 2), transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2) +
	                                                                    transferStatusTabHeight, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

	filledBoxRGBA (renderer.renderer,
	               transferRows[0].startX - (transferRowCellWidth / 2) - transferBorderThickness,
	               transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2) + transferRowHeight,
	               transferRows[0].startX - (transferRowCellWidth / 2), transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2) + transferStatusTabHeight +
	                                                                    transferRowHeight, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

	filledBoxRGBA (renderer.renderer,
	               transferRows[0].startX - (transferRowCellWidth / 2) + transferRowCellWidth,
	               transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2),
	               transferRows[0].startX - (transferRowCellWidth / 2) + transferRowCellWidth + transferBorderThickness, transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2) +
	                                                                                                                     transferStatusTabHeight, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

	filledBoxRGBA (renderer.renderer,
	               transferRows[0].startX - (transferRowCellWidth / 2) + transferRowCellWidth,
	               transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2) + transferRowHeight,
	               transferRows[0].startX - (transferRowCellWidth / 2) + transferRowCellWidth + transferBorderThickness,
	               transferRows[0].startY - (transferRowHeight * 2) + ((transferRowHeight - transferStatusTabHeight) / 2) + transferStatusTabHeight +
	               transferRowHeight, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

	filledBoxRGBA (renderer.renderer,
	               transferRows[0].startX - (transferRowCellWidth / 2) + transferStatusTabWidth,
	               transferRows[transferRows.size () - 1].startY + (transferRowHeight),
	               transferRows[0].startX - (transferRowCellWidth / 2) + transferStatusTabWidth + transferStatusTabWidth,
	               transferRows[transferRows.size () - 1].startY + (transferRowHeight) + transferBorderThickness, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

	//
	// Draw left bar
	filledBoxRGBA (renderer.renderer, transferSidebarGap,
	               transferRows[0].startY - (transferRowHeight * 2),
	               transferSidebarGap + transferSidebarWidth, transferRows[transferRows.size () - 1].startY + transferRowHeight, transferColorLeft.r, transferColorLeft.g, transferColorLeft.b, transferColorLeft.a);

	rectangleRGBA (renderer.renderer, transferSidebarGap,
	               transferRows[0].startY - (transferRowHeight * 2),
	               (transferSidebarGap + transferSidebarWidth) + 1, (transferRows[transferRows.size () - 1].startY + transferRowHeight) + 1, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);
	//
	// Draw right bar
	filledBoxRGBA (renderer.renderer,
	               transferBitmapWidth - (transferSidebarGap + transferSidebarWidth),
	               transferRows[0].startY - (transferRowHeight * 2),
	               transferBitmapWidth - transferSidebarGap, transferRows[transferRows.size () - 1].startY + transferRowHeight, transferColorRight.r, transferColorRight.g, transferColorRight.b, transferColorRight.a);

	rectangleRGBA (renderer.renderer,
	               transferBitmapWidth - (transferSidebarGap + transferSidebarWidth),
	               transferRows[0].startY - (transferRowHeight * 2),
	               (transferBitmapWidth - transferSidebarGap) + 1, (transferRows[transferRows.size () - 1].startY + transferRowHeight) + 1, transferColorBorder.r, transferColorBorder.g, transferColorBorder.b, transferColorBorder.a);

	//
	// Draw the two sprite types
	//
	gapWidth = ((transferRows[0].startX - (transferSidebarGap + transferSidebarWidth)) / 2);

	if (transferPlayerWhichSide == TRANSFER_COLOR_LEFT)
	{
		droidX = ((transferSidebarGap + transferSidebarWidth) + gapWidth) - ((SPRITE_SIZE * droidDrawScale) / 2);
		droidY = transferRows[0].startY - (transferRowHeight * 3);

		if (numPlayerTokens > 0)
		{
			for (int i = 0; i != numPlayerTokens; i++)
			{
				trn_renderToken (TOKEN_DIRECTION_RIGHT, transferSidebarGap - (BLOCK_WIDTH * 3), transferRows[i].startY + tokenOffsetY, transferColorLeft.r, transferColorLeft.g, transferColorLeft.b, transferColorLeft.a);
			}
		}

		playerDroid.sprite.setTintColor (255, 255, 255);
		playerDroid.sprite.render (droidX, droidY, droidDrawScale, static_cast<Uint8>(255));

		droidX = ((transferBitmapWidth - (transferSidebarGap + transferSidebarWidth)) - gapWidth) - ((SPRITE_SIZE * droidDrawScale) / 2);

		if (numDroidTokens > 0)
		{
			for (int i = 0; i != numDroidTokens; i++)
			{
				trn_renderToken (TOKEN_DIRECTION_LEFT,
				                 transferBitmapWidth - (transferSidebarGap + transferSidebarWidth) + (BLOCK_WIDTH * 4),
				                 transferRows[i].startY + tokenOffsetY, transferColorRight.r, transferColorRight.g, transferColorRight.b, transferColorRight.a);
			}
		}

		g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].sprite.setTintColor (0, 0, 0);     // Draw in black color
		g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].sprite.render (droidX, droidY, droidDrawScale, static_cast<Uint8>(255));

		switch (droidBlockPos)
		{
			case -1:
				//
				// Draw right side launchpad
				trn_renderLineForRow (transferRows[0], gapWidth, TRANSFER_ROW_LAUNCHER_RIGHT_COLOR);
				break;

			case -2:
			default:
				//
				// Render current moving token
				trn_renderToken (TOKEN_DIRECTION_LEFT, transferBitmapWidth - (transferSidebarGap + transferSidebarWidth) - (BLOCK_WIDTH), transferRows[droidBlockPos].startY + tokenOffsetY, transferColorRight.r, transferColorRight.g, transferColorRight.b, transferColorRight.a);
				//
				// Draw right side launchpad
				trn_renderLineForRow (transferRows[0], gapWidth, TRANSFER_ROW_LAUNCHER_RIGHT);
				break;
		}

		switch (playerBlockPos)
		{
			case -1:
				trn_renderLineForRow (transferRows[0], gapWidth, TRANSFER_ROW_LAUNCHER_LEFT_COLOR);
				break;

			case -2:
			default:
				trn_renderToken (TOKEN_DIRECTION_RIGHT, transferSidebarGap + transferSidebarWidth + BLOCK_WIDTH, transferRows[playerBlockPos].startY + tokenOffsetY, transferColorLeft.r, transferColorLeft.g, transferColorLeft.b, transferColorLeft.a);
				trn_renderLineForRow (transferRows[0], gapWidth, TRANSFER_ROW_LAUNCHER_LEFT_COLOR);
				break;
		}
	}
	else
	{
		droidX = ((transferSidebarGap + transferSidebarWidth) + gapWidth) - ((SPRITE_SIZE * droidDrawScale) / 2);
		droidY = transferRows[0].startY - (transferRowHeight * 3);

		if (numDroidTokens > 0)
		{
			for (int i = 0; i != numDroidTokens; i++)
			{
				trn_renderToken (TOKEN_DIRECTION_RIGHT, transferSidebarGap - (BLOCK_WIDTH * 3), transferRows[i].startY + tokenOffsetY, transferColorLeft.r, transferColorLeft.g, transferColorLeft.b, transferColorLeft.a);
			}
		}

		g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].sprite.setTintColor (0, 0, 0);     // Draw in black color
		g_shipDeckItr->second.droid[playerDroid.transferTargetDroidIndex].sprite.render (droidX, droidY, droidDrawScale, static_cast<Uint8>(255));

		droidX = ((transferBitmapWidth - (transferSidebarGap + transferSidebarWidth)) - gapWidth) - ((SPRITE_SIZE * droidDrawScale) / 2);

		playerDroid.sprite.setTintColor (255, 255, 255);
		playerDroid.sprite.render (droidX, droidY, droidDrawScale, static_cast<Uint8>(255));

		if (numPlayerTokens > 0)
		{
			for (int i = 0; i != numPlayerTokens; i++)
			{
				trn_renderToken (TOKEN_DIRECTION_LEFT,
				                 transferBitmapWidth - (transferSidebarGap + transferSidebarWidth) + (BLOCK_WIDTH * 4),
				                 transferRows[i].startY + tokenOffsetY, transferColorRight.r, transferColorRight.g, transferColorRight.b, transferColorRight.a);
			}
		}

		switch (playerBlockPos)
		{
			case -1:
				//
				// Draw right side launchpad
				trn_renderLineForRow (transferRows[0], gapWidth, TRANSFER_ROW_LAUNCHER_RIGHT_COLOR);
				break;

			case -2:
			default:
				//
				// Render current moving token
				trn_renderToken (TOKEN_DIRECTION_LEFT, transferBitmapWidth - (transferSidebarGap + transferSidebarWidth) - (BLOCK_WIDTH), transferRows[playerBlockPos].startY + tokenOffsetY, transferColorRight.r, transferColorRight.g, transferColorRight.b, transferColorRight.a);
				//
				// Draw right side launchpad
				trn_renderLineForRow (transferRows[0], gapWidth, TRANSFER_ROW_LAUNCHER_RIGHT);
				break;
		}

		switch (droidBlockPos)
		{
			case -1:
				trn_renderLineForRow (transferRows[0], gapWidth, TRANSFER_ROW_LAUNCHER_LEFT_COLOR);
				break;

			case -2:
			default:
				trn_renderToken (TOKEN_DIRECTION_RIGHT, transferSidebarGap + transferSidebarWidth + BLOCK_WIDTH, transferRows[droidBlockPos].startY + tokenOffsetY, transferColorLeft.r, transferColorLeft.g, transferColorLeft.b, transferColorLeft.a);
				trn_renderLineForRow (transferRows[0], gapWidth, TRANSFER_ROW_LAUNCHER_LEFT_COLOR);
				break;
		}
	}
}

//-------------------------------------------------------------------------------------------------------------------
//
// Set a color for the transfer game
void trn_setTransferColor (int whichSide, float red, float green, float blue, float alpha)
//-------------------------------------------------------------------------------------------------------------------
{
	switch (whichSide)
	{
		case TRANSFER_COLOR_LEFT:
			transferColorLeft.r = red;
			transferColorLeft.g = green;
			transferColorLeft.b = blue;
			transferColorLeft.a = alpha;
			break;

		case TRANSFER_COLOR_RIGHT:
			transferColorRight.r = red;
			transferColorRight.g = green;
			transferColorRight.b = blue;
			transferColorRight.a = alpha;
			break;

		case TRANSFER_COLOR_BACKGROUND:
			transferColorBackground.r = red;
			transferColorBackground.g = green;
			transferColorBackground.b = blue;
			transferColorBackground.a = alpha;
			break;

		case TRANSFER_COLOR_BORDER:
			transferColorBorder.r = red;
			transferColorBorder.g = green;
			transferColorBorder.b = blue;
			transferColorBorder.a = alpha;
			break;
	}
}