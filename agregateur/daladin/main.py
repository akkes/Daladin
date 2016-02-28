import sys
from parser import *


def main():
    if len(sys.argv) <= 1:
        print "Error: no url given"
        return 1

    print selectParser(sys.argv[1])

if __name__ == '__main__':
    main()
