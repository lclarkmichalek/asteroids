#include <stdlib.h>
#include <stdbool.h>

#include <allegro5/allegro.h>

#include "./asteroid.h"
#include "./particles.h"
#include "./bullets.h"
#include "./ship.h"
#include "./vector.h"

#include "./game.h"

Game* new_game(Vector size) {
    Game *game = malloc(sizeof(Game));
    game->asteroids = NULL;
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

void rotate_ship_left(Game *game) {
    game->ship.angle -= SHIP_ROTATION_SPEED;
}

void rotate_ship_right(Game *game) {
    game->ship.angle += SHIP_ROTATION_SPEED;
}

void update_game(Game *game) {
    update_asteroids(game->asteroids);
    update_particle_manager(game->particlemanager);
    update_bullet_manager(game->bulletmanager);
    game->ship.position = vec_add(game->ship.position, game->ship.velocity);

    AsteroidNode* hit = bullet_hit(game->bulletmanager, game->asteroids);
    if (hit != NULL)
        split_asteroid(hit);

    if (point_collides(game->asteroids, game->ship.position) != NULL)
        game->lives--;
}

void draw_game(Game *game) {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    draw_asteroids(game->asteroids);
    draw_ship(game);
    draw_bullets(game->bulletmanager);
    draw_particles(game->particlemanager);
    
    draw_hud(game);
}
