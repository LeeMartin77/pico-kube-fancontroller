typedef void (*fn_gpio_put)(unsigned int,char);

typedef void (*fn_sleep_ms)(unsigned int);

void blink(unsigned int pin, unsigned int on_duration, unsigned int off_duration, fn_gpio_put gpio_put, fn_sleep_ms sleep_ms) {
  gpio_put(pin, 1);
  sleep_ms(on_duration);
  gpio_put(pin, 0);
  sleep_ms(off_duration);
}