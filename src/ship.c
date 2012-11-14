#include <math.h>

#include "./ship.h"

Ship new_ship(Vector p, Vector v) {
    Ship s;
    s.position = p;
    s.velocity = v;
    s.angle = 0;
    s.invincible = SHIP_INVINCIBLE;
    return s;
}

void rotate_ship_left(Ship *ship) {
    ship->angle -= SHIP_ROTATION_SPEED;
}

void rotate_ship_right(Ship *ship) {
    ship->angle += SHIP_ROTATION_SPEED;
}

void bound_ship_speeds(Ship *ship) {
    Vector *v = &ship->velocity;
    if (v->x < -MAX_SPEED)
        v->x = -MAX_SPEED;
    if (v->x > MAX_SPEED)
        v->x = MAX_SPEED;
    if (v->y < -MAX_SPEED)
        v->y = -MAX_SPEED;
    if (v->y > MAX_SPEED)
        v->y = MAX_SPEED;
}

void accelerate_ship(Ship *ship) {
    Vector delta;
    delta.x = (sin(-ship->angle) * ACCEL_CONST);
    delta.y = (cos(-ship->angle) * ACCEL_CONST);
    ship->velocity = vec_sub(ship->velocity, delta);
    bound_ship_speeds(ship);
}

void deccelerate_ship(Ship *ship) {
    Vector delta;
    delta.x = (sin(-ship->angle) * ACCEL_CONST);
    delta.y = (cos(-ship->angle) * ACCEL_CONST);
    ship->velocity = vec_add(ship->velocity, delta);
    bound_ship_speeds(ship);
}
