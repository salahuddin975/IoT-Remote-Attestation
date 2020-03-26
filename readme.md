# Remote Attestation Testbed

## About
We build a IoT testbed to experiment with vulnerability exploitation and remote attestation of IoT devices.
There are two primary files: 

1. A testbed server that starts and terminate different IoT projects
2. A Client (Attacker) that exploits particular vulnerable project by sending payload remotely


There are four IoT projects:
1. Irrigation Server
2. Smart Home
3. Remote control monitor car
4. DNS server raspberry

And all these projects can be exploited using:
1. Stack-based buffer overflow (using TCP bind shell)
2. Stack-based buffer overflow (using TCP reverse shell)
3. Heap Overflow (\textbf{not included yet})
4. Ret-to-libc


## Setup
To run the testbed, we need the following requirements:

1. disable ASLR
2. Install necessary libraries for IoT projects
3. compile the main file of the projects enabling execution in stack and disabling stack protector

### Prerequisite
Run the following commands to meet project and environment dependencies:
```sh
chmod +x prerequisite.sh
./prerequisite.sh
```



## Simulation
### Run Server Side
Select any of the included projects after running the server
```
$ ./testbed.sh
```

### Run Client Side
Select particular project and related exploit after running the client
```
$ ./client.sh
```
