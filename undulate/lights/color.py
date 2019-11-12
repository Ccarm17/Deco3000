import math
import colorsys


class Color:
    def __init__(self, red: int, green: int, blue: int):
        if red < 0 or green < 0 or blue < 0 \
                or red > 255 or green > 255 or blue > 255:
            raise ValueError('Color values must be in the range [0,255]')

        self._red: int = red
        self._green: int = green
        self._blue: int = blue

    def str(self):
        # Good format for the MQTT
        r = str(self._red).zfill(3)
        g = str(self._green).zfill(3)
        b = str(self._blue).zfill(3)
        return f'{r}{g}{b}'

    def __repr__(self):
        # Good format for the MQTT
        r = str(self._red).zfill(3)
        g = str(self._green).zfill(3)
        b = str(self._blue).zfill(3)
        return f'{r}{g}{b}'

    @staticmethod
    def sentiment_to_hue(sentiment):
        # sentiment = [0,1]
        return sentiment * 0.5

    @staticmethod
    def _float_to_1_255(x):
        return math.floor(x * 255)

    @staticmethod
    def sentiment_to_rgb(sentiment):
        hue = Color.sentiment_to_hue(sentiment)
        color = colorsys.hsv_to_rgb(hue, 1, 1)
        r = Color._float_to_1_255(color[0])
        g = Color._float_to_1_255(color[1])
        b = Color._float_to_1_255(color[2])
        return Color(r, g, b)
