#include <Arduino.h>
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <std_msgs/msg/int32.h>

#define RED_PIN PIN_A4    // pin 18
#define YELLOW_PIN PIN_A5 // pin 19
#define GREEN_PIN PIN_A8  // pin 22

enum stack_light_color
{
  red = 1,
  yellow,
  green,
  disable,
};

int *color = nullptr;

rclc_support_t support;
rcl_node_t node;
rcl_subscription_t subscriber;
rclc_executor_t executor;
std_msgs__msg__Int32 i_stack_light_cmd_input;
rcl_allocator_t allocator;

void stack_light_callback(const void *i_stack_light_cmd_input);

void setup()
{
  Serial.begin(115200);
  // USB serial comm setup
  set_microros_serial_transports(Serial);

  // setting pins as output and at logic level low
  pinMode(RED_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);
  pinMode(YELLOW_PIN, OUTPUT);
  digitalWrite(YELLOW_PIN, LOW);
  pinMode(GREEN_PIN, OUTPUT);
  digitalWrite(GREEN_PIN, LOW);

  // memory allocator
  allocator = rcl_get_default_allocator();

  // support with ros engine
  rclc_support_init(&support, 0, NULL, &allocator);

  // node setup
  rclc_node_init_default(&node, "stacklight_node", "", &support);

  rclc_subscription_init_default(&subscriber,
                                 &node,
                                 ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Int32),
                                 "stacklight_topic");

  // main event loop
  rclc_executor_init(&executor,
                     &support.context,
                     1,
                     &allocator);

  // adding the subscription via the executor to get inputs for configuring stack light
  rclc_executor_add_subscription(&executor,
                                 &subscriber,
                                 &i_stack_light_cmd_input,
                                 &stack_light_callback,
                                 ON_NEW_DATA);
}

void loop()
{
  rclc_executor_spin_some(&executor, RCL_MS_TO_NS(10));
}

void stack_light_callback(const void *i_stack_light_cmd_input)
{
  const std_msgs__msg__Int32 *stack_light_cmd_input = (const std_msgs__msg__Int32 *)i_stack_light_cmd_input;
  if (NULL == stack_light_cmd_input)
    return;

  digitalWrite(RED_PIN, LOW);
  digitalWrite(YELLOW_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);

  switch (stack_light_cmd_input->data)
  {
  case stack_light_color::red:
    digitalWrite(RED_PIN, HIGH);
    break;
  case stack_light_color::yellow:
    digitalWrite(YELLOW_PIN, HIGH);
    break;
  case stack_light_color::green:
    digitalWrite(GREEN_PIN, HIGH);
    break;
  case stack_light_color::disable:
    break;
  }
}
