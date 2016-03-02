import spotifyPlayer


class SpotifyPlaylistPlayer(spotifyPlayer.SpotifyPlayer):
    """docstring for SpotifyPlaylistPlayer"""
    def __init__(self, arg):
        super(SpotifyPlaylistPlayer, self).__init__(arg)
        self.playlist = self.session.get_playlist(self.spotifyURI)

    def preload(self):
        self.playlist.load()

    def play(self):
        self.listing = self.playlist
        super(SpotifyPlaylistPlayer, self).play()
