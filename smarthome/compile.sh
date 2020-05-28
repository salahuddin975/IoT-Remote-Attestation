#!/bin/bash

g++ -o SmartHome main.c xxhash.c -l bcm2835 -fno-stack-protector -zexecstack -fpermissive -lpthread
