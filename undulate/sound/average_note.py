from collections import deque
from statistics import mean
import math


class AverageNote:
    def __init__(self, size):
        self.contents = deque(maxlen=size)
        for x in range(size):
            self.contents.append(60)  # Middle C

    def average(self):
        return math.floor(mean(self.contents))

    def add(self, value):
        # numbers only please
        self.contents.appendleft(value)
