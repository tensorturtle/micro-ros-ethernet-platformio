# micro-ros-ethernet-platformio
An example configuration of micro_ros_platformio over UDP Ethernet

Using Teensy 4.1 and Ubuntu 20.04 host.

This is heavily based on: https://github.com/micro-ROS/micro_ros_platformio

## Network Setup

**Link together a Ubuntu 20.04 host machine and Teensy 4.1**
Usually, directly connecting an Ethernet wire between the two works. If not, use a hub.

**Set manual IP**

Get list of network devices:
```
nmcli con show
```

Note the `DEVICE` for the connection that is shared with the Teensy.
We also need to set an IP address and subnet mask.
```
DEVICE=enp3s0 # or eth0, etc.
IP_ADDRESS=192.168.99.1 # just avoid collision with other interfaces

nmcli dev mod $DEVICE ipv4.address $IP_ADDRESS/$NETMASK
```


## Check Teensy 4.1's Ethernet Connection

Flash [`UDPSendReceiveString.ino`](https://github.com/vjmuzik/NativeEthernet/blob/master/examples/UDPSendReceiveString/UDPSendReceiveString.ino)
Monitor the Teensy's serial port

Simple Python script to send UDP messages to Teensy
```python3
# send UDP Packet
import socket
import time

TEENSY_IP = "192.168.99.2"
TEENSY_PORT = 8888
MESSAGE = b"Message from host to Teensy!"

print("UDP target IP:", TEENSY_IP)
print("UDP target port:", TEENSY_PORT)
print("message:", MESSAGE)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
sock.sendto(MESSAGE, (TEENSY_IP, TEENSY_PORT))
```

## Host Setup

## Teensy 4.1 Setup
