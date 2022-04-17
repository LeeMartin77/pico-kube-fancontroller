
typedef void (*fn_gpio_init)(unsigned int);
typedef void (*fn_gpio_set_dir)(unsigned int,char);

typedef struct PinDefinition {
  unsigned int pin_number;
  char in_or_out;
} PinDefinition;

void enable_pins(struct PinDefinition transceiver_pins[], size_t array_size, fn_gpio_init gpio_init, fn_gpio_set_dir gpio_set_dir) {
    for (int i = 0; i < array_size; i++) {
      gpio_init(transceiver_pins[i].pin_number);
      gpio_set_dir(transceiver_pins[i].pin_number, transceiver_pins[i].in_or_out);
    }
}
