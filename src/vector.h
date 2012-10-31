#ifndef _VECTOR_H
#define _VECTOR_H

typedef struct Vector {
  int x, y, z;
} Vector;

// Creates a new vector
Vector new_vector();
// Adds two vectors
Vector vec_add(Vector, Vector);
// Subtracts a vector from another
Vector vec_sub(Vector, Vector);
// Calculates the magnitude squared
uint magnitude_squared(Vector);
// Calculate the cross product
Vector cross_product(Vector, Vector);
// Calculate the dot product
int dot_product(Vector, Vector);

#endif
