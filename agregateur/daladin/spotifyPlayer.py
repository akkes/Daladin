import threading
import spotify
import time
import logging
logging.basicConfig(level=logging.INFO)
end_of_track = threading.Event()

def on_end_of_track(self):
    end_of_track.set()


class SpotifyPlayer(object):
    """docstring for SpotifyPlayer"""
    def __init__(self, arg):
        super(SpotifyPlayer, self).__init__()
        self.spotifyURI = arg
        self.config = spotify.Config()
        self.config.user_agent = 'Daladin Radio '
        self.config.tracefile = b'/tmp/libspotify-trace.log'
        self.config.load_application_key_file('daladin_spotify.key')
        self.session = spotify.Session(self.config)
        loop = spotify.EventLoop(self.session)
        loop.start()
        audio = spotify.AlsaSink(self.session)
        self.session.login('alice', 's3cr3tp4ssw0rd')
        while self.session.connection.state != spotify.ConnectionState.LOGGED_IN:
            self.session.process_events()
            time.sleep(1)
            print self.session.connection.state

    def preload(self):
        pass

    def play(self):
        self.session.on(spotify.SessionEvent.END_OF_TRACK, on_end_of_track)
        for track in self.listing.tracks:
            print "load track"
            track.load()
            self.session.player.load(track)
            print "play track"
            self.session.player.play()
            try:
                while not end_of_track.wait(0.1):
                    pass
            except KeyboardInterrupt:
                pass
