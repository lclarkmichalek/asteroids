#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include "./asteroid.h"
#include "./particles.h"
#include "./bullets.h"
#include "./ship.h"
#include "./vector.h"
#include "./drawing.h"

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
    al_clear_to_color(BACKGROUND_COLOR);

    draw_asteroids(game->asteroids);
    draw_ship(game);
    draw_bullets(game->bulletmanager);
    draw_particles(game->particlemanager);

    draw_hud(game);
}

void draw_ship(Game *game) {
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_rotate_transform(&trans, game->ship->angle);
    al_translate_transform(&trans,
                           game->ship->position.x,
                           game->ship->position.y);
    al_use_transform(&trans);

    al_draw_line(-8, 9, 0, -11, SHIP_COLOR , 3.0f);
    al_draw_line(0, -11, 8, 9, SHIP_COLOR, 3.0f);
    al_draw_line(-6, 4, -1, 4, SHIP_COLOR, 3.0f);
    al_draw_line(6, 4, 1, 4, SHIP_COLOR, 3.0f);
}

void draw_hud(Game *game) {
    char buffer[80];
    sprintf(buffer, "%i", game->score);
    al_draw_text(ttf_font, HUD_COLOR, 0, 0, ALLEGRO_ALIGN_LEFT,
                 buffer);
}
