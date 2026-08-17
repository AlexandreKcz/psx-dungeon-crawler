#include "../../engine/types.h"

typedef struct player{
    vector2 position;
    unsigned short direction;
    Line* visual;
} Player;

void create_player();
void draw_dungeon();
void draw_player();
Player* get_player();
vector2 get_direction_vector(int dir);
vector2 player_direction_vector();
void set_player_lookat_matrix(short matrix[3][7][4]);
void display_matrix(short matrix[3][7][4]);
void player_input();
void load_dungeon_sprites();

void rotate_matrix(short n, short matrix[7][7][4]);
void rotate_array(short array[], int n, int d);