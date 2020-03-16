#!/bin/bash

	
while true
do

	echo "Please, choose a particular project..."
	echo "	Irrigation Server --> 1"
	echo "	Smart home --> 2"
	
	read input
	
	case $input in
		1)
		        echo "Running Irrigation server..."	
			./IrrigationServer/IrrigationServer
			;;
		2) 
		        echo "Running Smart Home server..."	
			./smarthome/SmartHome
			;;
		*) 

	                echo "	Irrigation Server --> 1"
	                echo "	Smart home --> 2"
			;;
	esac
done

