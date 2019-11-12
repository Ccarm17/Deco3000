from collections import deque
from statistics import mean
import math


class AverageSentiment:
    """
    Stores sentiments, gets averages
    Sentiments should be floats in range [0, 1]
    """
    def __init__(self, size):
        self.contents = deque(maxlen=size)

    def average(self):
        return mean(self.contents)

    def add(self, value):
        # numbers only please
        self.contents.appendleft(value)
