cd /home/pi/
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.37.tar.gz
tar zxvf bcm2835-1.37.tar.gz
cd bcm2835-1.37
./configure
make
sudo make check
sudo make install




