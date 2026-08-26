#pragma once

#include "types.h"

void _initialize_screen();
void set_screen_mode(int mode);
void _clear_vram();
void _initialize_ordering_table();

void initialize_debug_font();
void line_register(Line* line);

void _render_update();

void clear_display();
void _draw();
void _display();

void _vsync_callback_fps();

int get_vsync_time();
int get_fps();