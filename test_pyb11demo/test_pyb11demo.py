import os, sys
#sys.path.append('C:\\Work\\devlop\\pybind_benchmark\\pyb11_tests\\out\\build\\x64-Debug\\')
sys.path.append('C:\\Work\\devlop\\pybind_benchmark\\pyb11_tests\\out\\build\\x64-Release\\')
from pyb11demo import TimeCalculator
import pyb11demo
import builtins

builtins.nb_callbacks = 0
builtins.callbacks_data = []
def my_callback(data):
    #global nb
    builtins.nb_callbacks += 1
    #builtins.callbacks_data.append(data)
    #print(data)

tc = TimeCalculator()
tc.set_callback(my_callback)

#int nb_iter = 100000000, int payload_length = 64
payload_lengths = [64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536]
nb_iter = [1000, 100_000, 1_000_000, 100_000_000] #, 1_000_000_000]
for lp in payload_lengths:
    for nbi in nb_iter:
        builtins.nb_callbacks = 0
        builtins.callbacks_data = []
        #values = tc.run(100000, 64, 0xf)
        values = tc.run(nbi, lp, 0xf)
        #print("Total callabck = {}".format(builtins.nb_callbacks))
        #print("Data length = {}".format(len(builtins.callbacks_data)))
        #print("{} - Total callabck = {} - Data length = {} - Time = {}ms - {}ms".format(lp, builtins.nb_callbacks, len(builtins.callbacks_data), values['ms_int'], values['ms_double']))
        print("{} - Total callabck = {} - Time = {}ms - {}ms".format(lp, builtins.nb_callbacks, values['ms_int'], values['ms_double']))





