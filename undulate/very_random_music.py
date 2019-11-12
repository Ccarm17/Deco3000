from sound.music_player import MusicPlayer
from excluded.config import midi_output
import time
import random

root_notes = [0, 2, 4, 5, 7, 9, 11]
notes = []

octaves = 4
for note in root_notes:
    for x in range(octaves):
        notes.append(note + ((x + 4) * 12))

m = MusicPlayer(midi_output)

sounds = ['pling', 'plong', 'twang']

while True:
    m.play_note(random.choice(notes))
    time.sleep(random.random() * 2)
    print(random.choice(sounds))
