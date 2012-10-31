#include <stdlib.h>

#include "./bullets.h"

#include "./ship.h"
#include "./particles.h"

BulletManager* new_bullet_manager() {
    return new_particle_manager();
}

void delete_bullet_manager(BulletManager* bm) {
    return delete_particle_manager(bm);
}

void shoot_bullet(BulletManager* bm, Ship ship) {
    Vector velocity = vec_mul(ship.velocity, 1.2);
    add_particle(bm, ship.position, velocity, BULLET_LIFETIME);
}
