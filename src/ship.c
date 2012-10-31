#include "./ship.h"

Ship new_ship(Vector p, Vector v) {
    Ship s;
    s.position = p;
    s.velocity = v;
    s.angle = 0;
    return s;
}
