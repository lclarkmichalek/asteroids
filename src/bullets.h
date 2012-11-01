#ifndef _BULLET_H
#define _BULLET_H

#include "./particles.h"
#include "./ship.h"
#include "./asteroid.h"

#define BULLET_LIFETIME 60
#define BULLET_SPEED 7
#define BULLET_COLOR al_map_rgb(255, 255, 255)
#define SHOT_DELAY 7

typedef struct {
    ParticleManager pm;
    uint last_shot;
} BulletManager;

// Creates a new empty bullet manager
BulletManager* new_bullet_manager();
// Deletes the given bullet manager
void delete_bullet_manager(BulletManager*);
// Updates the bullets
void update_bullet_manager(BulletManager*);
// Adds a new bullet
void shoot_bullet(BulletManager*, Ship);
// Returns true if any bullet is inside an asteroid
AsteroidNode* bullet_hit(BulletManager*, AsteroidNode*);
// Draws the bullets to the screen
void draw_bullets(BulletManager*);

#endif
