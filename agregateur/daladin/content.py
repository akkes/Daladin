# encoding=utf-8
import urllib2
import urllib
import json
import re
from urlparse import urlparse
import soundPlayer
# TODO: Clean the way you access to genres, maybe with a property.


class Content:
    def __init__(self, uri):  # name to be removed
        # init properties
        self.uri = uri
        self.name = ""
        self.artist = ""
        self.duration = 0  # TODO : Spotify API

        # clean URI
        uri = soundPlayer.cleanURI(uri)
        print "cleaned uri: " + uri

        # get properties
        try:
            if re.match("^spotify:playlist:", uri):
                playlist = soundPlayer.session.get_playlist(uri).load()
                self.name = playlist.name
            elif re.match("^spotify:album:", uri):
                album = soundPlayer.session.get_album(uri).load()
                self.name = album.name
                self.artist = album.artist.load().name
            elif re.match("^spotify:artist:", uri):
                artist = soundPlayer.session.get_artist(uri).browse.load()
                self.artist = artist.load().name
                self.name = self.artist
            elif re.match("^spotify:track:", URI):
                track = soundPlayer.session.get_track(uri).load()
                self.artist = track.artists
                self.name = track.name

            # get genres
            self.genres = []
            if urlparse(uri).scheme == "spotify":
                response = urllib2.urlopen("http://developer.echonest.com" +
                                           "/api/v4/artist/profile" +
                                           "?api_key=ETXPUYCZFFTUMUV0D&name='" +
                                           urllib.quote_plus(self.artist) +
                                           "'&bucket=genre&format=json").read()
                json_response = json.loads(response)
                if json_response['response']['status']['code'] != 0:
                    print(str(json_response['response']['status']['code']) +
                          ": " + json_response['response']['status']['message'])
                    self.genres = [None]
                else:
                    for genre in json_response['response']['artist']['genres']:
                        print(genre["name"])
                        self.genres.append(genre["name"])
                    print("\n")
        except soundPlayer.spotify.error.Timeout:
            print "spotify is too slow!"

    def similarity(self, other):
        score = 0
        if (not self.genres or not other.genres):  # CETTE ELEGANCEKCEJCECUELIU
            return False
        for genre in self.genres:
            if genre in other.genres:
                score += 1
        return score >= min(len(self.genres), len(other.genres))/2
