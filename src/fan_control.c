#include <stdbool.h>

bool fc_button_held = false;
unsigned int fc_target_fan_speed = 50;

const unsigned int fn_upper_limit = 100;
const unsigned int fn_lower_limit = 0;

typedef unsigned int (*fc_gpio_get)(unsigned int);
typedef void (*fc_set_fan_speed_callback)(unsigned int);

void el_fan_control(unsigned int fc_fan_speed_modifer, unsigned int speed_up_gpio_pin, unsigned int speed_down_gpio_pin, fc_gpio_get gpio_get, fc_set_fan_speed_callback set_fan_speed) {
  const int speed_up_status = gpio_get(speed_up_gpio_pin);
  const int speed_dn_status = gpio_get(speed_down_gpio_pin);
  
  if (speed_up_status > 0 && speed_dn_status > 0) {
    return;
  }

  if (fc_button_held != true && (fc_button_held != speed_up_status > 0 || fc_button_held != speed_dn_status > 0)) {
      fc_button_held = true;
      if (speed_up_status > 0 && fc_target_fan_speed < fn_upper_limit) {
        fc_target_fan_speed = fc_target_fan_speed + fc_fan_speed_modifer;
        set_fan_speed(fc_target_fan_speed);
      }
      if (speed_dn_status > 0 && fc_target_fan_speed > fn_lower_limit) {
        fc_target_fan_speed = fc_target_fan_speed - fc_fan_speed_modifer;
        set_fan_speed(fc_target_fan_speed);
      }
  }

  fc_button_held = speed_up_status > 0 || speed_dn_status > 0;
}