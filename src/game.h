#ifndef _GAME_H
#define _GAME_H

#include <stdbool.h>

#include "./vector.h"
#include "./asteroid.h"
#include "./particles.h"
#include "./ship.h"
#include "./bullets.h"

#define LIVESN 3
#define SHIP_ROTATION_SPEED 0.01
#define SHIP_COLOR al_map_rgb(255, 255, 255)
#define BACKGROUND_COLOR al_map_rgb(0, 0, 0)
#define HUD_COLOR al_map_rgb(255, 255, 255)

// Initialises and loads fonts
bool init_font();

typedef struct Game {
    // The currently displayed asteroids
    AsteroidNode* asteroids;

    // The particle manager for the scene
    ParticleManager* particlemanager;

    // The player's ship
    Ship ship;

    // The currently displayed bullets
    BulletManager* bulletmanager;

    // The players score
    int score;

    // The players lives
    int lives;

    // The game size
    Vector size;
} Game;

// Creates a new empty game with the given screen size
Game* new_game(Vector size);
// Deletes the game and all component parts
void delete_game(Game*);
// Spawns a new asteroid. The ship will _not_ be inside the asteroid.
void spawn_asteroid(Game*);
// Rotates the ship left
void rotate_ship_left(Game*);
// Rotates the ship right
void rotate_ship_right(Game*);

// Updates the gamestate for a single frame
void update_game(Game*);
// Draws the game
void draw_game(Game*);
// Draws the spaceship
void draw_ship(Game*);
// Draws the hud
void draw_hud(Game*);

#endif
