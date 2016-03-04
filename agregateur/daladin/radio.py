import abstractPlayer


class radio(object):
    """docstring for radio"""
    def __init__(self):
        super(radio, self).__init__()
        self.items = []
        self.playedItem = None

    def addItem(self, newItem):
        if arg not in self.items:
            self.items.append(newItem)

    def playItem(self, item):
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
