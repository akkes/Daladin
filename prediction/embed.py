import sys, ctypes
from ctypes import c_char_p, c_uint32, Structure, POINTER
class RegisterS(Structure):
    pass

lib = ctypes.cdll.LoadLibrary("target/release/libprediction.so")
lib.register_new.restype = POINTER(RegisterS)
lib.register_free.argtypes = (POINTER(RegisterS), )

lib.register_get_size.restype = c_uint32
lib.register_get_size.argtypes = (POINTER(RegisterS), )

lib.register_add_radio.restype = c_uint32
lib.register_add_radio.argtypes = (POINTER(RegisterS), )

lib.register_add_content.restype = c_uint32
lib.register_add_content.argtypes = (POINTER(RegisterS), c_uint32)

lib.register_get_next_content.restype = c_uint32
lib.register_get_next_content.argtypes = (POINTER(RegisterS), c_uint32)


lib.register_apply_feedback.argtypes = (POINTER(RegisterS), c_uint32, c_uint32)

class Register:
    def __init__(self):
        self.obj = lib.register_new()
    def __enter__(self):
        return self
    def __exit__(self, exc_type, exc_value, traceback):
        lib.register_free(self.obj)
    def get_size(self):
        return lib.register_get_size(self.obj)
    def add_radio(self):
        return lib.register_add_radio(self.obj)
    def add_content(self, markov_id):
        return lib.register_add_content(self.obj, markov_id)
    def get_next_content(self, markov_id):
        return lib.register_get_next_content(self.obj, markov_id)
    def apply_feedback(self, markov_id, feedback):
        return lib.register_apply_feedback(self.obj, markov_id, feedback)
# 
# with Register() as register:
#     sample_radio = register.add_radio()
#     genre_list = ["undefined", "techno", "disco", "rock"]
#     register.add_content(0) #ID:1 techno
#     register.add_content(0) #ID:2 disco
#     register.add_content(0) #ID:3 rock
#     for i in range(1, 20):
#         a = genre_list[register.get_next_content(0)]
#         print(a)
#         if a == "techno":
#             print("applying feedback")
#             register.apply_feedback(0, 1)
#     register.add_content(0)
#     genre_list.append("house")
#     for i in range(1, 20):
#         a = genre_list[register.get_next_content(0)]
#         print(a)
#         if a == "house":
#             print("applying feedback")
#             register.apply_feedback(0, 1)
