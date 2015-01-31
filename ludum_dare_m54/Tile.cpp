#include "Tile.h"


Tile::Tile(void) : src(0.0, 0.0), pos(0.0,0.0)
{
}

Tile::Tile(int srx, int sry, int posx, int posy) : src((float)srx, (float)sry), pos((float)posx, (float)posy)
{

}

Tile::~Tile(void)
{
}
