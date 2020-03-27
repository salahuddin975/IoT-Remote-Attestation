#!/bin/bash

Attack()
{
	while true
	do

		echo "Please, choose a particular project..."
		echo "	Irrigation Server --> 1"
		echo "	Smart home --> 2"
		echo "	Remote Control Car Monitor --> 3"
		echo "	DNS server --> 4"
		echo "	Select new EXPLOIT type --> 5"

		read input
	
		case $input in
			1)
			        echo "Running Irrigation server..."	
				./IrrigationServer/IrrigationServer $1
				;;
			2) 
		        	echo "Running Smart Home server..."	
				./smarthome/SmartHome $1
				;;
			3) 
		        	echo "Running Remote Control Car Monitor Server..."	
				./remote-control-monitor-car-server/src/CarServer $1
				;;
			4) 
				echo "Running DNS server..."
				./DNS_server_raspberrypi/DNSserver $1
				;;
		
			5)
				return
				;;
			*) 

	                	echo "	Irrigation Server --> 1"
		                echo "	Smart home --> 2"
				echo "	Remote Control Car Monitor --> 3"
				echo "	DNS server --> 4"
				echo "	Select new EXPLOIT type --> 5"
				;;
		esac
	done
}


while true
do
	echo "Please, choose a particular EXPLOIt..."
	echo "	Stack buffer overflow  --> 1"
	echo "	Ret2libc -->2"
	echo "	Heap buffer overflow --> 3"
	echo "	Stack buffer overflow (reverse shell) --> 4"
	echo "	Ret2libc (reverse shell) --> 5"
	echo "	Heap buffer overflow (reverse shell) --> 6"
	echo "	Exit from program --> 7"

	read program
	case $program in
		1)
			Attack 1
			;;
		2)
			Attack 2
			;;
		3)
			Attack 3
			;;
		4)	
			Attack 1
			;;
		5)
			Attack 2
			;;
		6)
			Attack 3
			;;
		7)
			exit 1
			;;
	esac
done


