#pragma once

#include <libgs.h>

#include "types.h"

struct {
    vector3       position;
    vector3_short rotation;
    GsCOORDINATE2 coordinates;
} Camera; //TODO : change this to a variable instead of struct instance (I did'nt event know this was possible...)

void calculate_camera();

void camera_set_position_vector(vector3 position);
void camera_set_rotation_vector(vector3 rotation);