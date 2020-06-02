#pragma once

#include "main.h"

extern double hudTextPosX;
extern double hudTextPosY;
extern double hudScorePosX;
extern double hudScorePosY;

// Render the HUD to the screen
void gam_renderHud();

void gam_setHudText(const std::string &newText);
