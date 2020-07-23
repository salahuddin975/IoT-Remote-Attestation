#!/bin/bash

clang++  -I/usr/include/c++/8 -I/usr/include/arm-linux-gnueabihf/c++/8/ -L /usr/lib/gcc/arm-linux-gnueabihf/8/  main.cpp checksum.cpp -l bcm2835 -fno-stack-protector -z execstack -lpthread -fpermissive -lssl -lcrypto   -o SmartHome
