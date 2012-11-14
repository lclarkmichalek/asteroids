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

float normal_rand() {
    float v = drand48();
    return -(2*v - 1) * (2*v - 1) + 1;
}

void spawn_asteroid(Game *g) {
    Vector verts[VERTEXN];

    int i;
    float delta = 2 * 3.142 / VERTEXN;
    for (i = 0; i < VERTEXN; i++) {
        float mag = 70 * normal_rand() + 30;
        Vector v = {
            mag * cos(i * delta),
            mag * sin(i * delta)
        };
        verts[i] = v;
    }

    Vector pos = {drand48() * g->size.x,
                  drand48() * g->size.y
                 };
    Vector dir = {drand48() * 10,
                  drand48() * 10
                 };
    bound_asteroid_speeds(&dir);

    Asteroid *a = malloc(sizeof(Asteroid));
    *a = new_asteroid(pos, verts, dir);
    a->angle = drand48() * 2 * 3.142;
    AsteroidNode *n = malloc(sizeof(AsteroidNode));
    n->value = a;

    if (!point_in_asteroid_area(g->ship.position, a))
        insert_in_asteroid_list(&g->asteroids, n);
    else
        spawn_asteroid(g);
}

void handle_key_status(Game *game, ALLEGRO_KEYBOARD_STATE *keys) {
    if (game->status == Playing) {
        if (al_key_down(keys, ALLEGRO_KEY_LEFT))
            rotate_ship_left(&game->ship);
        if (al_key_down(keys, ALLEGRO_KEY_RIGHT))
            rotate_ship_right(&game->ship);
        if (al_key_down(keys, ALLEGRO_KEY_UP))
            accelerate_ship(&game->ship);
        if (al_key_down(keys, ALLEGRO_KEY_DOWN))
            deccelerate_ship(&game->ship);
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
    } else if (game->status == Won
               || game->status == Lost)
        game->status = Quit;
}

void emit_asteroid_hit_particles(ParticleManager *pm, Vector center) {
    Vector unit = {0, 1};
    char n;
    for (n = 0; n < ASTEROID_PARTICLEN; n++) {
        float angle = (float)drand48() * 2 * 3.142;
        float mag = (float)drand48() * 2;
        Vector direction = vec_mul(rotate(unit, angle), mag);
        add_particle(pm,
                     center, direction, 60);
    }
}

void update_game(Game *game) {
    if (game->status == Paused)
        update_paused(game);
    if (game->status != Playing)
        return;

    update_asteroids(game->asteroids, game->size);
    update_particles(game);

    if (!is_list_consistent(game->asteroids))
        puts("Inconsistent");

    AsteroidNode* hit = bullet_hit(game->bulletmanager, game->asteroids);
    if (hit != NULL && !hit->value->invincible) {
        emit_asteroid_hit_particles(game->particlemanager, hit->value->center);
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
        if (point_collides(game->asteroids, game->ship.position) != NULL) {
            game->lives--;
            game->ship.invincible = SHIP_INVINCIBLE;
            game->ship.position = vec_mul(game->size, 0.5);
            game->ship.velocity = new_vector();
            game->ship.angle = 0;
        }
    }
    game->ship.position =
        wrap(game->size, vec_add(game->ship.position, game->ship.velocity));
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

void update_paused(Game *game) {
    game->status = Paused;
}

void init_layer(Game *game, ALLEGRO_BITMAP **mask, float opacity) {
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_use_transform(&trans);
    *mask = al_create_bitmap(game->size.x, game->size.y);
    ALLEGRO_BITMAP *old = al_get_target_bitmap();
    al_set_target_bitmap(*mask);

    al_clear_to_color(al_map_rgba(0,0,0, 255 - (int)(opacity * 255)));

    int b1, b2, b3;
    al_get_blender(&b1, &b2, &b3);
    al_set_blender(ALLEGRO_DEST_MINUS_SRC, ALLEGRO_ONE, ALLEGRO_ONE);

    al_set_blender(b1, b2, b3);
    al_set_target_bitmap(old);
}

void draw_mask(ALLEGRO_BITMAP *mask) {
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_use_transform(&trans);
    al_draw_bitmap(mask, 0, 0, 0);

    al_destroy_bitmap(mask);
}

void draw_game(Game *game, float opacity) {
    ALLEGRO_BITMAP *mask = NULL;
    if (opacity != 1)
        init_layer(game, &mask, opacity);

    al_clear_to_color(BACKGROUND_COLOR);

    draw_asteroids(game->asteroids);
    draw_ship(game);
    draw_bullets(game->bulletmanager);
    draw_particles(game->particlemanager);

    draw_hud(game);

    if (opacity != 1)
        draw_mask(mask);
}

void draw_paused(Game *game) {
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_use_transform(&trans);

    int x, y;
    x = (game->size.x - 120) / 2;
    y = (game->size.y - 30) / 2;
    al_draw_text(ttf_font, al_map_rgb(200, 200, 200),
                 x, y, ALLEGRO_ALIGN_LEFT, "Paused");

    char* msg = "Use the arrow keys to move, and space to shoot";
    y = game->size.y - 130;
    x = (game->size.x - 740)/2;
    al_draw_text(ttf_font, al_map_rgb(200, 200, 200), x, y,
                 ALLEGRO_ALIGN_LEFT, msg);

    msg = "If your ship or an asteroid is bold, it is invincible";
    y = game->size.y - 80;
    x = (game->size.x - 800)/2;
    al_draw_text(ttf_font, al_map_rgb(200, 200, 200), x, y,
                 ALLEGRO_ALIGN_LEFT, msg);
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

    al_draw_line(-8, 20, 0, 0, SHIP_COLOR , width);
    al_draw_line(8, 20, 0, 0, SHIP_COLOR, width);
    al_draw_line(-6, 15, -1, 15, SHIP_COLOR, width);
    al_draw_line(6, 15, 1, 15, SHIP_COLOR, width);
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
    for (i = 0; i < game->lives; i++) {
        al_identity_transform(&trans);
        al_translate_transform(&trans, 30 + 30 * i, 50);
        al_use_transform(&trans);

        al_draw_line(-8, 9, 0, -11, SHIP_COLOR , 1.0f);
        al_draw_line(0, -11, 8, 9, SHIP_COLOR, 1.0f);
        al_draw_line(-6, 4, -1, 4, SHIP_COLOR, 1.0f);
        al_draw_line(6, 4, 1, 4, SHIP_COLOR, 1.0f);
    }
}

void draw_won(Game *game) {
    int x, y;
    x = (game->size.x - 50) / 2;
    y = (game->size.y - 30) / 2;
    al_draw_text(ttf_font, al_map_rgb(200, 200, 200), x, y,
                 ALLEGRO_ALIGN_LEFT, "Congratulations! You won");
}

void draw_lost(Game *game) {
    int x, y;
    x = (game->size.x - 50) / 2;
    y = (game->size.y - 30) / 2;
    al_draw_text(ttf_font, al_map_rgb(200, 200, 200), x, y,
                 ALLEGRO_ALIGN_LEFT, "You lost");
}
