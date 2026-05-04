#include "bsp/board_api.h"
#include "tusb.h"
#include "usb_descriptors.h"
#include "macro_button.h"

enum {
	BLINK_NOT_MOUNTED = 250,
	BLINK_MOUNTED = 1000,
	BLINK_SUSPENDED = 2500,
	KEY_INPUT_INTERVAL_MS = 250,
};

static uint32_t blink_interval_ms = BLINK_NOT_MOUNTED;

static void led_blinking_task(void);
static void hid_task(void);

int main(void) {
	board_init();
	macro_button_init();

	tud_init(BOARD_TUD_RHPORT);

	if(board_init_after_tusb) {
		board_init_after_tusb();
	}

	while(1) {
		tud_task();
		led_blinking_task();

		hid_task();
	}

	return 0;
}

/* callback */
void tud_mount_cb(void) {
	blink_interval_ms = BLINK_MOUNTED;
}

void tud_umount_cb(void) {
	blink_interval_ms = BLINK_NOT_MOUNTED;
}

void tud_suspend_cb(bool remote_wakeup_en) {
	(void)remote_wakeup_en;
	blink_interval_ms = BLINK_SUSPENDED;
}

void tud_resume_cb(void) {
	blink_interval_ms = tud_mounted() ? BLINK_MOUNTED : BLINK_NOT_MOUNTED;
}

void send_hid_report(uint8_t report_id, uint32_t key) {
	if ( !tud_hid_ready() ) return;

	switch(report_id) {
	case REPORT_ID_KEYBOARD:
		static bool has_keyboard_key = false;

		if ( key ) {
			uint8_t modifiers = KEYBOARD_MODIFIER_LEFTCTRL | KEYBOARD_MODIFIER_LEFTALT;
			uint8_t keycode[6] = { 0 };

			keycode[0] = (uint8_t)key;

			tud_hid_keyboard_report(REPORT_ID_KEYBOARD, modifiers, keycode);
			has_keyboard_key = true;
		} else {
			if ( has_keyboard_key) tud_hid_keyboard_report(REPORT_ID_KEYBOARD, 0, NULL);
			has_keyboard_key = false;
		}
		break;

	default:
		break;
	}
}

static void hid_task(void) {
	const uint32_t interval_ms = KEY_INPUT_INTERVAL_MS;
	static uint32_t start_ms = 0;

	if ( board_millis() - start_ms < interval_ms) return;
	start_ms += interval_ms;

	uint32_t const key = macro_button_key();

	if ( tud_suspended() && key ) {
		tud_remote_wakeup();
	} else {
		send_hid_report(REPORT_ID_KEYBOARD, key);
	}
}

void tud_hid_report_complete_cb(uint8_t instance, uint8_t const * report, uint16_t len) {
	(void)instance;
	(void)len;

	uint8_t next_report_id = report[0] + 1u;

	if ( next_report_id < REPORT_ID_COUNT ) {
		send_hid_report(next_report_id, macro_button_key());
	}
}

uint16_t tud_hid_get_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t* buffer, uint16_t reqlen) {
	(void)instance;
	(void)report_id;
	(void)report_type;
	(void)buffer;
	(void)reqlen;

	return 0;
}


void tud_hid_set_report_cb(uint8_t instance, uint8_t report_id, hid_report_type_t report_type, uint8_t const* buffer, uint16_t bufsize) {
	(void)instance;

	if ( report_type == HID_REPORT_TYPE_OUTPUT ) {
		if ( report_id == REPORT_ID_KEYBOARD ) {
			if ( bufsize < 1 ) return;

			uint8_t const kbd_leds = buffer[0];

			if ( kbd_leds & KEYBOARD_LED_CAPSLOCK ) {
				blink_interval_ms = 0;
				board_led_write(true);
			} else {
				board_led_write(false);
				blink_interval_ms = BLINK_MOUNTED;
			}
		}
	}
}

static void led_blinking_task(void) {
	static uint32_t start_ms = 0;
	static bool led_state = false;

	if(!blink_interval_ms) return;

	if(board_millis() - start_ms < blink_interval_ms) return;
	start_ms += blink_interval_ms;

	board_led_write(led_state);
	led_state = !led_state;
}

