import abstractPlayer


class Radio(object):
    """docstring for Radio"""
    def __init__(self):
        super(Radio, self).__init__()
        self.items = []
        self.playedItem = None

    def addItem(self, new_item):
        if new_item not in self.items:
            self.items.append(new_item)

    def playItem(self, item):
        print "play item"
        item.play()

    def play(self):
        # TODO: select item based on Djinn predictions
        for item in self.items:
            self.playItem(item)
            if self.playedItem is None:
                break

    def stop(self):
        self.playedItem.stop()
        self.playedItem = None
