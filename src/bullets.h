#ifndef _BULLET_H
#define _BULLET_H

#include "./particles.h"
#include "./ship.h"

#define BULLET_LIFETIME 120

typedef ParticleManager BulletManager;

// Creates a new empty bullet manager
BulletManager* new_bullet_manager();
// Deletes the given bullet manager
void delete_bullet_manager(BulletManager*);
// Adds a new bullet
void shoot_bullet(BulletManager*, Ship);

#endif
