#pragma once

// TODO : Remove those checklists (but save them somewhere)

/// Main Engine Start, contains : 
///     - System Memory Init
///     - Render Screen Init
///     - Font Init
///     - Input Pad Init
///     - Load CD Data
void _initialize();

/// Main Game Start, contains just Game Start and version display
void _start();

/// Main Engine loop, contains : 
///     - Pad Update
///     - Game Update
///     - Render Update
void _update();