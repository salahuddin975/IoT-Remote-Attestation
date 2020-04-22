# Remote Attestation Testbed

## About
We build a IoT testbed to experiment with vulnerability exploitation and remote attestation of IoT devices.
Right now, we are working on Raspberry Pi model 3 B+ (OS: Raspbian Buster)
There are two primary files: 

1. A testbed server that starts and terminate different IoT projects
2. A Client (Attacker) that exploits particular vulnerable project by sending payload remotely


There are four IoT projects:
1. Irrigation Server
2. Smart Home
3. Remote control monitor car
4. DNS server raspberry

And all these projects can be exploited using:
1. Stack-based buffer overflow (TCP bind shell)
2. Stack-based buffer overflow (TCP reverse shell)
3. Heap Overflow (TCP bind shell)
4. Heap Overflow (TCP reverse shell)
5. Ret-to-libc (TCP bind shell)
6. Ret-to-libc (TCP revese shell)
7. Injecting Code using Mprotect()


## Setup
To run the testbed, we need the following requirements:

1. disable ASLR
2. Install necessary libraries for IoT projects
3. compile the main file of the projects enabling execution in stack and disabling stack protector

### Prerequisite
Run the following commands to meet project and environment dependencies:
```sh
chmod +x compile.sh
./compile.sh
```



## Simulation
### Primary Interface
You can download the `interface.sh` file in any Linux machine and run it to access both the server and the client.
Note: You must have the `University of Houston` VPN installed in your computer.
```
user@local:~$ ./interface.sh
```
It will prompt users to start the server, client, and the verifier.

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
