#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "./asteroid.h"
#include "./vector.h"

Asteroid new_asteroid(Vector pos, Vector verts[VERTEXN], Vector dir) {
    Asteroid a;
    a.center = vec_add(pos, verts[VERTEXN/2]);
    memcpy(a.verticies, verts, sizeof(Vector[VERTEXN]));

    Vector *vert;
    uint largest_mg2 = 0;
    for(vert = a.verticies; (uint)(vert - a.verticies) < VERTEXN; vert++) {
        uint mag = magnitude_squared(vec_sub(*vert, a.center));
        if (mag > largest_mg2) {
            largest_mg2 = mag;
        }
    }
    a.radius_squared = largest_mg2;

    a.direction = dir;
    a.angle = 0;
    return a;
}

void delete_asteroid(Asteroid* a) {
    free(a);
}

// Returns true if point1 and point2 are both on the same side of the line a b
bool same_side(Vector point1, Vector point2, Vector a, Vector b) {
    return dot_product(
               cross_product(vec_sub(b, a), vec_sub(point1, a)),
               cross_product(vec_sub(b, a), vec_sub(point2, a)));
}

// Returns true if the point is inside the triangel a b c
bool in_triangle(Vector point, Vector a, Vector b, Vector c) {
    return same_side(point, a, b, c) && same_side(point, b, a, c) &&
           same_side(point, c, a, b);
}

bool point_in_asteroid(Vector p, Asteroid* asteroid) {
    if (magnitude_squared(vec_sub(p, asteroid->center)) >
            asteroid->radius_squared) {
        return false;
    }

    Vector *a, *b;
    b = asteroid->verticies + VERTEXN - 1; // last element
    a = asteroid->verticies;
    while(a - asteroid->verticies < VERTEXN) {
        if (in_triangle(p, *a, *b, asteroid->center)) {
            return true;
        }
        b = a;
        a++;
    }
    return false;
}

void delete_asteroid_node(AsteroidNode* n) {
    delete_asteroid(n->value);
    if (n->prev != NULL) {
        n->prev->next = NULL;
    }
    if (n->next != NULL) {
        n->next->prev = NULL;
    }
    free(n);
}

void delete_asteroid_list(AsteroidNode* n) {
    for(; n != NULL; n = n->next) {
        delete_asteroid_node(n);
    }
}

void preappend_asteroid_list(AsteroidNode* al, AsteroidNode* n) {
    n->next = al;
    n->prev = NULL;
    al->prev = n;
}

AsteroidNode* point_collides(AsteroidNode* asteroids, Vector point) {
    AsteroidNode *node = asteroids;
    for(; node != NULL; node = node->next) {
        if (point_in_asteroid(point, node->value)) {
            return node;
        }
    }
    return NULL;
}

void update_asteroids(AsteroidNode* asteroids) {
    AsteroidNode *node;
    for(node = asteroids;
        node != NULL;
        node = node->next) {
        node->value->center = vec_add(node->value->center,
                                      node->value->direction);
        node->value->angle += ASTEROID_ROTATION_SPEED;
    }
}

void split_asteroid(AsteroidNode* asteroid) {
    AsteroidNode *child1, *child2;
    child1 = malloc(sizeof(AsteroidNode));
    child1->value = malloc(sizeof(Asteroid));
    child2 = malloc(sizeof(AsteroidNode));
    child2->value = malloc(sizeof(Asteroid));

    Vector* vertex;
    for(vertex = asteroid->value->verticies;
        vertex - asteroid->value->verticies < VERTEXN;
        vertex++) {
        *vertex = vec_mul(*vertex, 0.5);
    }

    Vector direction1, direction2;
    // D1 = D0 + (0, 1)
    direction1 = vec_mul(asteroid->value->direction, 1.5);
    direction2 = vec_mul(asteroid->value->direction, 0.5);

    *child1->value = new_asteroid(asteroid->value->center,
                                  asteroid->value->verticies,
                                  direction1);
    *child2->value = new_asteroid(asteroid->value->center,
                                  asteroid->value->verticies,
                                  direction2);

    child1->prev = asteroid->prev;
    child1->next = child2;
    child2->prev = child1;
    child2->next = asteroid->next;
    asteroid->prev->next = child1;
    asteroid->next->prev = child2;
    free(asteroid);
}
