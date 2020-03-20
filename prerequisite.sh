#!/bin/sh

# Compile Irrigation Server
echo "Compiling Irrigation Server"
cd IrrigationServer
gcc -o IrrigationServer server.cpp -fno-stack-protector -z execstack
chmod +x so.py ho.py
cd ..

# install Smart Home
echo "Compiling Smart Home"
cd smarthome
gcc -o SmartHome main.c -l bcm2835 -fno-stack-protector -z execstack
chmod +x so.py ho.py
cd ..

# install Remote Control Monitor Car Server
sudo apt-get install libopencv-dev
cd remote-control-monitor-car-server/src/
make
chmod +x so.py ho.py
cd ../..
