#pragma once

#include "main.h"

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

struct __TRANSFER_ROW
{
	bool  rightSideActive;
	bool  leftSideActive;
	bool  rightSideActiveIsOn;
	bool  leftSideActiveIsOn;
	int   leftSideType;
	int   rightSideType;
	int   currentColor;
	float leftSideActiveCounter;
	float rightSideActiveCounter;
	float startX;
	float startY;
	float rightSideActiveAlphaCount;
	float rightSideActiveAlphaColor;
	float leftSideActiveAlphaCount;
	float leftSideActiveAlphaColor;
};

extern int                         numberTransferRows;
extern int                         transferBitmapWidth;
extern int                         transferBitmapHeight;
extern int                         transferPlayerWhichSide;
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