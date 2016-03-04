# encoding=utf-8
import urllib2
import urllib
import json
import spotify
import time
name = "Julien Dorké"
response = urllib2.urlopen("http://developer.echonest.com/api/v4/artist/profile?api_key=ETXPUYCZFFTUMUV0D&name='" + urllib.quote_plus(name) + "'&bucket=genre&format=json").read()
jsoned = json.loads(response)
if jsoned['response']['status']['code'] != 0:
    print(str(jsoned['response']['status']['code']) + ": " + jsoned['response']['status']['message'])
else:
    print(jsoned['response']['artist']['genres'])
