#pragma once

/// Main Engine Start, contains : 
///     - System Memory Init
///     - Render Screen Init
///     - Font Init
///     - Input Pad Init
///     - Load CD Data
void initialize();

/// Main Game Start, contains just Game Start and version display
void start();

/// Main Engine loop, contains : 
///     - Pad Update
///     - Game Update
///     - Render Update
void update();