#include <stdlib.h>

#include "./particles.h"
#include "./vector.h"

bool is_alive(Particle p, ParticleManager* pm) {
    if (p.created + p.lifetime < pm->current_frame)
        return false;
    else
        return true;
}

ParticleManager* new_particle_manager() {
    ParticleManager* pm = malloc(sizeof(ParticleManager));
    pm->current_frame = 0;
    Particle* particle;
    for(particle = pm->particles;
            (particle - pm->particles) < PARTICLEN;
            particle++) {
        particle->position = new_vector();
        particle->velocity = new_vector();
        particle->lifetime = 0;
        particle->created = 0;
    }
    return pm;
}

void delete_particle_manager(ParticleManager* pm) {
    free(pm);
}

void update_particle_manager(ParticleManager* pm) {
    pm->current_frame++;
    Particle* particle;
    for(particle = pm->particles;
            (particle - pm->particles) < PARTICLEN;
            particle++) {
        if (pm->current_frame - particle->created >= particle->lifetime) {
            particle->position = vec_add(particle->position, particle->velocity);
        }
    }
}

// Finds a 'dead' particle; either a particle that has expired, or the oldest particle
// in the particle manager
Particle* find_dead_particle(ParticleManager* pm) {
    Particle* oldest = NULL;
    Particle* particle;
    for(particle = pm->particles;
            (particle - pm->particles) < PARTICLEN;
            particle++) {
        if (pm->current_frame - particle->created >= particle->lifetime) {
            return particle;
        }
        if (oldest == NULL || particle->created < oldest->created) {
            oldest = particle;
        }
    }
    return oldest;
}

void add_particle(ParticleManager* pm, Vector pos, Vector vel, uint lifetime) {
    Particle* particle = find_dead_particle(pm);
    particle->position = pos;
    particle->velocity = vel;
    particle->lifetime = lifetime;
    particle->created = pm->current_frame;
}
