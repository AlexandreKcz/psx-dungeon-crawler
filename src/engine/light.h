#pragma once

#include "./types.h"

void set_sun_direction_vector(vector3 direction);
void set_sun_direction(int x, int y, int z);
void set_sun_color(Color* color);
void set_sun(int r, int g, int b);

void _update_sun();

void set_ambiant_light_color(Color* color);
void set_ambiant_light(int r, int g, int b);

void _update_ambiant_light();