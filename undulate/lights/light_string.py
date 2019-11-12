from collections import deque


class LightString:
    def __init__(self, maxlen: int):
        self.contents = deque(maxlen=maxlen)

    def add(self, value):
        self.contents.appendleft(value)

    def __str__(self):
        return str(self.contents)

    def __repr__(self):
        return repr(self.contents)
