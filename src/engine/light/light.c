#include "../light.h"

#include <libgs.h>

GsF_LIGHT sun_light;
vector3 sun_direction_vector;
Color* sun_color;
Color* ambiant_light_color;

void set_sun_direction_vector(vector3 direction){
    sun_direction_vector = direction;

    _update_sun();
}

void set_sun_direction(int x, int y, int z){
    vector3 direction = { .vx = x, .vy = y, .vz = z };
    set_sun_direction_vector(direction);

    _update_sun();
}

void set_sun_color(Color* color){
    if(sun_color)
        color_free(sun_color);

    sun_color = color;

    _update_sun();
}

void set_sun(int r, int g, int b){
    if(sun_color)
        color_free(sun_color);

    color_create(r, g, b, &sun_color);

    _update_sun();
}

void _update_sun(){
    sun_light.vx = sun_direction_vector.vx;
    sun_light.vy = sun_direction_vector.vy; //should I multiply by -1 ?
    sun_light.vz = sun_direction_vector.vz;

    sun_light.r = sun_color->r;
    sun_light.g = sun_color->g;
    sun_light.b = sun_color->b;

    GsSetFlatLight(0, &sun_light);
}

void set_ambiant_light_color(Color* color){
    if(ambiant_light_color)
        color_free(ambiant_light_color);

    ambiant_light_color = color;

    _update_ambiant_light();
}

void set_ambiant_light(int r, int g, int b){
    if(ambiant_light_color)
        color_free(ambiant_light_color);

    color_create(r, g, b, &ambiant_light_color);

    _update_ambiant_light();
}

void _update_ambiant_light(){
        GsSetAmbient(
        ambiant_light_color->r * (ONE/255),
        ambiant_light_color->g * (ONE/255),
        ambiant_light_color->b * (ONE/255)
    );
}