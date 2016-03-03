import spotifyPlayer


class SpotifyPlaylistPlayer(spotifyPlayer.SpotifyPlayer):
    """docstring for SpotifyPlaylistPlayer"""
    def __init__(self, arg):
        super(SpotifyPlaylistPlayer, self).__init__(arg)
        self.playlist = self.session.get_playlist(self.spotifyURI)

    def preload(self):
	super(SpotifyPlaylistPlayer, self).preload()
        try:
            self.playlist.load()
        except Exception as e:
            print e

    def play(self):
        self.listing = self.playlist
        super(SpotifyPlaylistPlayer, self).play()
