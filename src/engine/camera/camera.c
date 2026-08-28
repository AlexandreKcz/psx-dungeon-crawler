#include "../camera.h"

/// Mostly based on MBDesign-s work here : <https://mbdesigns.itch.io/ps1-homebrew-source-code-rendering-3d-graphics-with-psyq>

void calculate_camera(){
    VECTOR vec;
    static GsVIEW2 view;

    view.view = Camera.coordinates.coord;
    view.super = WORLD;

    RotMatrix(&Camera.rotation, &view.view);
    ApplyMatrixLV(&view.view, &Camera.position, &vec);
    TransMatrix(&view.view, &vec);
    GsSetView2(&view);
}

void camera_set_position_vector(vector3 position){
    Camera.position = position;

    calculate_camera();
}

void camera_set_rotation_vector(vector3 rotation){
    vector3_short converted_rotation = vector3_convert_to_vector3_short(&rotation);
    Camera.rotation = converted_rotation;

    calculate_camera();
}