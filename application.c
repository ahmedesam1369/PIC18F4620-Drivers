/* 
 * File:   application.c
 * Author: Eng A_Esam
 *
 * Created on April 26, 2023, 2:51 PM
 */
#include "application.h"


extern led_t led1;

volatile uint32 timer3_counter_on = 0;
timer3_t timer3_counter_obj;

void Timer3_DefaultInterruptHandler(void){
    timer3_counter_on++;
    led_turn_toggle(&led1);
}



int main() {
    Std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
    timer3_counter_obj.Timer3_InterruptHandler = Timer3_DefaultInterruptHandler;
    timer3_counter_obj.priority = INTERRUPT_LOW_PRIORITY;
    timer3_counter_obj.timer3_prescaler_value = TIMER3_PRESCALLER_DIV_BY_8;
    timer3_counter_obj.timer3_preload_value = 0;
    timer3_counter_obj.timer3_mode = TIMR3_TIMER_MODE; 
    timer3_counter_obj.timer3_counter_mode = TIMER3_ASYNC_COUNTER_MODE;
    
    ret = Timer3_Init(&timer3_counter_obj);
    
    while(1){
        
    }
    return (EXIT_SUCCESS);

}

void application_intialize(void){
    Std_ReturnType ret = E_NOT_OK;
    ecu_layer_initialize();

}


