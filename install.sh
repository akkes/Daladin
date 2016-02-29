#/bin/sh
# install dependencies
wget -q -O - https://apt.mopidy.com/mopidy.gpg | sudo apt-key add -
sudo wget -q -O /etc/apt/sources.list.d/mopidy.list https://apt.mopidy.com/jessie.list
sudo apt-get update
sudo apt-get upgrade
apt-get install vim git htop python-dev python-pip libasound2-dev python-spotify python-bs4 libglib2.0 glib-networking-services libreadline6 libglib2.0-dev glib-networking-services build-essential autoconf libtool libreadline6-dev

pip2 install pyalsaaudio nxppy feedparser

# install nfc
mkdir explorenfc-install
cd explorenfc-install
# instal lib deps
wget http://cache.nxp.com/documents/software/SW282715.zip
unzip SW282715.zip
sudo dpkg -i libneardal0_0.14.2-1_armhf.deb libwiringpi2-2.25-1_armhf.deb neard-explorenfc_0.9-1_armhf.deb

# make libneardal
wget http://cache.nxp.com/documents/software/SW282816.zip
unzip SW282816.zip
tar -zxf libneardal_0.14.2.orig.tar.gz
cd libneardal-0.14.2/
./autogen.sh
make
cd ..

# make neard-explorenfc
tar -xzf neard-explorenfc_0.9.orig.tar.gz
# FIXME: needed?
sudo dpkg -i libneardal-dev_0.14.2-1_armhf.deb
#cd neard-explorenfc-0.9/
#export WIRINGPI_CFLAGS=-I/usr/local/include
#export WIRINGPI_LIBS=-L/usr/local/lib\ -lwiringPi
#./bootstrap
#./configure --disable-dependency-tracking --prefix=/usr --sysconfdir=/etc
#make

# add user
# mkdir /
# useradd [options]  aladin [Options: -c comment -d dir -e date -f days -g group -G groups -k [dir]  -m -M -n -o -p passwd -r -s shell -u uid -D [options] ]
