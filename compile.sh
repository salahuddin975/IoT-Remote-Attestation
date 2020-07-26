#!/bin/sh

#install llvm
sudo apt install cmake
./install_llvm.sh

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
echo "Installing prerequisite for Smarthome server"
cd smarthome
./install-bcm2835.sh
echo "Compiling Smart Home"
./compile.sh
chmod +x so.py ret_2_libc.py ho.py so_rev.py rev_ret2libc.py  ho_rev.py
cd ..

#install DNS server
cd DNS_server_raspberrypi
make clean; make
chmod +x so.py ret_2_libc.py ho.py so_rev.py rev_ret2libc.py  ho_rev.py
cd ..

#Compile PICON
cd picon/
./compile.sh

