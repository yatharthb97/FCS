#!/usr/bin/env python3
# coding: utf-8

#Shapes

import numpy as np


def Ellipsoid(xr, yr, zr):
    N=50
    stride=1
    u = np.linspace(0, 2 * np.pi, N)
    v = np.linspace(0, np.pi, N)
    x = np.outer(np.cos(u), np.sin(v))
    y = np.outer(np.sin(u), np.sin(v))
    z = np.outer(np.ones(np.size(u)), np.cos(v))
    x*=xr
    y*=yr
    z*=zr
    return x,y,z