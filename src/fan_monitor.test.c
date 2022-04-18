#include <assert.h>
#include <stdio.h>
#include "fan_monitor.c"

char fm_call_sequence [1000];
int fm_callcounts [6];

typedef struct {
  int pin_number;
  int event;
  char enabled;
  fm_gpio_irq_callback_t callback;
} fmgpioIrqEnabledArgs;

typedef struct {
  int pin_number;
  int event;
} fmgpioEventAcknowledged;

fmgpioIrqEnabledArgs _fm_actual_gpio_set_irq_enabled_with_callback_args [1000];
fmgpioEventAcknowledged _fm_actual_gpio_event_acknowledged_args [1000];
int _fm_actual_gpio_init_calls [1000];

int fm_get_total_callout() {
  int callcounts = 0;
  for (int i = 0; i < 6; i++)
  {
    callcounts = callcounts + fm_callcounts[i];
  }
  return callcounts;
}

void fm_fake_gpio_init(unsigned int pin) {
  _fm_actual_gpio_init_calls[fm_callcounts[0]] = pin;
  fm_call_sequence[fm_get_total_callout()] = 'I';
  fm_callcounts[0]++;
}

void fm_fake_gpio_set_irq_enabled_with_callback(unsigned int pin_number, unsigned int event, char enabled, fm_gpio_irq_callback_t callback) {
  _fm_actual_gpio_set_irq_enabled_with_callback_args[fm_callcounts[1]].pin_number = pin_number;
  _fm_actual_gpio_set_irq_enabled_with_callback_args[fm_callcounts[1]].event = event;
  _fm_actual_gpio_set_irq_enabled_with_callback_args[fm_callcounts[1]].enabled = enabled;
  _fm_actual_gpio_set_irq_enabled_with_callback_args[fm_callcounts[1]].callback = callback;
  fm_call_sequence[fm_get_total_callout()] = 'S';
  fm_callcounts[1]++;
}

void fm_fake_event_acknowledged(unsigned int pin_number, unsigned int event) {
  _fm_actual_gpio_set_irq_enabled_with_callback_args[fm_callcounts[2]].pin_number = pin_number;
  _fm_actual_gpio_set_irq_enabled_with_callback_args[fm_callcounts[2]].event = event;
  fm_call_sequence[fm_get_total_callout()] = 'A';
  fm_callcounts[2]++;
}

void test_fan_monitor() {
  for (int i = 0; i < 6; i++)
  {
    fm_callcounts[i] = 0;
  }
  // Initialises Pins @ zero
  const int TEST_PIN_ONE = 4;

  setup_fan_monitor_pin(TEST_PIN_ONE, fm_fake_gpio_init, fm_fake_gpio_set_irq_enabled_with_callback, fm_fake_event_acknowledged);

  assert(fm_callcounts[0] == 1);
  assert(fm_callcounts[1] == 1);
  assert(fm_callcounts[2] == 0);
  assert(get_fan_revolutions_since_reset(TEST_PIN_ONE) == 0);
  assert(fm_call_sequence[0] == 'I');
  assert(fm_call_sequence[1] == 'S');

  assert(_fm_actual_gpio_init_calls[0] == TEST_PIN_ONE);
  assert(_fm_actual_gpio_set_irq_enabled_with_callback_args[0].pin_number == TEST_PIN_ONE);
  assert(_fm_actual_gpio_set_irq_enabled_with_callback_args[0].event == fm_GPIO_IRQ_EDGE_RISE);
  assert(_fm_actual_gpio_set_irq_enabled_with_callback_args[0].enabled == 1);
  assert(_fm_actual_gpio_set_irq_enabled_with_callback_args[0].callback == _pulse_callback);
  assert(_fm_gpio_acknowledge_irq == fm_fake_event_acknowledged);


  // Send some fake revolutions
  const int testCount = 96;
  for (int i = 0; i < testCount; i++) {
    assert(fm_callcounts[2] == i);
    _pulse_callback(TEST_PIN_ONE, fm_GPIO_IRQ_EDGE_RISE);
    assert(fm_callcounts[2] == i + 1);
    assert(_fm_actual_gpio_set_irq_enabled_with_callback_args[i].pin_number == TEST_PIN_ONE);
    assert(_fm_actual_gpio_set_irq_enabled_with_callback_args[i].event == fm_GPIO_IRQ_EDGE_RISE);
  }

  assert(get_fan_revolutions_since_reset(TEST_PIN_ONE) == testCount);
  reset_revolutions(TEST_PIN_ONE);
  assert(get_fan_revolutions_since_reset(TEST_PIN_ONE) == 0);
  // Make sure we can just send more

  const int testCountTwo = 37;
  for (int i = 0; i < testCountTwo; i++) {
    assert(fm_callcounts[2] == i + testCount);
    _pulse_callback(TEST_PIN_ONE, fm_GPIO_IRQ_EDGE_RISE);
    assert(fm_callcounts[2] == i + 1 + testCount);
    assert(_fm_actual_gpio_set_irq_enabled_with_callback_args[i + testCount].pin_number == TEST_PIN_ONE);
    assert(_fm_actual_gpio_set_irq_enabled_with_callback_args[i + testCount].event == fm_GPIO_IRQ_EDGE_RISE);
  }
  
  assert(get_fan_revolutions_since_reset(TEST_PIN_ONE) == testCountTwo);
  reset_revolutions(TEST_PIN_ONE);

  // Add a second pin, make sure we can handle it all correctly
  const int TEST_PIN_TWO = 7;

  setup_fan_monitor_pin(TEST_PIN_TWO, fm_fake_gpio_init, fm_fake_gpio_set_irq_enabled_with_callback, fm_fake_event_acknowledged);

  assert(fm_callcounts[0] == 2);
  assert(fm_callcounts[1] == 2);
  assert(get_fan_revolutions_since_reset(TEST_PIN_TWO) == 0);

  assert(_fm_actual_gpio_init_calls[1] == TEST_PIN_TWO);
  assert(_fm_actual_gpio_set_irq_enabled_with_callback_args[1].pin_number == TEST_PIN_TWO);
  assert(_fm_actual_gpio_set_irq_enabled_with_callback_args[1].event == fm_GPIO_IRQ_EDGE_RISE);
  assert(_fm_actual_gpio_set_irq_enabled_with_callback_args[1].enabled == 1);
  assert(_fm_actual_gpio_set_irq_enabled_with_callback_args[1].callback == _pulse_callback);
  assert(_fm_gpio_acknowledge_irq == fm_fake_event_acknowledged);

  // Some pulses to one, some to another. 
  // This is a bit of a scattergun test

  const int PIN_ONE_COUNT = 12;
  const int PIN_TWO_COUNT = 37;

  for (int i = 0; i < PIN_ONE_COUNT; i++) {
    _pulse_callback(TEST_PIN_ONE, fm_GPIO_IRQ_EDGE_RISE);
  }

  for (int i = 0; i < PIN_TWO_COUNT; i++) {
    _pulse_callback(TEST_PIN_TWO, fm_GPIO_IRQ_EDGE_RISE);
  }
  
  assert(get_fan_revolutions_since_reset(TEST_PIN_ONE) == PIN_ONE_COUNT);
  assert(get_fan_revolutions_since_reset(TEST_PIN_TWO) == PIN_TWO_COUNT);

  reset_revolutions(TEST_PIN_ONE);

  assert(get_fan_revolutions_since_reset(TEST_PIN_ONE) == 0);
  assert(get_fan_revolutions_since_reset(TEST_PIN_TWO) == PIN_TWO_COUNT);
  
  reset_revolutions(TEST_PIN_TWO);
  assert(get_fan_revolutions_since_reset(TEST_PIN_ONE) == 0);
  assert(get_fan_revolutions_since_reset(TEST_PIN_TWO) == 0);

  _pulse_callback(TEST_PIN_ONE, fm_GPIO_IRQ_EDGE_RISE);
  assert(get_fan_revolutions_since_reset(TEST_PIN_ONE) == 1);
  assert(get_fan_revolutions_since_reset(TEST_PIN_TWO) == 0);
}