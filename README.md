# micro-ros-ethernet-platformio
An example configuration of micro_ros_platformio over UDP Ethernet

Using Teensy 4.1 and Ubuntu 20.04 host.

This is heavily based on: https://github.com/micro-ROS/micro_ros_platformio

## Network Setup

**Link together a Ubuntu 20.04 host machine and Teensy 4.1**

Usually, directly connecting an Ethernet wire between the two works. If not, use a hub.

**Set manual IP**

In this example, we use the following addresses:

IP | Device
--- | ---
Host | 192.168.99.1
Teensy 4.1 | 192.168.99.2

Get list of network devices:
```
nmcli con show
```

Note the `CONN` for the connection that is shared with the Teensy.
We also need to set an IP address and subnet mask.
```
CONN=eth0
IP_ADDRESS=192.168.99.1 # just avoid collision with other interfaces
NETMASK=24

nmcli con mod $CONN ipv4.address $IP_ADDRESS/$NETMASK ipv4.method manual ipv6.method ignore connection.autoconnect yes
nmcli con up $CONN
```


## (Optional) Check Teensy 4.1's Ethernet Connection

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

## Main Setup

Following https://github.com/micro-ROS/micro_ros_platformio:

**Install VS Code**: https://code.visualstudio.com/download

**Install PlatformIO plugin for VSCode**: https://platformio.org/install/ide?install=vscode

**Install apt dependencies**:
```
apt install -y git cmake python3-pip
```

**Clone this repository**
```
git clone git@github.com:tensorturtle/micro-ros-ethernet-platformio.git
```

**Open, Build, and Flash the Teensy Program**

**From the PlatformIO Home within VS Code, open `MicroRosEthernetDemo` directory, which is a PlatformIO Project.**

**Open a terminal in the same directory**

**Connect the Teensy 4.1 to the host via USB**

**Build and flash the project**

```
pio lib install # Install dependencies
pio run # Build the firmware
pio run --target upload # Flash the firmware
```

## Run microROS Agent on Host

On the host,
```
docker run -it --rm -v /dev:/dev -v /dev/shm:/dev/shm --privileged --net=host microros/micro-ros-agent:humble udp4 --port 8888 -v6\
```

## View ROS2 messages being published
```
docker run -it --rm --network host osrf/ros:humble-desktop-full
```
If host is a NVIDIA Jetson, find appropriate docker image from https://github.com/dusty-nv/jetson-containers
```
docker run -it --rm --network host dustynv/ros:humble-ros-base-l4t-r35.2.1
```
and run:
```
ros2 topic list
``` 
to find `/micro_ros_platformio_node_publisher`. See the example message:
```
ros2 topic echo /micro_ros_platformio_node_publisher
```

