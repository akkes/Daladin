from ctypes import cdll

lib = cdll.LoadLibrary("target/release/libprediction.so")


print("done!")
