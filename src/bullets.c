#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <allegro5/allegro_primitives.h>

#include "./ship.h"
#include "./particles.h"
#include "./asteroid.h"

#include "./bullets.h"

BulletManager* new_bullet_manager() {
    BulletManager *bm = malloc(sizeof(BulletManager));
    ParticleManager* pm = new_particle_manager();
    bm->pm = *pm;
    free(pm);
    bm->last_shot = -(SHOT_DELAY + 1);
    return bm;
}

void delete_bullet_manager(BulletManager* bm) {
    delete_particle_manager(&bm->pm);
}

void shoot_bullet(BulletManager* bm, Ship ship) {
    if (bm->pm.current_time - bm->last_shot < SHOT_DELAY)
        return;
    if (ship.invincible > 0)
        return;
    bm->last_shot = bm->pm.current_time;
    Vector velocity = {
        -sin(-ship.angle) * BULLET_SPEED,
        -cos(-ship.angle) * BULLET_SPEED
    };
    add_particle(&bm->pm, ship.position, velocity, BULLET_LIFETIME);
}

AsteroidNode* bullet_hit(BulletManager* bm, AsteroidNode* asteroids) {
    Particle* bullet;
    for(bullet = bm->pm.particles;
            bullet - bm->pm.particles < PARTICLEN; bullet++) {
        if (bullet->alive) {
            Vector np = vec_add(bullet->position, vec_mul(bullet->velocity, 0.015));
            AsteroidNode* node = point_collides(asteroids, np);
            if (node) {
                bullet->alive = false;
                return node;
            }
        }
    }
    return NULL;
}

void draw_bullets(BulletManager *bm) {
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_use_transform(&trans);
    Particle *b;
    for(b = bm->pm.particles; b - bm->pm.particles < PARTICLEN; b++) {
        if (b->alive) {
            Vector p2 = vec_sub(b->position, vec_mul(b->velocity, 0.01));
            al_draw_line(b->position.x, b->position.y, p2.x, p2.y,
                         BULLET_COLOR, 2);
        }
    }
}
