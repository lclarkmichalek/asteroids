#ifndef _VECTOR_H
#define _VECTOR_H

typedef unsigned int uint;

typedef struct Vector {
    float x, y, z;
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
// Calculate the cross product
Vector cross_product(Vector, Vector);
// Calculate the dot product
int dot_product(Vector, Vector);
// mod over the vector
Vector wrap(Vector, Vector);
// Rotates the vector in 2 dimensions
Vector rotate(Vector, float);

#endif
