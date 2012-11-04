#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>

#include "./asteroid.h"
#include "./vector.h"
#include "./drawing.h"

Asteroid new_asteroid(Vector pos, Vector verts[VERTEXN], Vector dir) {
    Asteroid a;
    memcpy(a.verticies, verts, sizeof(Vector[VERTEXN]));
    a.center = vec_add(pos, verts[VERTEXN/2]);

    Vector *vert;
    int largest_mg2 = -1;
    for(vert = a.verticies; vert - a.verticies < VERTEXN; vert++) {
        int mag = magnitude_squared(*vert);
        if (mag > largest_mg2) {
            largest_mg2 = mag;
        }
    }
    a.radius_squared = largest_mg2;

    a.direction = dir;
    a.angle = 0;
    a.invincible = ASTEROID_INVINCIBLE;
    a.generation = MAX_GENERATION;
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
            asteroid->radius_squared)
        return false;

    Vector *a, *b;
    b = asteroid->verticies + VERTEXN - 1; // last element
    a = asteroid->verticies;
    while(a - asteroid->verticies < VERTEXN) {
        if (in_triangle(p, *a, *b, vec_sub(p, asteroid->center))) {
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

bool is_list_consistent(AsteroidNode* asteroids) {
    AsteroidNode *prev, *current;
    prev = NULL;
    for(current = asteroids;
            current != NULL;
            prev = current, current = current->next) {
        if (current->prev != prev)
            return false;
    }
    return true;
}

void delete_asteroid_list(AsteroidNode* n) {
    for(; n != NULL; n = n->next) {
        delete_asteroid_node(n);
    }
}

void remove_from_asteroid_list(AsteroidNode **list, AsteroidNode *node) {
    if (*list == node)
        *list = node->next;
    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;
    free(node->value);
    free(node);
}

void insert_in_asteroid_list(AsteroidNode **list, AsteroidNode *node) {
    AsteroidNode *fst = *list;
    if (fst)
        fst->prev = node;
    node->prev = NULL;
    node->next = fst;
    *list = node;
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
            al_draw_line(a->x, a->y, b->x, b->y, ASTEROID_COLOR, 2);
        }
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

void split_asteroid(AsteroidNode **list, AsteroidNode *asteroid) {
    char oldgen = asteroid->value->generation;
    if (oldgen - 1 < 0) {
        remove_from_asteroid_list(list, asteroid);
        return;
    }

    AsteroidNode *child;
    child = malloc(sizeof(AsteroidNode));
    child->value = malloc(sizeof(Asteroid));

    Vector* vertex;
    for(vertex = asteroid->value->verticies;
            vertex - asteroid->value->verticies < VERTEXN;
            vertex++) {
        *vertex = vec_mul(*vertex, ASTEROID_SIZE_DECAY);
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
    asteroid->value->generation = oldgen - 1;
    child->value->generation = oldgen - 1;

    insert_in_asteroid_list(list, child);
}
