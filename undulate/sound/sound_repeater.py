from sound.music_player import MusicPlayer
from time import sleep
import random

if __name__ == '__main__':
    m = MusicPlayer('loopMIDI Port 1')

    while True:
        m.stop_note(87)
        m.play_note(75)
        sleep(1)
