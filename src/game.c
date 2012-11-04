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
    return ttf_font != NULL;
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
    game->status = Playing;
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
        {100, 50, 0},
        {50, 100, 0},
        {0, 75, 0},
        {-100, 100, 0},
        {-125, 25, 0},
        {-125, -50, 0},
        {-25, -50, 0},
        {-50, -100, 0},
        {25, -100, 0},
        {100, -50, 0},
        {100, -25, 0}
    };
    Vector pos = {0, 0, 0};
    Vector dir = {1, 3, 0};

    Asteroid *a = malloc(sizeof(Asteroid));
    *a = new_asteroid(pos, verts, dir);
    AsteroidNode *n = malloc(sizeof(AsteroidNode));
    n->value = a;

    n->prev = NULL;
    if (g->asteroids == NULL) {
        g->asteroids = n;
        n->next = NULL;
    } else {
        n->next = g->asteroids;
        g->asteroids = n;
    }
}

void handle_key_status(Game *game, ALLEGRO_KEYBOARD_STATE *keys) {
    if (game->status == Playing) {
        if (al_key_down(keys, ALLEGRO_KEY_LEFT))
            rotate_ship_left(game);
        if (al_key_down(keys, ALLEGRO_KEY_RIGHT))
            rotate_ship_right(game);
        if (al_key_down(keys, ALLEGRO_KEY_UP))
            accelerate_ship(game);
        if (al_key_down(keys, ALLEGRO_KEY_DOWN))
            deccelerate_ship(game);
        if (al_key_down(keys, ALLEGRO_KEY_SPACE))
            shoot_bullet(game->bulletmanager, game->ship);

    } else if (game->status == Paused) {

    }
}

void handle_key_event(Game *game, int keycode) {
    if (game->status == Playing) {
        switch(keycode) {
        case ALLEGRO_KEY_ESCAPE:
            game->status = Paused;
            break;
        }
    } else if (game->status == Paused) {
        switch(keycode) {
        case ALLEGRO_KEY_ESCAPE:
            game->status = Playing;
            break;
        }
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
    if (game->ship.invincible)
        return;
    Vector delta;
    delta.x = (sin(-game->ship.angle) * ACCEL_CONST);
    delta.y = (cos(-game->ship.angle) * ACCEL_CONST);
    game->ship.velocity = vec_sub(game->ship.velocity, delta);
    bound_ship_speeds(game);
}

void deccelerate_ship(Game *game) {
    if (game->ship.invincible)
        return;
    Vector delta;
    delta.x = (sin(-game->ship.angle) * ACCEL_CONST);
    delta.y = (cos(-game->ship.angle) * ACCEL_CONST);
    game->ship.velocity = vec_add(game->ship.velocity, delta);
    bound_ship_speeds(game);
}

void update_game(Game *game) {
    if (game->status != Playing)
        return;

    update_asteroids(game->asteroids, game->size);
    update_particles(game);

    if (!is_list_consistent(game->asteroids))
        puts("Inconsistent");

    AsteroidNode* hit = bullet_hit(game->bulletmanager, game->asteroids);
    if (hit != NULL) {
        Vector unit = {0, 1, 0};
        char n;
        for (n = 0; n < ASTEROID_PARTICLEN; n++) {
            float angle = (float)drand48() * 2 * 3.142;
            float mag = (float)drand48() * 2;
            Vector direction =  vec_mul(rotate(unit, angle), mag);
            add_particle(game->particlemanager,
                         hit->value->center, direction, 60);
        }
        split_asteroid(&game->asteroids, hit);
        game->score += ASTEROID_SCORE;
    }

    update_ship(game);

    if (game->lives < 0)
        game->status = Lost;
    else if (!count_asteroids(game->asteroids))
        game->status = Won;
}

void update_ship(Game* game) {
    if (game->ship.invincible) {
        game->ship.invincible--;
    } else {
        game->ship.position =
            wrap(game->size, vec_add(game->ship.position, game->ship.velocity));
        if (point_collides(game->asteroids, game->ship.position) != NULL) {
            game->lives--;
            game->ship.invincible = SHIP_INVINCIBLE;
            game->ship.position = vec_mul(game->size, 0.5);
            game->ship.velocity = new_vector();
            game->ship.angle = 0;
        }
    }
    game->ship.velocity = vec_mul(game->ship.velocity, 1 - SHIP_FRICTION);
}

void update_particles(Game *game) {
    ParticleManager *pm, *bm;
    pm = game->particlemanager;
    bm = &game->bulletmanager->pm;

    bm->current_frame++;
    Particle* particle;
    for(particle = bm->particles;
            (particle - bm->particles) < PARTICLEN;
            particle++) {
        if (particle->alive)
            particle->position = wrap(game->size,
                                      vec_add(particle->position, particle->velocity));
        if (particle->created + particle->lifetime < bm->current_frame)
            particle->alive = false;
    }

    pm->current_frame++;
    for(particle = pm->particles;
            (particle - pm->particles) < PARTICLEN;
            particle++) {
        if (particle->alive)
            particle->position = wrap(game->size,
                                      vec_add(particle->position, particle->velocity));
        if (particle->created + particle->lifetime < pm->current_frame)
            particle->alive = false;
    }

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
    al_rotate_transform(&trans, game->ship.angle);
    al_translate_transform(&trans,
                           (int)game->ship.position.x,
                           (int)game->ship.position.y);
    al_use_transform(&trans);

    int width = !game->ship.invincible ? 2 : 4;

    al_draw_line(-8, 9, 0, -11, SHIP_COLOR , width);
    al_draw_line(0, -11, 8, 9, SHIP_COLOR, width);
    al_draw_line(-6, 4, -1, 4, SHIP_COLOR, width);
    al_draw_line(6, 4, 1, 4, SHIP_COLOR, width);
}

void draw_hud(Game *game) {
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_use_transform(&trans);

    char buffer[17] = "Score: ";
    snprintf(buffer + 7, sizeof(char[10]), "%d", game->score);
    al_draw_text(ttf_font, HUD_COLOR, 0, 0, ALLEGRO_ALIGN_LEFT,
                 buffer);

    int i;
    for (i = 0; i != game->lives; i++) {
        al_identity_transform(&trans);
        al_translate_transform(&trans, 30 + 30 * i, 50);
        al_use_transform(&trans);

        al_draw_line(-8, 9, 0, -11, SHIP_COLOR , 1.0f);
        al_draw_line(0, -11, 8, 9, SHIP_COLOR, 1.0f);
        al_draw_line(-6, 4, -1, 4, SHIP_COLOR, 1.0f);
        al_draw_line(6, 4, 1, 4, SHIP_COLOR, 1.0f);
    }
}
