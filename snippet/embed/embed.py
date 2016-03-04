from ctypes import cdll

lib = cdll.LoadLibrary("target/release/libembed.so")

lib.process()
print(lib.sum(4, 6))
print("done!")
