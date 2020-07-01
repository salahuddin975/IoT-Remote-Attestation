#!/bin/sh

# Compile Irrigation Server
echo "Installing prerequisite for Irrigation server"
sudo apt-get install wiringpi
sudo apt-get install libssl-dev
echo "Compiling Irrigation Server"
cd IrrigationServer
make clean; make
chmod +x so.py ret_2_libc.py ho.py so_rev.py rev_ret2libc.py  ho_rev.py 
cd ..

# install Smart Home
echo "Compiling Smart Home"
cd smarthome
./compile.sh
chmod +x so.py ret_2_libc.py ho.py so_rev.py rev_ret2libc.py  ho_rev.py
cd ..

# install Remote Control Monitor Car Server
sudo apt-get install libopencv-dev
cd remote-control-monitor-car-server/src/
make clean; make
chmod +x so.py ret_2_libc.py ho.py so_rev.py rev_ret2libc.py  ho_rev.py
cd ../..

#install DNS server
cd DNS_server_raspberrypi
make clean; make
chmod +x so.py ret_2_libc.py ho.py so_rev.py rev_ret2libc.py  ho_rev.py
cd ..

