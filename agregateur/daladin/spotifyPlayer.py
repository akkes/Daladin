import threading
import spotify
import time
import logging
import re
import abstractPlayer
logging.basicConfig(level=logging.INFO)
end_of_track = threading.Event()


def on_end_of_track(self):
    end_of_track.set()


class SpotifyPlayer(abstractPlayer):
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
        while
        self.session.connection.state != spotify.ConnectionState.LOGGED_IN:
            self.session.process_events()
            time.sleep(1)
            print self.session.connection.state
        if re.match("^[spotify:playlist:]", self.spotifyURI):
            self.listing = self.session.get_playlist(self.spotifyURI)
        else if re.match("^[spotify:album:]", self.spotifyURI):
            self.listing = self.session.get_album(self.spotifyURI).browse()
        else if re.match("^[spotify:artist:]", self.spotifyURI):
            self.listing = self.session.get_album(self.spotifyURI).browse()
        else if re.match("^[spotify:track:]", self.spotifyURI):
            self.listing = {'tracks': array(self.
                            session.get_track(self.spotifyURI))}

    def preload(self):
        try:
            self.listing.load()
        except Exception as e:
            print e

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

    def stop(self):
        self.session.player.pause()
