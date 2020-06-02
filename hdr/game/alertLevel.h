#pragma once

#include "main.h"

// Locate all the alert level tiles and store their location
void gam_locateAlertTiles ();

// Render the alert level tiles according to the current alert level
void gam_renderAlertTiles ();

// Get the current alert level
int gam_getCurrentAlertLevel();

// Set the new alert level
void gam_setAlertLevel(int newAlertLevel);