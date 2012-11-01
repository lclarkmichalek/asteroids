#include <stdlib.h>

#include <allegro5/allegro_primitives.h>

#include "./ship.h"
#include "./particles.h"
#include "./asteroid.h"

#include "./bullets.h"

BulletManager* new_bullet_manager() {
    return new_particle_manager();
}

void delete_bullet_manager(BulletManager* bm) {
    delete_particle_manager(bm);
}

void update_bullet_manager(BulletManager* bm) {
    update_particle_manager(bm);
}

void shoot_bullet(BulletManager* bm, Ship ship) {
    Vector velocity = vec_mul(ship.velocity, 1.2);
    add_particle(bm, ship.position, velocity, BULLET_LIFETIME);
}

AsteroidNode* bullet_hit(BulletManager* bm, AsteroidNode* asteroids) {
    Particle* bullet;
    for(bullet = bm->particles; bullet - bm->particles < PARTICLEN; bullet++) {
        if (is_alive(*bullet, bm)) {
            AsteroidNode* node = point_collides(asteroids, bullet->position);
            if (node)
                return node;
            // Then kill the bullet
            bullet->lifetime = 0;
        }
    }
    return NULL;
}

void draw_bullets(BulletManager *bm) {
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_use_transform(&trans);
    Particle *b;
    for(b = bm->particles; b - bm->particles < PARTICLEN; b++) {
        if (is_alive(*b, bm)) {
            Vector p2 = vec_add(b->position, b->velocity);
            al_draw_line(b->position.x, b->position.y, p2.x, p2.y,
                         BULLET_COLOR, 1);
        }
    }
}
