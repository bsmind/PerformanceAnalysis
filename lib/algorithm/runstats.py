"""@package Chimbuko

Computing mean and standard deviation in the incremental manner
(ref: https://en.wikipedia.org/wiki/Standard_deviation)

Author(s):
    Sungsoo Ha (sungsooha@bnl.gov)

Created:
    March 12, 2018

Last Modified:
    March 13, 2019
"""
import numpy as np

class RunStats(object):
    def __init__(self, s0=0., s1=0., s2=0.):
        """
        constructor
        Args:
            s0: initial power of sum with j == 0
            s1: initial power of sum with j == 1
            s2: initial power of sum with j == 2
        """
        self.s0 = s0
        self.s1 = s1
        self.s2 = s2

    def __str__(self):
        return "stat: s0 = {:.3f}, s1 = {:.3f}, s2 = {:.3f}".format(self.s0, self.s1, self.s2)

    def mean(self):
        """Return current mean"""
        try:
            mn = self.s1 / self.s0
        except ZeroDivisionError:
            mn = 0
        return mn

    def var(self):
        """Return currrent variance"""
        try:
            var = (self.s0*self.s2 - self.s1*self.s1) / (self.s0*self.s0)
        except ZeroDivisionError:
            var = np.inf
        return var

    def std(self):
        """Return current standard deviation"""
        return np.sqrt(self.var())

    def stat(self):
        return [self.s0, self.s1, self.s2]

    def add(self, x):
        """Add single data point"""
        self.s0 += 1
        self.s1 += x
        self.s2 += x*x

    def update(self, s0, s1, s2):
        self.s0 += s0
        self.s1 += s1
        self.s2 += s2

    def reset(self, s0, s1, s2):
        self.s0 = s0
        self.s1 = s1
        self.s2 = s2

    def __add__(self, other):
        """Add two running stat
        """
        s0 = self.s0 + other.s0
        s1 = self.s1 + other.s1
        s2 = self.s2 + other.s2

        return RunStats(s0, s1, s2)


