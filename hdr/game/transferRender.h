#pragma once

#include "main.h"
#include "transferGame.h"

enum TOKEN_DIRECTION
{
	TOKEN_DIRECTION_RIGHT = 0,
	TOKEN_DIRECTION_LEFT,
	TRANSFER_MOVE_DOWN,
	TRANSFER_MOVE_UP
};

#define BLOCK_HEIGHT             12
#define BLOCK_WIDTH              8
#define BLOCK_STEP               2

extern int                         numberTransferRows;
extern int                         transferBitmapWidth;
extern int                         transferBitmapHeight;
extern int                         transferBorderThickness;
extern float                       transferSidebarGap;
extern float                       transferSidebarWidth;
extern float                       transferLineThickness;
extern float                       transferStatusTabHeight;
extern float                       transferStatusTabWidth;
extern float                       transferBackgroundStartX;
extern float                       transferBackgroundStartY;
extern float                       transferBackgroundWidth;
extern float                       transferBackgroundHeight;
extern float                       transferRowCellWidth;
extern float                       gapWidth;
extern float                       transferRowHeight;
extern float                       transferRowStartY;
extern float                       activeTokenPosX;
extern std::vector<__TRANSFER_ROW> transferRows;

extern PARA_Color transferColorLeft;
extern PARA_Color transferColorRight;
extern PARA_Color transferColorStatusCell;
extern PARA_Color transferColorBorder;
extern PARA_Color transferColorLeftActive;
extern PARA_Color transferColorRightActive;

// Render the current transfer game elements
void trn_renderTransferGame ();

// Set a color for the transfer game
void trn_setTransferColor (int whichSide, float red, float green, float blue, float alpha);

// Return the row type by string
std::string trn_getRowName (int rowType);