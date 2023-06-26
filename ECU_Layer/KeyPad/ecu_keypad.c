/* 
 * File:   ecu_keypad.c
 * Author: Eng A_Esam
 *
 * Created on April 26, 2023, 5:00 AM
 */

#include "ecu_keypad.h"

static const uint8 btn_valus[Ecu_KeyPad_Rows][Ecu_KeyPad_Columns] = {
                                                                        {'7','8','9','/'},
                                                                        {'4','5','6','*'},
                                                                        {'1','2','3','-'},
                                                                        {'#','0','=','+'},

                                                            };

Std_ReturnType KeyPad_Initialize(const keypad_t *keypad_obj)
{
    Std_ReturnType ret = E_OK;
    if(NULL == keypad_obj){
        ret = E_NOT_OK;
    }else{
        for(uint8 i = 0; i<Ecu_KeyPad_Rows; i++)
        {
            ret = gpio_pin_initialize(&(keypad_obj->KeyPad_Row_Pins[i]));
        }
        for(uint8 i = 0; i<Ecu_KeyPad_Columns; i++)
        {
            ret = gpio_pin_direction_initialize(&(keypad_obj->KeyPad_Columns_Pins[i]));
        }
        
        
    }
    return ret;
}
Std_ReturnType KeyPad_Get_Value(const keypad_t *keypad_obj, uint8 *value)
{
    Std_ReturnType ret = E_OK;
    uint8 l_row_counter = ZERO_INIT , l_columns_counter, l_counter = ZERO_INIT;
    uint8 column_logic = ZERO_INIT;
    if((NULL == keypad_obj) || (NULL == value)){
        ret = E_NOT_OK;
    }else{
        for(l_row_counter=ZERO_INIT; l_row_counter< Ecu_KeyPad_Rows;  l_row_counter++)
        {
            for(l_counter =ZERO_INIT; l_counter< Ecu_KeyPad_Rows; l_counter++){
                ret = gpio_pin_write_logic(&(keypad_obj->KeyPad_Row_Pins[l_counter]), GPIO_LOW);
            }
            ret = gpio_pin_write_logic(&(keypad_obj->KeyPad_Row_Pins[l_row_counter]), GPIO_HIGH);
//            __delay_ms(10);
            for(l_columns_counter =ZERO_INIT; l_columns_counter< Ecu_KeyPad_Columns; l_columns_counter++){
                ret = gpio_pin_read_logic(&(keypad_obj->KeyPad_Columns_Pins[l_columns_counter]), &column_logic);
                if(GPIO_HIGH == column_logic){
                    *value = btn_valus[l_row_counter][l_columns_counter];
                }
            }
            
        }
        
    }
    return ret;
    
}