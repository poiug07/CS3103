#!/usr/bin/python3
import random
import sys
import subprocess
import streamlit as st
import time

# ./perf.py <number of executions to average>
NUM = int(sys.argv[1])
random.seed(2003)
start_time = 1645491600
total_times = 0.0

def run(dir, K):
    cmd = ["./test.o", dir, "1645491600", K]
    t=time.time()
    process = subprocess.run(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    elapsed_time = time.time() - t
    return elapsed_time

if __name__ == "__main__":
    for j in range (NUM):
        total=0
        for i in range (5):
            total += run("case%d/"%(i+1), str(int(random.random()*9000)+1))
        total_times += total

    print("Average time: ", total_times/NUM)