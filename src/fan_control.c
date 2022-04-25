#include <stdbool.h>

bool fc_last_speed_up = false;
bool fc_last_speed_dn = false;
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

  if (fc_last_speed_up != speed_up_status > 0 && fc_last_speed_up != true) {
      fc_last_speed_up = true;
      if (fc_target_fan_speed < fn_upper_limit) {
          fc_target_fan_speed = fc_target_fan_speed + fc_fan_speed_modifer;
          set_fan_speed(fc_target_fan_speed);
      }
  }

  if (!speed_up_status > 0 && fc_last_speed_up == true) {
      fc_last_speed_up = false;
  }

  if (fc_last_speed_dn != speed_dn_status > 0 && fc_last_speed_dn != true) {
      fc_last_speed_dn = true;
      if (fc_last_speed_dn > fn_lower_limit) {
          fc_target_fan_speed = fc_target_fan_speed - fc_fan_speed_modifer;
          set_fan_speed(fc_target_fan_speed);
      }
  } 
  
  if (!speed_dn_status > 0 && fc_last_speed_dn == true) {
      fc_last_speed_dn = false;
  }
}