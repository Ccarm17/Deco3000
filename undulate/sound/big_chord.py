from sound.music_player import MusicPlayer
import random
from time import sleep


class BigChord:
    """Adds good and bad notes to form a big chord with good or bad"""

    good_notes = [64, 67, 72, 76, 79, 84, 88, 91]
    bad_notes = [47, 42, 39, 35, 30, 27, 23, 18, 11]

    def __init__(self, port='loopMIDI Port 1'):
        self.mp = MusicPlayer(port)
        self.current_notes = []
        self.good_count = 0
        self.bad_count = 0
        self.mp.play_note(60)
        self.mp.play_note(72)
        self.mp.play_note(48)

    def add_good(self):
        print("Good note")
        if self.bad_count > 0:
            self.mp.stop_note(self.current_notes.pop())
            self.bad_count -= 1
            return
        new_note = random.choice(self.good_notes)
        self.current_notes.append(new_note)
        self.good_count += 1
        self.mp.play_note(new_note)

    def add_bad(self):
        print("Bad note")
        if self.good_count > 0:
            self.mp.stop_note(self.current_notes.pop())
            self.good_count -= 1
            return
        new_note = random.choice(self.bad_notes)
        self.current_notes.append(new_note)
        self.bad_count += 1
        self.mp.play_note(new_note)


if __name__ == '__main__':
    # Dummy data example of the big_chord model
    bc = BigChord('loopMIDI Port 1')
    sleep(2.5)
    while True:
        note = random.random()
        if note > 0.75:
            bc.add_good()
        else:
            bc.add_bad()
        sleep(random.random() * 2)
