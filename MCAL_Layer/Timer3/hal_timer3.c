/* 
 * File:   hal_timer3.c
 * Author: Eng A_Esam
 *
 * Created on June 25, 2023, 12:39 PM
 */
#include "hal_timer3.h"

static inline void Timer3_Mode_Select(const timer3_t *_timer);
static uint16 timer3_preload = ZERO_INIT;
#if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void (* TIMER3_InterruptHandler)(void) = NULL;
#endif
    
Std_ReturnType Timer3_Init(const timer3_t *_timer){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer){
        ret = E_NOT_OK;
    }else{
        TIMER3_MODULE_DISABLE();
        Timer3_Mode_Select(_timer);
        TIMER3_PRESCALER_SELECT(_timer->timer3_prescaler_value);
        TMR3H = (_timer->timer3_preload_value) >> 8;
        TMR3L = (uint8)(_timer->timer3_preload_value);
        timer3_preload = _timer->timer3_preload_value;
       
#if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE  
       TIMER3_InterruptEnable();
       TIMER3_InterruptFlagClear();
       TIMER3_InterruptHandler = _timer->Timer3_InterruptHandler;
 
#if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _timer->priority){
           INTERRUPT_GlobalInterruptHighEnable();
           TIMER3_InterruptHighPriority();
       }else if(INTERRUPT_LOW_PRIORITY == _timer->priority){
           INTERRUPT_GlobalInterruptLowEnable();
           TIMER3_InterruptLowPriority();
       }else{/* Nothing */}
        #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
#endif
        TIMER3_MODULE_ENABLE();
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType Timer3_DeInit(const timer3_t *_timer){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer){
        ret = E_NOT_OK;
    }else{
        TIMER3_MODULE_DISABLE();
#if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        TIMER3_InterruptDisable();
#endif
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType Timer3_Write_Value(const timer3_t *_timer, uint16 _value){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _timer){
        ret = E_NOT_OK;
    }else{
        TMR3H = (_value) >> 8;
        TMR3L = (uint8)(_value);
        timer3_preload = _value;
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType Timer3_Read_Value(const timer3_t *_timer, uint16 *_value){
    Std_ReturnType ret = E_NOT_OK;
    uint8 l_tmr3l = ZERO_INIT, l_tmr3h = ZERO_INIT;
    if((NULL == _timer) || (NULL == _value)){
        ret = E_NOT_OK;
    }else{
        l_tmr3l = TMR3L;
        l_tmr3h = TMR3H;
        *_value = (uint16)((l_tmr3h << 8) + l_tmr3l);
        ret = E_OK;
    }
    return ret;
}  
   

static inline void Timer3_Mode_Select(const timer3_t *_timer){
   if(TIMR3_TIMER_MODE == _timer->timer3_mode){
            TIMER3_TIMER_MODE_ENABLE();
       }else if(TIMER3_COUNTER_MODE == _timer->timer3_mode){
            TIMER3_COUNTER_MODE_ENABLE();
            if(TIMER3_SYNC_COUNTER_MODE == _timer->timer3_counter_mode){
                TIMER3_SYNC_COUNTER_MODE_ENABLE();
            }else if(TIMER3_ASYNC_COUNTER_MODE == _timer->timer3_counter_mode){
                TIMER3_ASYNC_COUNTER_MODE_ENABLE();
            }
            else{/* Nothing */}
       }else{/* Nothing */}
}
    
void TIMER3_ISR(void){
    //    The TIMER3 interrupt occurred (must be cleared in software)
        TIMER3_InterruptFlagClear();
        
        TMR3H = (timer3_preload) >> 8;
        TMR3L = (uint8)(timer3_preload);
    //    Callback function gets called every time this ISR executes
    if(TIMER3_InterruptHandler){
        TIMER3_InterruptHandler();
    }else{
        /* Nothing */
    }
    
}