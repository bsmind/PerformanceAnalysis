import sys
import time
import os
import MILOF

filepath = 'data/testdata.mat'
dimension = 2
num_k = 10
kpar = 4
buck = 1024
width = 0

MILOF.MILOF_Kmeans_Merge(kpar, dimension, buck, filepath, num_k, width)