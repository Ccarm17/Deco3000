import paho.mqtt.client as mqtt

# This is the Publisher

client = mqtt.Client()
print("Made client!")
client.connect('192.168.8.104', 1883, 60)
print("Connected!")
client.publish('lobster/mac/input', "1")
# client.publish('lobster/mac/input', "3")
print("Published!")
client.disconnect()
print("Done!")