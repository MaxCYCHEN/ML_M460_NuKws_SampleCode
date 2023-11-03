#ifndef __BTN_H__
#define __BTN_H__

#include "NuMicro.h"

typedef enum {
    BTN_0,
    BTN_1,
} BTN_NUM;

void btn_low_level_init(void);
uint8_t btn_pressed_detect(BTN_NUM num);

#endif  /* __BTN_H__ */
