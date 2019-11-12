import paho.mqtt.client as mqtt

SERVER = ('localhost', 1883, 60)
MQTT_SERVER_PORT = 1880
MQTT_TOPIC = 'topic/test'

client = mqtt.Client()

client.connect(SERVER[0], SERVER[1], SERVER[2])
client.publish(MQTT_TOPIC, 'Howdy')
client.disconnect()

