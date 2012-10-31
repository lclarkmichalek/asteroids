#ifndef _ASTEROID_H
#define _ASTEROID_H

#include <stdbool.h>

#include "./vector.h"

#define VERTEXN 12
#define ASTEROID_ROTATION_SPEED 0.05

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
} Asteroid;

// Creates a new Asteroid at the given position, with the given verticies
// heading in the given direction
Asteroid new_asteroid(Vector pos, Vector[VERTEXN], Vector);
// Frees the memory used by the Asteroid.
void delete_asteroid(Asteroid*);
// Does collision detection on an asteroid
bool point_in_asteroid(Vector, Asteroid*);

struct AsteroidNode {
    struct AsteroidNode* prev;
    struct AsteroidNode* next;
    Asteroid* value;
};
typedef struct AsteroidNode AsteroidNode;

// Frees the memory used by the AsteroidNode. Also calls delete_asteroid on the linked asteroid
void delete_asteroid_node(AsteroidNode*);

// Calls delete_asteroid_node on each element of the Asteroid list
void delete_asteroid_list(AsteroidNode*);
// Adds an asteroid to the front of the list
void prepend_asteroid_list(AsteroidNode*, AsteroidNode*);
// Returns an AsteroidNode if the point is inside any asteroid in the
// game. Returns NULL if there is no collision
AsteroidNode* point_collides(AsteroidNode*, Vector);
// Updates the positions of the asteroids for one frame
void update_asteroids(AsteroidNode*);
// Splits an asteroid in two, removes it from the list, and adds the
// new ones to the list
void split_asteroid(AsteroidNode*);

#endif
