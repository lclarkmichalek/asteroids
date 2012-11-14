#ifndef _PARTICLES_H
#define _PARTICLES_H

#include <stdbool.h>

#include "./vector.h"

#define PARTICLEN 256
#define PARTICLE_COLOR al_map_rgb(255, 255, 255)

typedef struct Particle {
    // The position of the particle
    Vector position;
    // The direction of the particle
    Vector velocity;

    // How many seconds the particle lives for
    float lifetime;
    // The time that the particle was created
    float created;
    // Is the particle alive
    bool alive;
} Particle;

typedef struct ParticleManager {
    Particle particles[PARTICLEN];

    float current_time;
} ParticleManager;

// Creates a new ParticleManager
ParticleManager* new_particle_manager();
// Frees the memory held by the ParticleManager
void delete_particle_manager(ParticleManager*);
// Adds a particle to the particle manager with the given position, direction and lifetime
void add_particle(ParticleManager*, Vector, Vector, float);

// Draws the particles
void draw_particles(ParticleManager*);

#endif
