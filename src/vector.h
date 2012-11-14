#ifndef _VECTOR_H
#define _VECTOR_H

#include <stdbool.h>

typedef unsigned int uint;

typedef struct Vector {
    float x, y;
} Vector;

// Creates a new vector
Vector new_vector();
// Adds two vectors
Vector vec_add(Vector, Vector);
// Subtracts a vector from another
Vector vec_sub(Vector, Vector);
// Multiplies the vector by a scalar
Vector vec_mul(Vector, float);
// Calculates the magnitude squared
uint magnitude_squared(Vector);
// Calculate the dot product
int dot_product(Vector, Vector);
// mod over the vector
Vector wrap(Vector, Vector);
// Rotates the vector in 2 dimensions
Vector rotate(Vector, float);
// Returns true if the point is in the triangle denoted by the 3 vectors
bool in_triangle(Vector, Vector, Vector, Vector);

#endif
