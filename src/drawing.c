#include <allegro5/allegro.h>

#include "./vector.h"

#include "./drawing.h"

void rotate_around(ALLEGRO_TRANSFORM *trans, Vector p, float angle) {
    al_identity_transform(trans);
    al_rotate_transform(trans, angle);
    al_translate_transform(trans, p.x, p.y);
}
