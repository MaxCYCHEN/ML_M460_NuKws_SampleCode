//
// Includes
//
#include "btn.h"

//
// Functions
//

//
// btn_low_level_init
//
void btn_low_level_init(void)
{
    GPIO_SetMode(PH, (BIT0 | BIT1), GPIO_MODE_INPUT);
    
    GPIO_SET_DEBOUNCE_TIME(PH, GPIO_DBCTL_DBCLKSRC_LIRC, GPIO_DBCTL_DBCLKSEL_512);
    GPIO_ENABLE_DEBOUNCE(PH, (BIT0 | BIT1));
    
    GPIO_EnableInt(PH, 0, GPIO_INT_FALLING);
    GPIO_EnableInt(PH, 1, GPIO_INT_FALLING);
}

//
// btn_pressed_detect
//
uint8_t btn_pressed_detect(BTN_NUM num)
{
    switch(num) {
    case BTN_0:
        if (GPIO_GET_INT_FLAG(PH, BIT0)) {
            GPIO_CLR_INT_FLAG(PH, BIT0);
            return 1;
        } 
        
        return 0;
    case BTN_1:
         if (GPIO_GET_INT_FLAG(PH, BIT1)) {
            GPIO_CLR_INT_FLAG(PH, BIT1);
             return 1;
        } 
    }
    
    return 0;
}