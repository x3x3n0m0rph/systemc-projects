import os
import scipy.integrate
import numpy as np
import math

from pyDigitalWaveTools.vcd.parser import VcdParser
import matplotlib.pyplot as plt

TRACE_FOLDERS = ["trace_s", "trace_m"]
TRACE_FILE_PREFIX = 'freq_'
TRACE_FILE_SUFFIX = '.vcd'

def file_name(file_dir):
    for root, dirs, files in os.walk(file_dir):
        for file in files:
            yield file

for catalogue in TRACE_FOLDERS:
    db = dict()
    for file in file_name(catalogue):
        if file.startswith(TRACE_FILE_PREFIX):
            freq = float(str(file).removeprefix(TRACE_FILE_PREFIX).removesuffix(TRACE_FILE_SUFFIX))
            parser = VcdParser()
            parser.parse(open(catalogue + os.sep + file))
            data = parser.scope.toJson()['children'][0]['children']
            # input = {k: float(v.removeprefix("r")) for k,v in data[0]['data']}
            # output = {k: float(v.removeprefix("r")) for k,v in data[1]['data']}
            input_energy = scipy.integrate.simps(np.array([float(v.removeprefix("r")) for k,v in data[0]['data']])**2)
            output_energy = scipy.integrate.simps((np.array([float(v.removeprefix("r")) for k,v in data[1]['data']])-2.5)**2)
            amplf_db = 10*math.log(output_energy/input_energy)
            print(f"{freq} Hz", input_energy, output_energy, amplf_db)
            db.update({freq: amplf_db})

    lists = sorted(db.items()) # sorted by key, return a list of tuples

    x, y = zip(*lists) # unpack a list of pairs into two tuples

    plt.plot(x, y)
    plt.xscale('log')
    plt.xlabel("Частота, Hz")
    plt.ylabel("Усиление, dB")
    plt.legend()
    plt.savefig(f"{catalogue}.png")
