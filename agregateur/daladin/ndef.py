import neardal
import threading

from parser import *
import radio
import spotifyPlayer


class NDEFReader(object):
    """docstring for NDEFReader"""
    def __init__(self, radios):
        super(NDEFReader, self).__init__()
        self.radios = radios
        self.ad = neardal.Adapter()
        self.stopped = False

    def startPolling(self):
        t = threading.Thread(target=self.ad.launch)
        t.start()

        # get original record
        original_record = self.ad.get_last_record()

        # wait for a Beam and handle it
        while self.stopped is False:
            record = original_record
            while record['type'] != 'URI' and record['type'] != 'SmartCard':
                self.ad.wait_record()
                record = self.ad.get_last_record()

            # add it to the radio and play it
            print "detected URI:" + record['URI']
            player = selectParser(record['URI'])
            self.radios[0].addItem(player)
            self.radios[0].playItem(player)
            record = None

        # stop when asked
        self.ad.stop()

    def stopPolling(self):
        self.stopped = True
        self.ad.stop()
