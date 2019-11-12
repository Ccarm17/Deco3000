import mido
import time
import random
import threading
import math
from excluded.config import midi_output

DEFAULT_PORT_NAME = 'Microsoft GS Wavetable Synth 0'  # Use this one for simple sounds


class MusicPlayer:
    # From sad to happy
    note_mapping = [
        ([48], 'sustain'),
        ([51], 'sustain'),
        ([54], 'repeat', 5),  # play for 5 seconds, repeat
        ([52], 'repeat', 5),
        ([55, 56], 'sustain'),
        ([57], 'sustain'),
        ([59, 61, 64], 'repeat', 8),
        ([67, 70, 72], 'repeat', 12),
        ([73, 77, 80], 'sustain'),
        ([81, 85, 88], 'sustain'),
    ]

    all_notes = []

    def __init__(self, port_name=None):
        if port_name is None:
            port_name = DEFAULT_PORT_NAME
        self.outport = mido.open_output(port_name)
        for x in self.note_mapping:
            self.all_notes += x[0]

    def sentiment_to_note(self, sentiment):
        """
        Given a sentiment, plays the appropriate note(s)
        :param sentiment: float [0,1]
        """

        # Convert float to int
        emotion = math.floor(sentiment * 10)
        if emotion == 10:
            emotion = 9

        # Get relevant info out of note_mapping
        note_info = self.note_mapping[emotion]
        notes = note_info[0]
        behaviour = note_info[1]
        note_length = 0
        if len(note_info) > 2:
            note_length = note_info[2]

        # Stop the old notes
        self.stop_all_notes()

        # Play the appropriate note(s)
        for note in notes:
            self.play_note(note)

        print('emotion: ' + str(emotion))

    def play_note(self, note):
        msg = mido.Message('note_on', note=note)
        self.outport.send(msg)

    def play_short_note(self, note, length):
        """
        Play the note for 'length' seconds
        """
        t = threading.Thread(target=self._play_short_note_t, args=(note, length))
        t.start()
        return

    def _play_short_note_t(self, note, length):
        self.play_note(note)
        time.sleep(length)
        self.stop_note(note)
        return

    def stop_note(self, note):
        msg = mido.Message('note_off', note=note)
        self.outport.send(msg)

    def stop_all_notes(self):
        for note in self.all_notes:
            self.stop_note(note)

    def reset(self):
        """ Stops all sounds """
        msg = mido.Message('reset')
        self.outport.send(msg)

    def set_control(self, channel, control, value):
        msg = mido.Message('control_change', channel=channel, control=control, value=value)
        self.outport.send(msg)
        print(f"sent control change to control {control}")


note_mapping = [0, 2, 4, 5, 7, 9, 11]
for i in range(len(note_mapping)):
    for octave in range(1, 8):
        note_mapping.append(note_mapping[i] + octave * 12)

note_mapping = [60, 62, 64, 65, 67, 69, 71, 72]
times = [0.025, 0.05, 0.1, 0.2, 0.4, 0.8]

if __name__ == '__main__':
    mp = MusicPlayer(midi_output)
    print(f'port: {mp.outport}')
    input('press enter to begin')

    while True:
        mp.play_note(random.choice(note_mapping))
        time.sleep(random.choice(times))
