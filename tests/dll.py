import ctypes

class Vector(ctypes.Structure):
    _fields_ = [('x', ctypes.c_float),
                ('y', ctypes.c_float),
                ('z', ctypes.c_float)]

dll = ctypes.cdll.LoadLibrary("./libasteroids.so")

dll.new_vector.restype = Vector
dll.new_vector.argtypes = []

dll.vec_add.restype = Vector
dll.vec_add.argtypes = [Vector, Vector]

dll.vec_sub.restype = Vector
dll.vec_sub.argtypes = [Vector, Vector]

dll.vec_mul.restype = Vector
dll.vec_mul.argtypes = [Vector, ctypes.c_float]

dll.magnitude_squared.restype = ctypes.c_uint
dll.magnitude_squared.argtypes = [Vector]

dll.cross_product.restype = Vector
dll.cross_product.argtypes = [Vector, Vector]

dll.dot_product.restype = ctypes.c_int
dll.dot_product.argtypes = [Vector, Vector]

dll.wrap.restype = Vector
dll.wrap.argtypes = [Vector, Vector]

dll.rotate.restype = Vector
dll.rotate.argtypes = [Vector, ctypes.c_float]
