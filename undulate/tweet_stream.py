## If you're getting error 401:
#       1) Reset your system clock!
#       2) Check your API keys for tweepy

import tweepy
import random
import paho.mqtt.client as mqtt
from sentiment.sentiment_analyser import SentimentAnalyser
from sound.music_player import MusicPlayer
from sentiment.average_sentiment import AverageSentiment
from excluded.config import midi_output
from lights.color import Color
import logging
import time

good_notes = [64, 67, 72, 76, 79, 84, 88, 91]
bad_notes = [47, 42, 39, 35, 30, 27, 23, 18, 11]


class MyStreamListener(tweepy.StreamListener):
    DUMMY_MODE = True

    analyser = SentimentAnalyser()
    sentiments = AverageSentiment(5)
    player = MusicPlayer(midi_output)
    mqtt_client = None
    using_mqtt = False
    MQTT_TOPIC = 'lobster/mac/rgb'
    light_number = 0
    LIGHT_COUNT = 4  # TODO: Verify
    SPECIAL_COUNT = 10  # Do something special every x tweets
    special_counter = 0
    first_time = False
    special_light_codes = ['1', '2']

    def on_status(self, status):
        if self.using_mqtt and self.mqtt_client is None:
            # Connect MQTT if not connected
            print("MQTT connecting")
            self.mqtt_client = mqtt.Client()
            self.mqtt_client.connect('192.168.8.104', 1883, 65000)

        logging.info(time.asctime())
        print(f'Tweet: {status.text}')
        if self.DUMMY_MODE:
            sentiment = random.random()
        else:
            sentiment = self.analyser.analyse(status.text)
        logging.info(f'    Sentiment: {sentiment}')
        # note = SentimentSound.map_sentiment_to_note(sentiment)
        if sentiment == 0.5:
            # Azure gives back 0.5 when it doesn't know what to do
            return
        self.sentiments.add(sentiment)
        self.player.sentiment_to_note(self.sentiments.average())

        # send to lights/mqtt
        if self.using_mqtt:
            color = str(Color.sentiment_to_rgb(sentiment))
            color += str(self.light_number).zfill(3)
            self.light_number = (self.light_number + 1) % self.LIGHT_COUNT
            # Color format rrrgggbbbnnn (r=red, g=green, b=blue, n=light number)
            self.mqtt_client.publish('lobster/mac/input', color)
            logging.info(f'    Color: {color}')
            self.special_counter += 1

        # if self.special_counter == self.SPECIAL_COUNT:
            # Do something special every once in a while
            # self.mqtt_client.publish('lobster/mac/input', random.choice(self.special_light_codes))
            # self.special_counter = 0

    def on_error(self, status_code):
        print(f"Error {status_code} occurred")
        if status_code == 420:
            # returning False in on_error disconnects the stream
            return False

        # returning non-False reconnects the stream, with backoff.

    def activate_sentiment(self):
        """
        Turns on real sentiment analysis instead of random number generation
        """
        self.DUMMY_MODE = False

    def activate_lights(self):
        self.using_mqtt = True
