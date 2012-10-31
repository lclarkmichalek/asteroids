#include <string.h>
#include <stdlib.h>

#include "./asteroid.h"
#include "./vector.h"

Asteroid new_asteroid(Vector pos, Vector verts[VERTEXN], Vector dir) {
  Asteroid a;
  a.center = pos;
  memcpy(a.verticies, verts, sizeof(Vector[VERTEXN]));

  Vector* vert
  uint largest_mg2 = 0;
  for(vert = a.verticies; vert - a.verticies < VERTEXN; vert++) {
    uint mag = magnitude_squared(*vert);
    if (mag > largest_mg2)
      largest_mg2 = mag;
  }
  a.radius_squared = largest_mg2;
  
  a.direction = dir;
  a.angle = 0;
}

void delete_asteroid(Asteroid* a) {
  free(a);
}

void delete_asteroid_node(AsteroidNode* n) {
  delete_asteroid(n->value);
  if (n->prev != NULL)
    n->prev->next = NULL;
  if (n->next != NULL)
    n->next->prev = NULL;
  free(n);
}

AsteroidList new_asteroid_list() {
  AsteroidList a;
  a.head = a.tail = NULL;
}

void delete_asteroid_list(AsteroidList* al) {
  AsteroidNode* n;
  for(n = al->head; n->next != NULL; n = n->next) {
    delete_asteroid_node(n);
  }
  free(al);
}

void append_asteroid_list(AsteroidList* al, AsteroidNode* n) {
  n->prev = al->tail;
  n->next = NULL;
  al->tail->next = n;
  al->tail = n;
}

void preappend_asteroid_list(AsteroidList* al, AsteroidNode* n) {
  n->next = al->head;
  n->prev = NULL;
  al->head->prev = n;
  al->head = n;
}
