#!/bin/bash

gcc  -I/usr/include/c++/8 src/main.c src/checksum.c -lbcm2835 -fno-stack-protector -z execstack -lpthread -fpermissive -lssl -lcrypto -o Smarthome-gcc
