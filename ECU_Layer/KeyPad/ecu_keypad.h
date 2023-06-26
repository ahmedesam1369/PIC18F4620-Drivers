/* 
 * File:   ecu_keypad.h
 * Author: Eng A_Esam
 *
 * Created on April 26, 2023, 5:00 AM
 */

#ifndef ECU_KEYPAD_H
#define	ECU_KEYPAD_H

/* Section : Includes  */
#include "ecu_keypad_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"


/* Section : Macro Declarations */
#define Ecu_KeyPad_Rows 4
#define Ecu_KeyPad_Columns 4
/* Section: Macro Functions Declarations */

/* Section : Data Type Declarations*/
typedef struct{
    pin_config_t KeyPad_Row_Pins[Ecu_KeyPad_Rows];
    pin_config_t KeyPad_Columns_Pins[Ecu_KeyPad_Columns];
}keypad_t; 
/* Section : Function Declarations */
Std_ReturnType KeyPad_Initialize(const keypad_t *keypad_obj);
Std_ReturnType KeyPad_Get_Value(const keypad_t *keypad_obj, uint8 *value);

   
#endif	/* ECU_KEYPAD_H */

