from collections import deque
from sound.music_player import MusicPlayer
import logging
from logging import getLogger
from excluded.config import midi_output

class NoteString:
    """
    Stores notes in a string, similar to lights
    When a new note is pushed in, and old one is pushed out
    """

    def __init__(self, max_len, midi_port=midi_output):
        self.mp: MusicPlayer = MusicPlayer(midi_port)
        self.contents: deque = deque(maxlen=max_len)
        for x in range(len(self.contents)):
            self.contents[x] = None
        logging.basicConfig(level=logging.INFO)
        self.logger = getLogger('NoteString')

    def add(self, note: int):
        self.stop_last_note()
        self.contents.appendleft(note)
        self.mp.play_note(note)
        self.logger.info(f'Added note {note}')

    def stop_last_note(self):
        if len(self.contents) == self.contents.maxlen:
            last_note = self.contents[self.contents.maxlen - 1]
            self.mp.stop_note(last_note)
            self.logger.info(f'Removed note {last_note}')
