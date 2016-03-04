import neardal
import threading

import parser
import radio
import spotifyPlayer


class NDEFReader(object):
    """docstring for NDEFReader"""
    def __init__(self, radios):
        super(NDEFReader, self).__init__()
        self.radios = radios
        self.ad = neardal.Adapter()

    def startPolling(self):
        t = threading.Thread(target=self.ad.launch)
        t.start()

        # get original record
        record = ad.get_last_record()

        # wait for a Beam and handle it
        while self.stopped is False:
            while record['type'] != 'URI' and record['type'] != 'SmartCard':
                self.ad.wait_record()
                record = self.ad.get_last_record()

            # add it to the radio and play it
            player = selectParser(record['URI'])
            self.radios[0].addItem(player)
            self.radios[0].playItem(player)

        # stop when asked
        ad.stop()

    def stopPolling(self):
        self.stopped = True
        ad.stop()
