import sys
import threading

import radio
import ndef
import soundPlayer


def main():
    # Prepare environement
    soundPlayer.createSoundPlayer()
    radios = [radio.Radio()]

    # Setup NDEF listening
    ndef_listener = ndef.NDEFReader(radios)
    ndef_listener.startPolling()

    # setup buttons listening

if __name__ == '__main__':
    main()
