#include "bsp/board_api.h"
#include "tusb.h"
#include "hardware/gpio.h"
#include "macro_button.h"

/* 2x4 */
static struct macro_button_t macro_buttons[MACRO_BUTTON_MAX] = {
	{ 16, HID_KEY_F10 }, // PIN21
	{ 17, HID_KEY_VOLUME_DOWN }, // PIN22
	{ 18, HID_KEY_VOLUME_UP }, // PIN24
	{ 19, HID_KEY_MUTE }, // PIN25
	{ 20, HID_KEY_ARROW_RIGHT }, // PIN26 Win+右
	{ 21, HID_KEY_F1 }, // PIN27
	{ 14, HID_KEY_F2 }, // PIN19
	{ 15, HID_KEY_F4 }, // PIN20 Alt+F4
};

/**
 * F1 - F8 keys initialization, pull-up
 */
void macro_button_init(void) {
	for (int i = 0; i < MACRO_BUTTON_MAX; i++) {
		int pin = macro_buttons[i].pin;
		if ( pin >= 0 ) {
			gpio_init(pin);
			gpio_set_dir(pin, GPIO_IN);
			gpio_pull_up(pin); // Enable pull-up resistor
		} else {
			// Invalid pin, handle error if necessary
		}
	}
}

/**
 * F1 - F8 keys are used for the 8 buttons, active low.
 * Returns the keycode of the first pressed button, or 0 if no button is pressed.
 */
uint8_t macro_button_key(void) {
	uint8_t key = 0;
	static uint8_t last_key = 0;

	for (uint i = 0; i < MACRO_BUTTON_MAX; i++) {
		uint8_t val = 1;

		val = gpio_get(macro_buttons[i].pin);

		if ( val == 0 ) { // Active low, detect falling edge
			key = macro_buttons[i].hid_key; // Return the corresponding HID key for the first pressed button
			break;
		}
	}

	if ( last_key != key ) {
		last_key = key;
	} else {
		key = 0; // No change, treat as no key pressed
	}

	return key;
}
