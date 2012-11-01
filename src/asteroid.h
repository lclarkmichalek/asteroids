#ifndef _ASTEROID_H
#define _ASTEROID_H

#include <stdbool.h>

#include "./vector.h"

#define VERTEXN 12
#define ASTEROID_ROTATION_SPEED 0.02
#define ASTEROID_COLOR al_map_rgb(255, 255, 255)
#define ASTEROID_INVINCIBLE 30
#define ASTEROID_SPEED 1
#define MAX_GENERATION 3
#define ASTEROID_SIZE_DECAY 0.7

typedef struct Asteroid {
    // The position vectors of the verticies, relative to the center
    Vector verticies[VERTEXN];

    // A point in the Asteroid
    Vector center;
    // The largest distance from the center to a vertex
    uint radius_squared;

    // The direction that the Asteroid is travelling per frame
    Vector direction;
    // The angle of the Asteroid in radians
    float angle;

    // How many frames the asteroid is invincible for
    char invincible;

    // The "generation" of asteroid this is
    char generation;
} Asteroid;

// Creates a new Asteroid at the given position, with the given verticies
// heading in the given direction
Asteroid new_asteroid(Vector pos, Vector[VERTEXN], Vector);
// Frees the memory used by the Asteroid.
void delete_asteroid(Asteroid*);
// Does collision detection on an asteroid
bool point_in_asteroid(Vector, Asteroid*);

struct AsteroidNode {
    struct AsteroidNode *next, *prev;
    Asteroid* value;
};
typedef struct AsteroidNode AsteroidNode;

// Frees the memory used by the AsteroidNode. Also calls delete_asteroid on the linked asteroid
void delete_asteroid_node(AsteroidNode*);

// Calls delete_asteroid_node on each element of the Asteroid list
void delete_asteroid_list(AsteroidNode*);
// Returns an AsteroidNode if the point is inside any asteroid in the
// game. Returns NULL if there is no collision
AsteroidNode* point_collides(AsteroidNode*, Vector);
// Updates the positions of the asteroids for one frame
void update_asteroids(AsteroidNode*, Vector);
// Draws the asteroids on the screen
void draw_asteroids(AsteroidNode*);

#endif
