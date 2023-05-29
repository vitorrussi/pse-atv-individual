import numpy as np
import os
import time

M = np.linspace(1,6,6,dtype=int)
C = np.linspace(1,6,6,dtype=int)

def clearOutput():
    with open('output.txt', 'w') as f:
        f.write('')

def getIfOutputLastLineIsSegmentationFault():
    with open('output.txt', 'r') as f:
        lines = f.readlines()
        if 'Segmentation fault' in lines[-1]:
            return True
    return False

def getIfOutputIsSegmentationFault():
    with open('output.txt', 'r') as f:
        for line in f:
            if 'Segmentation fault' in line:
                return True
    return False

def runMake():
    # clearOutput()
    os.system('rm output.txt')
    os.system('ERROR=$(make clean build run 2>&1 > /dev/null); echo $ERROR > output.txt')
    # os.system('make')
    # os.system('make run > output.txt 2>&1 ')
    # wait until the output.txt is created
    while not os.path.exists('output.txt'):
        time.sleep(1)

    


outputs_text = []

for m in M:
    for c in C:
        N = np.linspace(m*c+1, m*c+4, 4, dtype=int)
        for n in N:
            with open('./include/defines.h', 'w') as f:
                f.write('#pragma once\n')
                f.write('/* passengers */\n')
                f.write('const int n = {};\n'.format(n))
                f.write('/* cars */\n')
                f.write('const int m = {};\n'.format(m))
                f.write('/* car slots */\n')
                f.write('const int C = {};\n'.format(c))
            runMake()
            if getIfOutputIsSegmentationFault():
                text = 'm = {}, C = {}, n = {} : Segmentation fault'.format(m,c,n)
                outputs_text.append(text)
                print(text)
            else:
                text = 'm = {}, c = {}, n = {} : OK'.format(m,c,n)
                outputs_text.append(text)
                print(text)
