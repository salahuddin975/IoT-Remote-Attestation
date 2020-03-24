#!/bin/bash

	
while true
do

	echo "Please, choose a particular project..."
	echo "	Irrigation Server --> 1"
	echo "	Smart home --> 2"
	echo "	Remote Control Car Monitor --> 3"
	echo "	DNS server --> 4"

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
		4) 
			echo "Running DNS server..."
			./DNS_server_raspberrypi/DNSserver 5555
			;;
		
		*) 

	                echo "	Irrigation Server --> 1"
	                echo "	Smart home --> 2"
			echo "	Remote Control Car Monitor --> 3"
			echo "	DNS server --> 4"
			;;
	esac
done

