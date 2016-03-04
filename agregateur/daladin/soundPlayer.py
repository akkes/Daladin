import spotify
import constants
import logging
import threading
import time
import re

logging.basicConfig(level=logging.INFO)
end_of_track = threading.Event()
session = None
config = None
should_play = True


def on_end_of_track(session):
    end_of_track.set()


def createSoundPlayer():
    global session
    global config
    if (session is None and config is None):
        # configure spotifySDK
        config = spotify.Config()
        config.user_agent = constants.USER_AGENT
        config.tracefile = b'/tmp/libspotify-trace.log'
        config.load_application_key_file('daladin_spotify.key')
        session = spotify.Session(config)
        loop = spotify.EventLoop(session)
        loop.start()
        audio = spotify.AlsaSink(session)

        # connect
        session.login('alice', 's3cr3tp4ssw0rd', remember_me=True)
        while (session.connection.state != spotify.ConnectionState.
               LOGGED_IN):
            session.process_events()
            time.sleep(1)
            print session.connection.state

        # event handling
        session.on(spotify.SessionEvent.END_OF_TRACK,
                   on_end_of_track)


def checkConnection():
    global session
    if session.connection.state != spotify.ConnectionState.LOGGED_IN:
        session.relogin()


def getTracksContainer(URI):
    global session
    # remove specific user URI prefix if needed
    if re.match("^spotify:user:", URI):
        URI = re.sub("^spotify:user:[A-z]+:collection:",
               "spotify:",
               URI)

    # get object according to URI format
    if re.match("^spotify:playlist:", URI):
        return session.get_playlist(URI)
    elif re.match("^spotify:album:", URI):
        return session.get_album(URI).browse()
    elif re.match("^spotify:artist:", URI):
        return session.get_artist(URI).browse()
    elif re.match("^spotify:track:", URI):
        return {'tracks': array(session.
                                get_track(URI))}
    else:
        print "not a spotify URI, files are not (Yet) supported"


def preload(listing):
    try:
        listing.load()
    except Exception as e:
        print e


def stop():
    global session
    session.player.unload()
    should_play = False


def actualPlay(listing):
    print "actualPlay"
    i = 0
    should_play = True
    print str(i) + " < " + str(len(listing.tracks)) + " and " + str(should_play)
    while (i < len(listing.tracks) and should_play is True):
        track = listing.tracks[i]
        print "load track"
        track.load()
        session.player.load(track)
        print "play track " + track.name
        session.player.play()
        try:
            while not end_of_track.wait():
                pass
        except KeyboardInterrupt:
            pass
        i += 1
    print "end of tracks"


def play(URI):
    print "play"
    global session
    stop()
    checkConnection()

    # get list container
    listing = getTracksContainer(URI)

    # load list
    preload(listing)

    # play it
    print "start playing thread"
    playing_thread = threading.Thread(target=actualPlay, args=(listing,))
    playing_thread.start()
