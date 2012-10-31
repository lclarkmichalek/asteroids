#ifndef _ASTEROID_H
#define _ASTEROID_H

#include "./vector.h"

#define VERTEXN 12

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

typedef struct AsteroidList {
    AsteroidNode* head;
    AsteroidNode*tail;
} AsteroidList;

// Creates a new empty AsteroidList
AsteroidList new_asteroid_list();
// Calls delete_asteroid_node on each element of the AsteroidList
void delete_asteroid_list(AsteroidList);
// Adds an asteroid to the beginning of the list
void append_asteroid_list(AsteroidList*, AsteroidNode*);
// Adds an asteroid to the front of the list
void prepend_asteroid_list(AsteroidList*, AsteroidNode*);

#endif
