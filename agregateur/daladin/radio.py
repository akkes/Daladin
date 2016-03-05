import soundPlayer
import content


class Radio(object):
    """docstring for Radio"""
    def __init__(self):
        super(Radio, self).__init__()
        self.contents = []
        self.played_content = None

    def addContent(self, new_content):
        if new_content not in self.contents:
            self.contents.append(new_content)

    def playContent(self, item):
        print "play content" + item.name
        soundPlayer.play(item.uri)

    def play(self):
        # TODO: select item based on Djinn predictions
        i = 0
        while i < len(self.contents) and self.played_content is None:
            item = self.contents[i]
            self.playContent(item)
            i += 1

    def stop(self):
        soundPlayer.stop()
        self.played_content = None
