Attack()
{
	while true
	do
		echo ""
		echo "Please choose a particular attack..."
        	echo "	Stack overflow --> 1"
        	echo "	Ret2libc --> 2"
        	echo "	Heap overflow --> 3"
		echo "	Stack overflow (reverse shell) --> 4"
		echo "	Ret2libc (reverse shell) --> 5"
		echo "	Heap Overflow (reverse shell) --> 6"
		echo "	Select new server --> 7"

       		read input
        	case $input in
                	1) 
                        	$1 1
                       		;;
               		2) 
                       		$1 2
                       		;;
               		3) 
                       		$1 3
                      		;;
			4)
				$1 1
				;;
			5)
				$1 2
				;;
			6)	
				$1 3
				;;

			7)
				return
				;;
               		*) 
		                echo "Please choose a particular attack..."
                		echo "  Stack overflow --> 1"
		                echo "  Ret2libc --> 2"
        		        echo "  Heap overflow --> 3"
                		echo "  Stack overflow (reverse shell) --> 4"
		                echo "  Ret2libc (reverse shell) --> 5"
        	        	echo "  Heap Overflow (reverse shell) --> 6"
                		echo "  Select new server --> 7"
                       		;;
       		esac
	done
}



while true
do
	echo "Please, choose a particular Server to EXPLOIT..."
        echo "	Irrigation Server --> 1"
        echo "	Smart Home --> 2"
	echo "	Remote Control Car Monitor --> 3"
	echo "	DNS server --> 4"
	echo "	exit from program -> 5"

	read program
	case $program in
		1)
			Attack ./IrrigationServer/IrrigationServer 
			;;

		2)
                	Attack ./smarthome/SmartHome
                	;;
		
		3)
                	Attack ./remote-control-monitor-car-server/src/CarServer
                	;;
		4)
			Attack ./DNS_server_raspberrypi/DNSserver
			;;
		5)
			exit 1
			;;

	esac
done
