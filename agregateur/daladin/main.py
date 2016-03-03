import sys
import neardal
import threading
from parser import *

ad = neardal.Adapter()


def foo(bar):
    print "foo"
    print ad.get_record(bar)


def main():
    if len(sys.argv) <= 1:
        print "Error: no url given"
        return 1

    # get URI through NFC-NDEF

    t = threading.Thread(target=ad.launch)
    t.start()

    record = ad.get_last_record()
    while record['type'] != 'URI' and record['type'] != 'SmartCard':
        ad.wait_record()
        record = ad.get_last_record()
    ad.stop()

    player = selectParser(record['URI'])
    print "preload"
    player.preload()
    print "play"
    player.play()

if __name__ == '__main__':
    main()
