#include "./ship.h"

Ship new_ship(Vector p, Vector v) {
    Ship s;
    s.position = p;
    s.velocity = v;
    s.angle = 0;
    s.invincible = SHIP_INVINCIBLE;
    return s;
}
