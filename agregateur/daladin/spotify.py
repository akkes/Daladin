import spotify
config = spotify.Config()
config.user_agent = 'Daladin Radio '
config.tracefile = b'/tmp/libspotify-trace.log'
config.load_application_key_file('daladin_spotify.key')
session = spotify.Session(config)
audio = spotify.AlsaSink(session)
session.login('alice', 's3cr3tp4ssw0rd')
session.process_events()
session.connection.state
track = session.get_track('spotify:track:2Ld2LehpgQNREMxl9LlIzm')
track.load()
session.player.load(track)
session.player.play()
