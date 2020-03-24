Attack()
{
	while true
	do
		echo ""
		echo "Please choose a particular attack..."
        	echo "	Stack Overflow --> 1"
        	echo "	Heap Overflow --> 2"
        	echo "	Ret-to-libc --> 3"
		echo "	Select new server --> 4"

       		read input
        	case $input in
                	1) 
                        	$1 localhost 5555
                       		;;
               		2) 
                       		./ho.py
                       		;;
               		3) 
                       		./r2lc.py
                      		;;
			4)
				return
				;;
               		*) 
                       		echo "	Please enter a valid option"
               	        	echo "	Stack Overflow --> 1"
                       		echo "	Heap Overflow --> 2"
                       		echo "	Ret-to-libc --> 3"
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

	read program
	case $program in
		1)
			Attack ./IrrigationServer/so.py 
			;;

		2)
                	Attack ./smarthome/so.py
                	;;
		
		3)
                	Attack ./remote-control-monitor-car-server/src/so.py
                	;;
		4)
			Attack ./DNS_server_raspberrypi/so.py
			;;

	esac
done
