#pragma once

#include "types.h"

void initialize_screen();
void set_screen_mode(int mode);
void clear_vram();
void initialize_oredering_table();

void initialize_debug_font();
void line_register(Line* line);

void render_update();

void clear_display();
void draw();
void display();