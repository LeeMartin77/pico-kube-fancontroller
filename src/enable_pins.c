
typedef void (*fn_gpio_init)(unsigned int);
typedef void (*fn_gpio_set_dir)(unsigned int,char);

typedef struct PinDefinition {
  unsigned int pin_number;
  char in_or_out;
} PinDefinition;

int _enpin_i = 0;

void enable_pins(struct PinDefinition transceiver_pins[], size_t array_size, fn_gpio_init gpio_init, fn_gpio_set_dir gpio_set_dir) {
    for (_enpin_i = 0; _enpin_i < array_size; _enpin_i++) {
      gpio_init(transceiver_pins[_enpin_i].pin_number);
      gpio_set_dir(transceiver_pins[_enpin_i].pin_number, transceiver_pins[_enpin_i].in_or_out);
    }
}
