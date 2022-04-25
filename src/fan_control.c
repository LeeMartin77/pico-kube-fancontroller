#include <stdbool.h>

unsigned int fc_last_speed_up = 0;
unsigned int fc_last_speed_dn = 0;
unsigned int fc_target_fan_speed = 50;

typedef unsigned int (*fc_gpio_get)(unsigned int);
typedef void (*fc_set_fan_speed_callback)(unsigned int);

void el_fan_control(unsigned int fc_fan_speed_modifer, unsigned int speed_up_gpio_pin, unsigned int speed_down_gpio_pin, fc_gpio_get gpio_get, fc_set_fan_speed_callback set_fan_speed) {
  if (fc_last_speed_up != gpio_get(speed_up_gpio_pin) > 0 && fc_last_speed_up != true) {
      fc_last_speed_up = true;
      if (fc_target_fan_speed < 100) {
          fc_target_fan_speed = fc_target_fan_speed + fc_fan_speed_modifer;
          set_fan_speed(fc_target_fan_speed);
      }
  } else {
      fc_last_speed_up = false;
  }

  if (fc_last_speed_dn != gpio_get(speed_down_gpio_pin) > 0 && fc_last_speed_dn != true) {
      fc_last_speed_dn = true;
      if (fc_last_speed_dn > 0) {
          fc_target_fan_speed = fc_target_fan_speed - fc_fan_speed_modifer;
          set_fan_speed(fc_target_fan_speed);
      }
  } else {
      fc_last_speed_dn = false;
  }
}