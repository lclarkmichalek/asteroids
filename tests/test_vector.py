import ctypes
import unittest

from .dll import *

class TestVectorOperations(unittest.TestCase):
    def vector_equal(self, v1, v2):
        self.assertEqual(v1.x, v2.x)
        self.assertEqual(v1.y, v2.y)

    def test_addition(self):
        a = dll.new_vector()
        b = dll.new_vector()
        c = dll.new_vector()
        self.vector_equal(c, dll.vec_add(a, b))

        a.x = 3
        a.y = 2
        b.x = 4
        b.z = 2

        c.x = 7
        c.y = 2
        c.z = 2
        self.vector_equal(c, dll.vec_add(a, b))

    def test_subtraction(self):
        a = dll.new_vector()
        b = dll.new_vector()
        c = dll.new_vector()
        self.vector_equal(c, dll.vec_sub(a, b))

        a.x = 3
        a.y = 2
        b.x = 4
        b.z = 2

        c.x = -1
        c.y = 2
        c.z = -2
        self.vector_equal(c, dll.vec_sub(a, b))

    def test_multiplication(self):
        a = dll.new_vector()
        b = dll.new_vector()
        self.vector_equal(b, dll.vec_mul(a, 100))

        a.x = 3
        a.y = 2
        m = 3

        b.x = 9
        b.y = 6
        self.vector_equal(b, dll.vec_mul(a, m))

    def test_magnitude_squared(self):
        a = dll.new_vector()
        self.assertEquals(0, dll.magnitude_squared(a))

        a.x = 3
        a.y = 4
        a.z = 5

        self.assertEquals(50, dll.magnitude_squared(a))

    def test_in_triangle(self):
        a = Vector(-1, -1, 0)
        b = Vector(-1, 1, 0)
        c = Vector(1, 0, 0)
        p = Vector(0, 0, 0)
        self.assertTrue(dll.in_triangle(p, a, b, c))

        a = Vector(-101, -101, 0)
        b = Vector(-101, -99, 0)
        c = Vector(-99, -100, 0)
        p = Vector(-100, -100, 0)
        self.assertTrue(dll.in_triangle(p, a, b, c))

        a = Vector(0, 0, 0)
        b = Vector(0, 0, 0)
        c = Vector(1, 0, 0)
        p = Vector(1, 1, 0)
        self.assertFalse(dll.in_triangle(p, a, b, c))

        a = Vector(-101, -101, 0)
        b = Vector(-101, -99, 0)
        c = Vector(-99, -100, 0)
        p = Vector(100, 100, 0)
        self.assertFalse(dll.in_triangle(p, a, b, c))
