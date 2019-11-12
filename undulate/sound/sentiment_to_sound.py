import math
import enum


class Emotion(enum.Enum):
    V_HAPPY = 3
    HAPPY = 2
    SAD = 1
    V_SAD = 0


class SentimentSound:
    def __init__(self, sentiment):
        self.note = SentimentSound.map_sentiment_to_note(sentiment)

    @staticmethod
    def map_sentiment_to_note(sentiment):
        """Given a sentiment [0, 1] Real, return an int representing a note"""
        # returns something from 40-100 currently. Low notes are more sad
        return math.floor(sentiment * 60) + 40

    @staticmethod
    def simplify_sentiment(sentiment: float) -> Emotion:
        if 0 > sentiment > 1:
            raise ValueError("sentiment must be in range [0,1]")
        return Emotion(math.floor(sentiment * 4))
