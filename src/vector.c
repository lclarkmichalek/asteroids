#include <math.h>

#include "./vector.h"

Vector new_vector() {
    Vector c;
    c.x = 0;
    c.y = 0;
    return c;
}

Vector vec_add(Vector a, Vector b) {
    Vector c = new_vector();
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

Vector vec_sub(Vector a, Vector b) {
    Vector c = new_vector();
    c.x = a.x - b.x;
    c.y = a.y - b.y;
    return c;
}

Vector vec_mul(Vector a, float sf) {
    Vector c = new_vector();
    c.x = a.x * sf;
    c.y = a.y * sf;
    return c;
}

// We don't calculate the actual magnitude as sqrt is expensive
uint magnitude_squared(Vector v) {
    return v.x * v.x + v.y * v.y;
}

int dot_product(Vector a, Vector b) {
    return a.x * b.x + a.y * b.y;
}

Vector wrap(Vector size, Vector p) {
    Vector c;
    c.x = fmod(p.x + size.x, size.x);
    c.y = fmod(p.y + size.y, size.y);
    return c;
}

Vector rotate(Vector v, float a) {
    Vector c;
    c.x = v.x * cos(a) - v.y * sin(a);
    c.y = v.x * sin(a) + v.y * cos(a);
    return c;
}

// Returns true if the point is inside the triangel a b c
bool in_triangle(Vector p, Vector a, Vector b, Vector c) {
    Vector ac, ab, ap;
    ac = vec_sub(c, a);
    ab = vec_sub(b, a);
    ap = vec_sub(p, a);

    int dotcc, dotcb, dotcp, dotbb, dotbp;
    dotcc = dot_product(ac, ac);
    dotcb = dot_product(ac, ab);
    dotcp = dot_product(ac, ap);
    dotbb = dot_product(ab, ab);
    dotbp = dot_product(ab, ap);

    float invdenom, u, v;
    invdenom = 1.0f/ (dotcc * dotbb - dotcb * dotcb);
    u = (dotbb * dotcp - dotcb * dotbp) * invdenom;
    v = (dotcc * dotbp - dotcb * dotcp) * invdenom;

    return (u >= 0) && (v >= 0) && (u + v <= 1);
}
