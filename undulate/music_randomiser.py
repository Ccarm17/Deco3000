from sound.music_player import MusicPlayer
import random
from time import sleep
from excluded.config import midi_output

from sentiment.average_sentiment import AverageSentiment

if __name__ == '__main__':
    m = MusicPlayer(midi_output)
    sentiments = AverageSentiment(5)

    while True:
        sentiment = random.random()
        print(f'sentiment: {sentiment}')
        sentiments.add(sentiment)
        m.sentiment_to_note(sentiments.average())
        sleep(random.random() * 2)
