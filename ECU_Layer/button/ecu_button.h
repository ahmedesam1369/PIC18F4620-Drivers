/* 
 * File:   ecu_button.h
 * Author: Eng A_Esam
 *
 * Created on March 26, 2023, 12:28 AM
 */

#ifndef ECU_BUTTON_H
#define	ECU_BUTTON_H

/*Section: Includes */
#include "ecu_button_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"

/* Section: Macro Declaration */

/* Section: Macro Function Declaration */

/* Section: Data Type Declaration */
typedef enum{
    BUTTON_PRESSED = 0,
    BUTTON_RELEASED,
}button_state_t;

typedef enum {
    BUTTON_ACTIVE_HIGH,
    BUTTON_ACTIVE_LOW
}button_active_t;

typedef struct{
    pin_config_t button_pin;
    button_state_t button_state;
    button_active_t button_connection;
}button_t;

/* Section: Function Declaration */
Std_ReturnType button_initialize(button_t *btn);
Std_ReturnType button_read_state(button_t *btn, button_state_t *btn_state);

#endif	/* ECU_BUTTON_H */

