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
	
	read program
	case $program in
		1)
			Attack ./IrrigationServer/so.py 
			;;

		2)
                	Attack ./smarthome/so.py
                	;;

	esac
done
