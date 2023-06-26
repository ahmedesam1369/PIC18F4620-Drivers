/* 
 * File:   ecu_led.c
 * Author: Eng A_Esam
 *
 * Created on January 21, 2023, 5:52 PM
 */

#include "ecu_led.h"

/**
 * @brief Initialize the assigned pin to be output and turn the led OFF or ON
 * @param led : pointer to the module configuration
 * @return Status of the function :
 *          (E_OK) : The Function Done Successfully
 *          (E_NOT_OK) : The function has issue while performing the function
 */
Std_ReturnType led_initialize(const led_t *led)
{
    Std_ReturnType  ret = E_OK;
    
    if(NULL == led){
        ret = E_NOT_OK;
    }else{
    pin_config_t pin_obj = {
        .port = led -> port_name,
        .pin = led -> pin,
        .logic = led -> led_status,
        .direction = GPIO_DIRECTION_OUTPUT,
    };
    gpio_pin_initialize(&pin_obj);
        
    }
    return ret;
    
}

/**
 * @breif turn the led on
 * @param led pointer to the module configuration
 * @return Status of the function :
 *          (E_OK) : The Function Done Successfully
 *          (E_NOT_OK) : The function has issue while performing the function
 */
Std_ReturnType led_turn_on(const led_t *led)
{
    Std_ReturnType ret = E_OK;
    if(NULL == led){
        ret = E_NOT_OK;
    }else{
        pin_config_t pin_obj = {
            .port = led -> port_name,
            .pin = led -> pin,
            .logic = led -> led_status,
            .direction = GPIO_DIRECTION_OUTPUT,
        };
        gpio_pin_write_logic(&pin_obj,GPIO_HIGH);
        
    }
    return ret;
    
}

/**
 * @breif turn the led off
 * @param led pointer to the module configuration
 * @return Status of the function :
 *          (E_OK) : The Function Done Successfully
 *          (E_NOT_OK) : The function has issue while performing the function
 */
Std_ReturnType led_turn_off(const led_t *led)
{
    Std_ReturnType ret = E_OK;
    if(NULL == led){
        ret = E_NOT_OK;
    }else{
        pin_config_t pin_obj = {
            .port = led -> port_name,
            .pin = led -> pin,
            .logic = led -> led_status,
            .direction = GPIO_DIRECTION_OUTPUT,
        };
        gpio_pin_write_logic(&pin_obj,GPIO_LOW);
        
    }
    return ret;
    
}

/**
 * @breif toggle  led 
 * @param led pointer to the module configuration
 * @return Status of the function :
 *          (E_OK) : The Function Done Successfully
 *          (E_NOT_OK) : The function has issue while performing the function
 */
Std_ReturnType led_turn_toggle(const led_t *led)
{
    Std_ReturnType ret = E_OK;
    if(NULL == led){
        ret = E_NOT_OK;
    }else{
        pin_config_t  pin_obj = {.port = led->port_name,
                        .pin=led->pin, .logic = led->led_status, .direction = GPIO_DIRECTION_OUTPUT};
         ret = gpio_pin_toggle_logic(&pin_obj);
    }
    return ret;
}