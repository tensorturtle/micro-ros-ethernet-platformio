#include <Arduino.h>
#include <micro_ros_platformio.h>

#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>

#include <std_msgs/msg/int32.h>
#include <std_msgs/msg/float32.h>

rcl_publisher_t publisher;
rcl_publisher_t float_publisher;
std_msgs__msg__Int32 msg;
std_msgs__msg__Float32 float_msg;

rclc_executor_t executor;
rclc_support_t support;
rcl_allocator_t allocator;
rcl_node_t node;
rcl_timer_t timer;

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){error_loop();}}
#define RCSOFTCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){}}

int DIGITAL_READ_PIN_0=23;
int ANALOG_READ_PIN_0=22;

// Error handle loop
void error_loop() {
  while(1) {
    delay(100);
  }
}

void timer_callback(rcl_timer_t * timer, int64_t last_call_time) {
  RCLC_UNUSED(last_call_time);
  if (timer != NULL) {
    msg.data = digitalRead(DIGITAL_READ_PIN_0);
    float_msg.data = analogRead(ANALOG_READ_PIN_0);

    RCSOFTCHECK(rcl_publish(&publisher, &msg, NULL));
    //msg.data++;

    RCSOFTCHECK(rcl_publish(&float_publisher, &float_msg, NULL));
    //float_msg.data += 0.1;
  }
}

byte local_mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress local_ip(192, 168, 99, 2);
IPAddress agent_ip(192, 168, 99, 1);
unsigned int agentPort = 8888;  


void setup() {
  // Pin setup
  pinMode(DIGITAL_READ_PIN_0, INPUT);
  pinMode(ANALOG_READ_PIN_0, INPUT);

  // Init ROS2
  set_microros_native_ethernet_transports(local_mac, local_ip, agent_ip, agentPort);
  delay(2000);

  allocator = rcl_get_default_allocator();

  //create init_options
  RCCHECK(rclc_support_init(&support, 0, NULL, &allocator));

  // create node
  RCCHECK(rclc_node_init_default(&node, "micro_ros_platformio_node", "", &support));

  // create publisher
  RCCHECK(rclc_publisher_init_default(
    &publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
    "micro_ros_platformio_node_publisher"));

  // create timer,
  RCCHECK(rclc_publisher_init_default(
    &float_publisher,
    &node,
    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
    "micro_ros_platformio_node_float_publisher"));

  const unsigned int timer_timeout = 10;
  RCCHECK(rclc_timer_init_default(
    &timer,
    &support,
    RCL_MS_TO_NS(timer_timeout),
    timer_callback));

  // create executor
  RCCHECK(rclc_executor_init(&executor, &support.context, 1, &allocator));
  RCCHECK(rclc_executor_add_timer(&executor, &timer));

  msg.data = 0;
  float_msg.data = 0.0;
}

void loop() {
  delay(10);
  RCSOFTCHECK(rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10)));
}
