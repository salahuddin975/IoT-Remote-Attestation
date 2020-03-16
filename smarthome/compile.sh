#!/bin/bash

gcc -o main main.c -l bcm2835
sudo ./main
sudo ./off
