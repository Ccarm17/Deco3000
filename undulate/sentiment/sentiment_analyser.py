# https://docs.microsoft.com/en-us/azure/cognitive-services/text-analytics/quickstarts/python-sdk#sentiment-analysis
from azure.cognitiveservices.language.textanalytics import TextAnalyticsClient
from msrest.authentication import CognitiveServicesCredentials
from excluded.config import azure


class SentimentAnalyser:
    id = 0

    def __init__(self):
        subscription_key = azure['api_key_1']
        endpoint = azure['text_analytics_endpoint']
        credentials = CognitiveServicesCredentials(subscription_key)
        self.text_analytics = TextAnalyticsClient(endpoint=endpoint, credentials=credentials)

    def analyse(self, text, id=None, language='en'):
        if id is None:
            id = self.id
            self.id += 1
        doc = {
            'id': id,
            'language': language,
            'text': text
        }
        response = self.text_analytics.sentiment(documents=[doc])
        return response.documents[0].score


