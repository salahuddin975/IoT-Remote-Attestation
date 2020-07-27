#!/bin/bash

clang  -I/usr/include/c++/8 -I/usr/include/arm-linux-gnueabihf/c++/8/ -L /usr/lib/gcc/arm-linux-gnueabihf/8/  src/main.c src/checksum.c -l bcm2835 -fno-stack-protector -z execstack -lpthread -fpermissive -lssl -lcrypto   -o build/SmartHome
