#!/usr/bin/python3
import random
import subprocess
import streamlit as st
import time

# process_time will not include time elapsed during sleep.
# ./test.o case5/ 1645491600 5

NUM = 10

random.seed(2003)
start_time = 1645491600
dir="case5/"
K = 5
total_time = 0.0

for i in range (NUM):
    t=time.time()
    cmd = ["./test.o", "case5/", "1645491600", str(int(random.random()*9000))]
    process = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    elapsed_time = time.time() - t
    total_time += elapsed_time
    # print(process.stdout.decode('utf-8'))
    print(elapsed_time)

print(total_time/NUM)