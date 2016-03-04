#encoding=utf-8
import urllib2
import urllib
import json
from urlparse import urlparse
"""Just a song for now."""
#TODO: Clean the way you access to genres, maybe with a property.
class Content:
    def __init__(self, uri, name): #name to be removed
        self.uri = uri
        self.genres = []
        self.name = 'Help!' #TODO : Spotify API
        self.artist = name #TODO : Spotify API
        self.duration = 2000000000 #TODO : Spotify API
        if urlparse(uri).scheme == "spotify":

            response = urllib2.urlopen("http://developer.echonest.com/api/v4/artist/profile?api_key=ETXPUYCZFFTUMUV0D&name='" + urllib.quote_plus(name) + "'&bucket=genre&format=json").read()
            json_response = json.loads(response)
            if json_response['response']['status']['code'] != 0:
                print(str(json_response['response']['status']['code']) + ": " + json_response['response']['status']['message'])
                self.genres = ['none']
            else:
                for genre in json_response['response']['artist']['genres']:
                    print(genre["name"])
                    self.genres.append(genre["name"])
                print("\n")
    def similarity(self, other):
        score = 0
        if not self.genres or not other.genres: #CETTE ELEGANCELKCEJICEJCICUELIU
            return False
        for genre in self.genres:
            if genre in other.genres:
                score+=1
        return score >= min(len(self.genres), len(other.genres))/2

test = Content("spotify:track:201zQBi6BQfVIpqws3jDXT", "Booba")
test2 = Content("spotify:track:201zQBi6BQfVIpqws3jDXT", "Oxmo Puccino")
print("Score de similarité : " + str(test.similarity(test2)))

sample_uri = "spotify:track:201zQBi6BQfVIpqws3jDXT"
parsed = urlparse(sample_uri)
print( urlparse(sample_uri).scheme)
