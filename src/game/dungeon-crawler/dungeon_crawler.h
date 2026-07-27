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
void player_input();