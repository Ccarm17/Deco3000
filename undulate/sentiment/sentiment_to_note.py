from math import floor
from sound.music_player import MusicPlayer


class SentimentToNote:
    # From sad to happy
    notes = [
        ([48], 'sustain'),
        ([51], 'sustain'),
        ([54], 'repeat', 5),  # play for 5 seconds, repeat
        ([52], 'repeat', 5),
        ([55, 56], 'sustain'),
        ([57], 'sustain'),
        ([59, 61, 64], 'repeat', 8),
        ([67, 71, 73], 'repeat', 12),
        ([74, 78, 81], 'sustain'),
        ([82, 86, 89], 'sustain'),
    ]

    player_options = {
        'sustain': MusicPlayer.play_note,
        'repeat': MusicPlayer.play_short_note
    }

    @staticmethod
    def map(sentiment):
        # Sentiments are in range [0,1]
        mapped = floor(sentiment * 10)
        if mapped == 10:
            mapped = 9
        notes = SentimentToNote.notes[mapped][0]

        return notes
