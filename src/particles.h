#ifndef _PARTICLES_H
#define _PARTICLES_H

#include "./vector.h"

static const int PARTICLEN = 256;

typedef struct Particle {
  // The position of the particle
  Vector position;
  // The direction of the paricle
  Vector velocity;

  // How many frames the particle lives for
  uint lifetime;
  // The frame that the particle was created
  uint created;
} Particle;

typedef struct ParticleManager {
  Particle particles[PARTICLEN];
  
  uint current_frame;
} ParticleManager;

// Creates a new ParticleManager
ParticleManager* new_particle_manager();
// Frees the memory held by the ParticleManager
void delete_particle_manager(ParticleManager*);
// Updates the particles in the ParticleManager.
// Must be called once and only once per frame
void update_particle_manager(ParticleManager*);
// Adds a particle to the particle manager with the given position, direction and lifetime
void add_particle(ParticleManager*, Vector, Vector, uint);

#endif
