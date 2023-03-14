# micro-ros-ethernet-platformio
An example configuration of micro_ros_platformio over UDP Ethernet

Using Teensy 4.1 and Ubuntu 20.04 host.

This is heavily based on: https://github.com/micro-ROS/micro_ros_platformio

## Check Teensy 4.1's Ethernet Connection

Flash [`UDPSendReceiveString.ino`](https://github.com/vjmuzik/NativeEthernet/blob/master/examples/UDPSendReceiveString/UDPSendReceiveString.ino)
Monitor the Teensy's serial port

```python3
# send UDP Packet
import socket
import time

UDP_IP = "192.168.99.2"
UDP_PORT = 8888
MESSAGE = b"Hello, World!"

print("UDP target IP:", UDP_IP)
print("UDP target port:", UDP_PORT)
print("message:", MESSAGE)

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) # UDP
sock.sendto(MESSAGE, (UDP_IP, UDP_PORT))
```

## Host Setup

## Teensy 4.1 Setup
