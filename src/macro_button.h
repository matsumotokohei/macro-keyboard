#ifndef MACRO_BUTTON_H
#define MACRO_BUTTON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <inttypes.h>

#define MACRO_BUTTON_MAX 8 /* 2x4 */

struct macro_button_t {
	uint8_t pin;
	uint8_t hid_key;
};

void macro_button_init(void);
uint8_t macro_button_key(void);

#ifdef __cplusplus
}
#endif

#endif // MACRO_BUTTON_H
