import neardal
import threading
ad = neardal.Adapter()
def foo(bar):
    print "foo"
    print ad.get_record(bar)

t = threading.Thread(target=ad.launch)
t.start()

ad.get_last_record()
