#include <string.h>
#include <stdlib.h>

#include "./asteroid.h"
#include "./vector.h"

Asteroid new_asteroid(Vector pos, Vector verts[VERTEXN], Vector dir) {
  Asteroid a;
  a.center = vec_add(pos, verts[VERTEXN/2]);
  memcpy(a.verticies, verts, sizeof(Vector[VERTEXN]));

  Vector *vert;
  uint largest_mg2 = 0;
  for(vert = a.verticies; (uint)(vert - a.verticies) < VERTEXN; vert++) {
      uint mag = magnitude_squared(vec_sub(*vert, a.center));
    if (mag > largest_mg2)
      largest_mg2 = mag;
  }
  a.radius_squared = largest_mg2;

  a.direction = dir;
  a.angle = 0;
  return a;
}

void delete_asteroid(Asteroid* a) {
  free(a);
}

// Returns true if point1 and point2 are both on the same side of the line a b
bool same_side(Vector point1, Vector point2, Vector a, Vector b) {
  return dot_product(
    cross_product(vec_sub(b, a), vec_sub(point1, a)),
    cross_product(vec_sub(b, a), vec_sub(point2, a)));
}

// Returns true if the point is inside the triangel a b c
bool in_triangle(Vector point, Vector a, Vector b, Vector c) {
  return same_side(point, a, b, c) && same_side(point, b, a, c) &&
      same_side(point, c, a, b);
}

bool point_in_asteroid(Vector p, Asteroid* asteroid) {
  if (magnitude_squared(vec_sub(p, asteroid->center)) >
      asteroid->radius_squared)
    return false;

  Vector *a, *b;
  b = asteroid->verticies + VERTEXN - 1; // last element
  a = asteroid->verticies;
  while(a - asteroid->verticies < VERTEXN) {
    if (in_triangle(p, *a, *b, asteroid->center))
      return true;
    b = a;
    a++;
  }
  return false;
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
