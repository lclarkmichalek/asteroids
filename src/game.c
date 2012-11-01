#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>

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

ALLEGRO_FONT *ttf_font = NULL;

bool init_font() {
    al_init_font_addon();
    if (!al_init_ttf_addon())
        return 1;
    ttf_font = al_load_ttf_font("font.ttf", 30, 0);
    return !ttf_font;
}

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

void spawn_asteroid(Game *g) {
    Vector verts[VERTEXN] = {
        {0, 0, 0},
        {20, 10, 0},
        {10, 20, 0},
        {0, 15, 0},
        {-20, 20, 0},
        {-25, 5, 0},
        {-25, -10, 0},
        {-5, -10, 0},
        {-10, -20, 0},
        {5, -20, 0},
        {20, -10, 0},
        {20, -5, 0}};
    Vector pos = {0, 0, 0};
    Vector dir = {1, 3, 0};

    Asteroid *a = malloc(sizeof(Asteroid));
    *a = new_asteroid(pos, verts, dir);
    AsteroidNode *n = malloc(sizeof(AsteroidNode));
    n->value = a;

    if (g->asteroids == NULL) {
        g->asteroids = n;
        n->next = NULL;
    } else {
        n->next = g->asteroids;
        g->asteroids = n;
    }
}

void rotate_ship_left(Game *game) {
    game->ship.angle -= SHIP_ROTATION_SPEED;
}

void rotate_ship_right(Game *game) {
    game->ship.angle += SHIP_ROTATION_SPEED;
}

void bound_ship_speeds(Game *game) {
    Vector *v = &game->ship.velocity;
    if (v->x < -MAX_SPEED)
        v->x = -MAX_SPEED;
    if (v->x > MAX_SPEED)
        v->x = MAX_SPEED;
    if (v->y < -MAX_SPEED)
        v->y = -MAX_SPEED;
    if (v->y > MAX_SPEED)
        v->y = MAX_SPEED;
}

void accelerate_ship(Game *game) {
    Vector delta;
    delta.x = (sin(-game->ship.angle) * ACCEL_CONST);
    delta.y = (cos(-game->ship.angle) * ACCEL_CONST);
    game->ship.velocity = vec_sub(game->ship.velocity, delta);
    bound_ship_speeds(game);
}

void deccelerate_ship(Game *game) {
    Vector delta;
    delta.x = (sin(-game->ship.angle) * ACCEL_CONST);
    delta.y = (cos(-game->ship.angle) * ACCEL_CONST);
    game->ship.velocity = vec_add(game->ship.velocity, delta);
    bound_ship_speeds(game);
}

void update_game(Game *game) {
    update_asteroids(game->asteroids, game->size);
    update_particle_manager(game->particlemanager);
    update_bullet_manager(game->bulletmanager);
    game->ship.position =
        wrap(game->size, vec_add(game->ship.position, game->ship.velocity));

    AsteroidNode* hit = bullet_hit(game->bulletmanager, game->asteroids);
    if (hit != NULL) {
        if (hit == game->asteroids)
            puts("T");
        else
            puts("F");
        split_asteroid(hit);
    }

    if (!game->ship.invincible &&
        point_collides(game->asteroids, game->ship.position) != NULL) {
        game->lives--;
        game->ship.invincible = SHIP_INVINCIBLE;
    }
    if (game->ship.invincible)
        game->ship.invincible--;

    int c = 0;
    AsteroidNode* n;
    for(n = game->asteroids; n != NULL; n = n->next)
        c++;
    printf("A: %i L: %i I: %i\n", c, game->lives, game->ship.invincible);
}

void draw_game(Game *game) {
    al_clear_to_color(BACKGROUND_COLOR);

    draw_asteroids(game->asteroids);
    draw_ship(game);
    draw_bullets(game->bulletmanager);
    draw_particles(game->particlemanager);

    //    draw_hud(game);
}

void draw_ship(Game *game) {
    // Flash the ship if it's invincible
    if (game->ship.invincible % 30 > 2)
        return;

    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_rotate_transform(&trans, game->ship.angle);
    al_translate_transform(&trans,
                           (int)game->ship.position.x,
                           (int)game->ship.position.y);
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
