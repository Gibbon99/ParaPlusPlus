#include "game/transferRender.h"

int                         numberTransferRows;
int                         transferBitmapWidth;
int                         transferBitmapHeight;
int                         transferPlayerWhichSide;
float                       transferRowStartY;
float                       transferRowHeight;
float                       activeTokenPosX;
std::vector<__TRANSFER_ROW> transferRows;

PARA_Color transferColorStatusCell;
PARA_Color transferColorBorder;
PARA_Color transferColorLeftActive;
PARA_Color transferColorRightActive;
