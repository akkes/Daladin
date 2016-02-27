#/bin/sh
# install dependencies
apt-get install python-dev python-pip libasound2-dev

pip2 install pyalsaaudio

# install pyspotify
wget -q -O - https://apt.mopidy.com/mopidy.gpg | sudo apt-key add -
sudo wget -q -O /etc/apt/sources.list.d/mopidy.list https://apt.mopidy.com/jessie.list
sudo apt-get update
sudo apt-get install python-spotify

# add user
# mkdir /
# useradd [options]  aladin [Options: -c comment -d dir -e date -f days -g group -G groups -k [dir]  -m -M -n -o -p passwd -r -s shell -u uid -D [options] ]
