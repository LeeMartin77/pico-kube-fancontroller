// We actually get two per revolution. Worth remembering.
// We will keep a count per GPIO. Might eat some memory but it's relatively safe.
int _fanrevcounts [28];

// Redeclaring for purity
const int fm_GPIO_IRQ_EDGE_RISE = 8;

typedef void (* fm_gpio_init)(unsigned int);

typedef void (* fm_gpio_acknowledge_irq)(unsigned int gpio, unsigned int events);

typedef void (* fm_gpio_irq_callback_t )(unsigned int gpio, unsigned int events);

typedef void (* fm_gpio_set_irq_enabled_with_callback)(unsigned int gpio, unsigned int events, char enabled, fm_gpio_irq_callback_t callback);

fm_gpio_acknowledge_irq _fm_gpio_acknowledge_irq;

void _pulse_callback(unsigned int gpio, unsigned int event) {
  _fanrevcounts[gpio]++;
  _fm_gpio_acknowledge_irq(gpio, event);
}

int get_fan_revolutions_since_reset(int gpio) {
  return _fanrevcounts[gpio];
} 

void setup_fan_monitor_pin(int gpio, fm_gpio_init gpio_init, fm_gpio_set_irq_enabled_with_callback gpio_set_irq_enabled_with_callback, fm_gpio_acknowledge_irq dep_gpio_acknowledge_irq) {
  _fanrevcounts[gpio] = 0;
  gpio_init(gpio);
  gpio_set_irq_enabled_with_callback(gpio, fm_GPIO_IRQ_EDGE_RISE, 1, _pulse_callback);
  // We might end up re-enabling this over and over, but this does for now
  _fm_gpio_acknowledge_irq = dep_gpio_acknowledge_irq;
}

void reset_revolutions(int gpio) {
  _fanrevcounts[gpio] = 0;
}
