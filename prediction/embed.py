from ctypes import cdll
import ctypes
lib = cdll.LoadLibrary("target/release/libprediction.so")

lib.get_first_first_probability.restype = ctypes.c_double;
print(type(lib.get_first_first_probability()))
print(lib.get_first_first_probability())
print(lib.sum(0))
print("done!")
