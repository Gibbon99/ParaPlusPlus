#pragma once

#include "main.h"

#define BLOCK_HEIGHT             8
#define BLOCK_WIDTH              4
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
extern float                       transferRowHeight;
extern float                       transferRowStartY;
extern float                       activeTokenPosX;
extern std::vector<__TRANSFER_ROW> transferRows;

extern PARA_Color transferColorStatusCell;
extern PARA_Color transferColorBorder;
extern PARA_Color transferColorLeftActive;
extern PARA_Color transferColorRightActive;
