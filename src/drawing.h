#ifndef _DRAWING_H
#define _DRAWING_H

#include <allegro5/allegro.h>

#include "./vector.h"

// Creates a transformation that rotates around a given point by
// the given angle
void rotate_around(ALLEGRO_TRANSFORM*, Vector, float);

#endif
