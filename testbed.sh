#!/bin/bash

	
while true
do

	echo "Please, choose a particular project..."
	echo "	Irrigation Server --> 1"
	echo "	Smart home --> 2"
	echo "	Remote Control Car Monitor --> 3"
	
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
		3) 
		        echo "Running Remote Control Car Monitor Server..."	
			./remote-control-monitor-car-server/src/server
			;;
		*) 

	                echo "	Irrigation Server --> 1"
	                echo "	Smart home --> 2"
			echo "	Remote Control Car Monitor --> 3"
			;;
	esac
done

