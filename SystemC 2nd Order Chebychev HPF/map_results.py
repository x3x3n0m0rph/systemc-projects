import os
from pyDigitalWaveTools.vcd.parser import VcdParser

TRACE_FOLDER = "SystemC 2nd Order Chebychev HPF\\trace"
TRACE_FILE_PREFIX = 'freq_100'
TRACE_FILE_SUFFIX = '.vcd'

def file_name(file_dir):
    for root, dirs, files in os.walk(file_dir):
        for file in files:
            yield file

mapped = dict()
parser = VcdParser()
for file in file_name(TRACE_FOLDER):
    if file.startswith(TRACE_FILE_PREFIX):
        # freq = float(str(file).removeprefix(TRACE_FILE_PREFIX).removesuffix(TRACE_FILE_SUFFIX))
        parser.parse(open(TRACE_FOLDER + os.sep + file))
        data = parser.scope.toJson()

