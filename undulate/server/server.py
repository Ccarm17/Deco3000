from flask import Flask
import main
import threading

app = Flask(__name__)

def streamer_thread():
    main.run()

@app.route('/')
def hello_world():
    listener = main.run()

    return 'HELLO WORLD'

if __name__ == '__main__':
    app.run()