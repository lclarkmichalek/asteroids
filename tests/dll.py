from ctypes import *

VERTEXN = 12

dll = cdll.LoadLibrary("./libasteroids.so")

class Vector(Structure):
    _fields_ = [('x', c_float),
                ('y', c_float),
                ('z', c_float)]

dll.new_vector.restype = Vector
dll.new_vector.argtypes = []

dll.vec_add.restype = Vector
dll.vec_add.argtypes = [Vector, Vector]

dll.vec_sub.restype = Vector
dll.vec_sub.argtypes = [Vector, Vector]

dll.vec_mul.restype = Vector
dll.vec_mul.argtypes = [Vector, c_float]

dll.magnitude_squared.restype = c_uint
dll.magnitude_squared.argtypes = [Vector]

dll.cross_product.restype = Vector
dll.cross_product.argtypes = [Vector, Vector]

dll.dot_product.restype = c_int
dll.dot_product.argtypes = [Vector, Vector]

dll.wrap.restype = Vector
dll.wrap.argtypes = [Vector, Vector]

dll.rotate.restype = Vector
dll.rotate.argtypes = [Vector, c_float]

dll.in_triangle.restype = c_bool
dll.in_triangle.argtypes = [Vector, Vector, Vector, Vector]

VertexArray = Vector * VERTEXN

class Asteroid(Structure):
    _fields_ = [('vertices', VertexArray),
                ('center', Vector),
                ('radius_squared', c_uint),
                ('direction', Vector),
                ('angle', c_float),
                ('invincible', c_char),
                ('generation', c_char)
                ]

dll.new_asteroid.restype = Asteroid
dll.new_asteroid.argtypes = [Vector, Vector*VERTEXN, Vector]

dll.point_in_asteroid.restype = c_bool
dll.point_in_asteroid.argtypes = [Vector, Asteroid]
