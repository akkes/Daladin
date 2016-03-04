import spotify
import constants

logging.basicConfig(level=logging.INFO)
end_of_track = threading.Event()
session = None
config = None
should_play = True


def on_end_of_track(self):
    end_of_track.set()


def createSoundPlayer(self, arg):
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
                   def on_end_of_track(self):
                       end_of_track.set())


def checkConnection():
    if session.connection.state != spotify.ConnectionState.LOGGED_IN:
        session.relogin()


def getTracksContainer(URI):
    if re.match("^spotify:playlist:", self.spotifyURI):
        return session.get_playlist(self.spotifyURI)
    elif re.match("^spotify:album:", self.spotifyURI):
        return session.get_album(self.spotifyURI).browse()
    elif re.match("^spotify:artist:", self.spotifyURI):
        return session.get_artist(self.spotifyURI).browse()
    elif re.match("^spotify:track:", self.spotifyURI):
        return {'tracks': array(session.
                                get_track(self.spotifyURI))}
    else:
        print "not a spotify URI, files are not (Yet) supported"


def preload(listing):
    try:
        listing.load()
    except Exception as e:
        print e


def stop():
    session.player.unload()
    should_play = False


def play(URI):
    stop()
    checkConnection()

    # get list container
    listing = getTracksContainer(URI)

    # load list
    preload(listing)

    # play it
    i = 0
    should_play = True
    while (i < len(listing.tracks) and should_play is True):
        track = listing[i]
        print "load track"
        track.load()
        session.player.load(track)
        print "play track" + track.name
        session.player.play()
        try:
            while not end_of_track.wait(0.1):
                pass
        except KeyboardInterrupt:
            pass
