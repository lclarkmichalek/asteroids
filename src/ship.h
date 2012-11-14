#ifndef _SHIP_H
#define _SHIP_H

#include "./vector.h"

#define SHIP_INVINCIBLE 1.5
#define MAX_SPEED 300
#define ACCEL_CONST 10
#define SHIP_ROTATION_SPEED 0.1

typedef struct Ship {
    // The position of the ship
    Vector position;
    // The velocity of the ship
    Vector velocity;
    // The angle the ship is pointing in radians
    float angle;
    // How long is the ship invincible for
    float invincible;
} Ship;

// Creates a new ship with the given position and velocity
Ship new_ship(Vector, Vector);
// Rotates the ship left
void rotate_ship_left(Ship*);
// Rotates the ship right
void rotate_ship_right(Ship*);
// Accelerates the ship
void accelerate_ship(Ship*);
// Decelerates the ship
void deccelerate_ship(Ship*);

#endif
