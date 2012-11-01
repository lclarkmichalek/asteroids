#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "./asteroid.h"
#include "./vector.h"
#include "./drawing.h"

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
    a.invincible = ASTEROID_INVINCIBLE;
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
    free(n);
}

void delete_asteroid_list(AsteroidNode* n) {
    for(; n != NULL; n = n->next) {
        delete_asteroid_node(n);
    }
}

AsteroidNode* point_collides(AsteroidNode* asteroids, Vector point) {
    AsteroidNode *node = asteroids;
    for(; node != NULL; node = node->next) {
        if (!node->value->invincible && point_in_asteroid(point, node->value)) {
            return node;
        }
    }
    return NULL;
}

void update_asteroids(AsteroidNode* asteroids, Vector size) {
    AsteroidNode *node;
    for(node = asteroids;
        node != NULL;
        node = node->next) {
        if (node->value->invincible)
            node->value->invincible--;
        node->value->center = wrap(size,
                                   vec_add(node->value->center,
                                           node->value->direction));
        node->value->angle += ASTEROID_ROTATION_SPEED;
    }
}

void bound_asteroid_speeds(Vector *v) {
    if (v->x < -ASTEROID_SPEED)
        v->x = -ASTEROID_SPEED;
    if (v->x > ASTEROID_SPEED)
        v->x = ASTEROID_SPEED;
    if (v->y < -ASTEROID_SPEED)
        v->y = -ASTEROID_SPEED;
    if (v->y > ASTEROID_SPEED)
        v->y = ASTEROID_SPEED;
}

void split_asteroid(AsteroidNode* asteroid) {
    AsteroidNode *child;
    child = malloc(sizeof(AsteroidNode));
    child->value = malloc(sizeof(Asteroid));

    Vector* vertex;
    for(vertex = asteroid->value->verticies;
        vertex - asteroid->value->verticies < VERTEXN;
        vertex++) {
        *vertex = vec_mul(*vertex, 0.5);
    }

    Vector direction1, direction2;
    direction1 = rotate(asteroid->value->direction, 3.0/4.0 * 3.142);
    direction2 = rotate(asteroid->value->direction, 5.0/4.0 * 3.142);
    bound_asteroid_speeds(&direction1);
    bound_asteroid_speeds(&direction2);

    *asteroid->value = new_asteroid(asteroid->value->center,
                                    asteroid->value->verticies,
                                    direction1);
    *child->value = new_asteroid(asteroid->value->center,
                                 asteroid->value->verticies,
                                 direction2);

    child->next = asteroid->next;
    asteroid->next = child;
}

void draw_asteroids(AsteroidNode* n) {
    ALLEGRO_TRANSFORM trans;
    for(; n != NULL; n = n->next) {
        rotate_around(&trans, n->value->center, n->value->angle);
        al_use_transform(&trans);

        Vector *a, *b;
        b = &n->value->verticies[VERTEXN - 1];
        for(a = n->value->verticies;
                a - n->value->verticies < VERTEXN;
                b = a, a++) {
            al_draw_line(a->x, a->y, b->x, b->y, ASTEROID_COLOR, 1);
        }
    }
}
