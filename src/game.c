#include <stdlib.h>
#include <stdbool.h>

#include "./game.h"

#include "./asteroid.h"
#include "./particles.h"
#include "./bullets.h"
#include "./ship.h"
#include "./vector.h"

Game* new_game(Vector size) {
    Game *game = malloc(sizeof(Game));
    game->asteroids = new_asteroid_list();
    game->particlemanager = new_particle_manager();
    game->bulletmanager = new_bullet_manager();
    game->ship = new_ship(vec_mul(size, 0.5), new_vector());
    game->size = size;

    game->score = 0;
    game->lives = LIVESN;
    return game;
}

void delete_game(Game *g) {
    delete_asteroid_list(g->asteroids);
    delete_particle_manager(g->particlemanager);
    delete_bullet_manager(g->bulletmanager);
    free(g);
}

AsteroidNode* point_collides(Game *g, Vector point) {
    AsteroidNode *node;
    for(node = g->asteroids.head;
        node != g->asteroids.tail;
        node = node->next) {
        if (point_in_asteroid(point, node->value)) {
            return node;
        }
    }
    return NULL;
}

void rotate_ship_left(Game *game) {
    game->ship.angle -= SHIP_ROTATION_SPEED;
}

void rotate_ship_right(Game *game) {
    game->ship.angle += SHIP_ROTATION_SPEED;
}
