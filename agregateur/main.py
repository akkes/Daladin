import requests
import bs4 as BeautifulSoup
import feedparser
import sys


def isAPodcastFeed(url):
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
    r = requests.get(url)
    soup = BeautifulSoup.BeautifulSoup(r.text, "lxml")
    print "origin url: " + url
    if isAPodcastFeed(soup.text):
        return url

    # search in header
    print len(soup.findAll('link'))
    for link in soup.findAll('link'):
        if link.has_attr('type') and link['type'] == "application/rss+xml":
            if isAPodcastFeed(link['href']):
                return link['href']

    # this webpage sucks, we need to search in links
    print len(soup.findAll('a'))
    for a in soup.findAll('a'):
        if a.has_attr('href'):
            if isAPodcastFeed(a['href']):
                return link['href']

    # this webpage does not link to a podcast, its webdev is lame!
    return False


def main():
    if len(sys.argv) <= 1:
        print "Error: no url given"
        return 1

    print findPodcast(sys.argv[1])

if __name__ == '__main__':
    main()
