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

    // How many frames the particle lives for
    uint lifetime;
    // The frame that the particle was created
    uint created;
    // Is the particle alive
    bool alive;
} Particle;

typedef struct ParticleManager {
    Particle particles[PARTICLEN];

    uint current_frame;
} ParticleManager;

// Creates a new ParticleManager
ParticleManager* new_particle_manager();
// Frees the memory held by the ParticleManager
void delete_particle_manager(ParticleManager*);
// Adds a particle to the particle manager with the given position, direction and lifetime
void add_particle(ParticleManager*, Vector, Vector, uint);

// Draws the particles
void draw_particles(ParticleManager*);

#endif
