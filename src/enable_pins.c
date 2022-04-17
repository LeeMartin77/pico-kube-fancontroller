
typedef void (*fn_gpio_init)(unsigned int);
typedef void (*fn_gpio_set_dir)(unsigned int,char);

void enable_pins(unsigned int transceiver_pin, fn_gpio_init gpio_init, fn_gpio_set_dir gpio_set_dir) {
    gpio_init(transceiver_pin);
    // GPIO_OUT unpacks to 1
    gpio_set_dir(transceiver_pin, 1);
}
