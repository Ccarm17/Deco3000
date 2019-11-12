from tweet_stream import MyStreamListener
from excluded import config
import tweepy
import logging


def run():
    logging.basicConfig(filename='log_tweet_stream.log', level=logging.DEBUG)
    consumer_key = config.config['api_key']
    consumer_secret = config.config['api_secret_key']
    access_token = config.config['access_token']
    access_token_secret = config.config['access_token_secret']

    auth = tweepy.OAuthHandler(consumer_key, consumer_secret)
    auth.set_access_token(access_token, access_token_secret)
    api = tweepy.API(auth)

    # Some of these are to test larger inputs
    hashtags = ['#climatechange', '#uq', '#uqbloom', '#uniofqld', '#uqlobster', '#uqjacarandalane', '#binchicken', '#uqgraduation',
                '#uqlife', '#stlucia', '#uqmoments', '#owntheunknown', '#uqpurple', '#createchange', '#uqwomen',
                '#uqlife', '#uqstudentlife', '#UQresearchweek2019']

    myStreamListener = MyStreamListener()

    # Turn on to get real sentiment analysis, not dummy numbers
    myStreamListener.activate_sentiment()

    # Turn this on to get lights working, if you've got the MQTT set up
    myStreamListener.activate_lights()

    myStream = tweepy.Stream(auth=api.auth, listener=myStreamListener)

    myStream.filter(track=hashtags, is_async=True)

    return myStreamListener


if __name__ == '__main__':
    run()
