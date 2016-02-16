#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
import sys

import rospy
from speech_msgs.msg import AudioData
import subprocess
import codecs
import wave
import pyaudio
from numpy import *
import os, glob, random
import struct
import time

import cPickle

import numpy as np
import six
import cv2
import os
import six.moves.cPickle as pickle

import chainer
from chainer import computational_graph as c
from chainer import cuda
import chainer.functions as F
from chainer import optimizers

model = pickle.load(open('model.pkl','rb'))
