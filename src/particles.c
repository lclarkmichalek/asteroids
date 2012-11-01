#include <stdlib.h>
#include <stdio.h>

#include <allegro5/allegro_primitives.h>

#include "./particles.h"
#include "./vector.h"

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
        particle->alive = false;
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
        if (particle->alive)
            particle->position = vec_add(particle->position, particle->velocity);
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
        if (!particle->alive) {
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
    particle->created = pm->current_frame + 1;
    particle->alive = true;
}

void draw_particles(ParticleManager* pm) {
    ALLEGRO_TRANSFORM trans;
    al_identity_transform(&trans);
    al_use_transform(&trans);
    Particle* p;
    for(p = pm->particles; (p - pm->particles) < PARTICLEN; p++) {
        if (p->alive) {
            Vector p2 = vec_add(p->position, p->velocity);
            al_draw_line(p->position.x, p->position.y, p2.x, p2.y,
                         PARTICLE_COLOR, 1);
            if (p->created + p->lifetime < pm->current_frame)
                p->alive = false;
        }
    }
}
