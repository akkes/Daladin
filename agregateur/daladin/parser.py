import requests
import bs4 as BeautifulSoup
import feedparser
import re
import constants
import spotifyPlayer

feedparser.USER_AGENT = constants.USER_AGENT


def isAPodcastFeed(url):
    if len(url) < 257:
        print url
    feed = feedparser.parse(url)
    if 'title' in feed.feed:
        print "rss: " + feed['feed']['title']

    # is the feed a podcast?
    # if there is entries
    if len(feed.entries):
        firstEnclosure = feed.entries[0].enclosures
        # and entries contains enclosures
        if len(firstEnclosure) >= 1:
            print "podcast feed \\o/ yay!"
            print firstEnclosure[0]
            return True
    return False


def findPodcast(url):
    headers = {'user-agent': constants.USER_AGENT}
    r = requests.get(url, headers=headers)
    print "origin url: " + url
    soup = BeautifulSoup.BeautifulSoup(r.text, "lxml")

    if isAPodcastFeed(soup.text):
        return url

    # create set for links in meta
    metaSet = set()

    # search in header
    print len(soup.findAll('link'))
    for link in soup.findAll('link'):
        if link.has_attr('type') and link['type'] == "application/rss+xml":
            metaSet.add(link['href'])

    # Search in metaSet
    for metaUrl in metaSet:
        if isAPodcastFeed(metaUrl):
            return metaUrl

    # create set for page link
    print "links in page"
    pageSet = set()

    # this webpage sucks, we need to search in links
    print len(soup.findAll('a'))
    for a in soup.findAll('a'):
        if a.has_attr('href'):
            print a['href']
            pageSet.add(a['href'])

    pageSet -= metaSet

    for pageUrl in pageSet:
        if isAPodcastFeed(pageUrl):
            return pageUrl

    # this webpage does not link to a podcast, its webdev is lame!
    return False


def selectParser(url):
    # if (re.match("^[http://|https://|www.|open.]*spotify.com", url) or
    #         re.match("^[spotify:]", url)):
    #     pass
    if re.match("^[spotify:]", url):
        return spotifyPlayer.SpotifyPlaylistPlayer(url)
    else:
        return findPodcast(url)
