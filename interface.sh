#!/bin/bash

echo "################################################"
echo "           Remote Attestation Testbed           "
echo "         RNS Lab, University of Houston         "
echo "################################################"

echo "There are three individual devices."
echo "	1. Server that simulates Projects"
echo "	2. Client that simulates Attacker"
echo "	3. Verifier that verifies integrity"
echo " "

echo "Now follow instructions to launch the scripts:"
echo "Press 'y' to say- yes, other- no"

echo " "
echo "Do you want to launch the server script?"
read s
if [[ "$s" == 'y' ]];then
	echo "Launching Server in new terminal....."
	gnome-terminal -- sh -c 'ssh -p 3385 pi@129.7.241.72 /home/pi/IoT-Remote-Attestation/testbed.sh; bash'
fi

echo " "
echo "Do you want to launch the client script?"
read c
if [[ "$c" == 'y' ]];then
	echo "Launching Client in new terminal....."
	gnome-terminal -- sh -c 'ssh -p 3390 pi@129.7.241.72 /home/pi/IoT-Remote-Attestation/client.sh; bash'
fi

echo " "
echo "Do you want to launch the verifier script?"
read v
if [[ "$v" == 'y' ]];then
	echo "Launching Verifier in a new terminal....."
	gnome-terminal -- sh -c 'ssh -p 3395 pi@129.7.241.72 /home/pi/IoT-Remote-Attestation/verifier.sh; bash'
fi
