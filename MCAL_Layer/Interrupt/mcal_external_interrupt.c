/* 
 * File:   mcal_external_interrupt.c
 * Author: Eng A_Esam
 *
 * Created on May 29, 2023, 9:51 PM
 */
#include "mcal_external_interrupt.h"

static void (*INT0_InterruptHandler)(void) = NULL;
static void (*INT1_InterruptHandler)(void) = NULL;
static void (*INT2_InterruptHandler)(void) = NULL;

static void (*RB4_InterruptHandlerHigh)(void) = NULL;
static void (*RB4_InterruptHandlerLow)(void) = NULL;
static void (*RB5_InterruptHandlerHigh)(void) = NULL;
static void (*RB5_InterruptHandlerLow)(void) = NULL;
static void (*RB6_InterruptHandlerHigh)(void) = NULL;
static void (*RB6_InterruptHandlerLow)(void) = NULL;
static void (*RB7_InterruptHandlerHigh)(void) = NULL;
static void (*RB7_InterruptHandlerLow)(void) = NULL;

static Std_ReturnType NTERUPT_INTx_Enable(const interrupt_INTx_t *int_obj);
static Std_ReturnType NTERUPT_INTx_Disable(const interrupt_INTx_t *int_obj);
static Std_ReturnType NTERUPT_INTx_Priority_Init(const interrupt_INTx_t *int_obj);
static Std_ReturnType NTERUPT_INTx_Edge_Init(const interrupt_INTx_t *int_obj);
static Std_ReturnType NTERUPT_INTx_Pin_Init(const interrupt_INTx_t *int_obj);
static Std_ReturnType NTERUPT_INTx_Clear_Flag(const interrupt_INTx_t *int_obj);

static Std_ReturnType INT0_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType INT1_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType INT2_SetInterruptHandler(void (*InterruptHandler)(void));
static Std_ReturnType INTERRUPT_INTx_SetInterruptHandler(const interrupt_INTx_t *int_obj);


static Std_ReturnType NTERUPT_RBx_Priority_Init(const interrupt_RBx_t *rb_obj);
static Std_ReturnType NTERUPT_RBx_Pin_Init(const interrupt_RBx_t *rb_obj);

static Std_ReturnType INTERRUPT_RBx_SetInterruptHandler(const interrupt_RBx_t *rb_obj);


Std_ReturnType NTERUPT_INTx_Init(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
       
    }else{
//        Disable the External Interrupt
        ret = NTERUPT_INTx_Disable(int_obj);
//        Clear The Interrupt Flag
        ret |= NTERUPT_INTx_Clear_Flag(int_obj);
//        Configure External Interrupt Edge
        ret |= NTERUPT_INTx_Edge_Init(int_obj);
//        Configure External Interrupt Priority
        #if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        ret |= NTERUPT_INTx_Priority_Init(int_obj);
        #endif
//        Configure External Interrupt I/O Pint
        ret |= NTERUPT_INTx_Pin_Init(int_obj);
//        Configure External Interrupt CallBack
        ret |= INTERRUPT_INTx_SetInterruptHandler(int_obj);
//        Enable the External Interrupt
        ret |= NTERUPT_INTx_Enable(int_obj);
        
    }
    return ret;
}

void INT0_ISR(void){
//    The INT0 external interrupt occurred (must be cleared in software)
    EXT_INT0_InterruptFlagClear();
//    Code
    
//    Callback function gets called every time this ISR executes
    if(INT0_InterruptHandler){
        INT0_InterruptHandler();
    }
}
void INT1_ISR(void){
//    The INT1 external interrupt occurred (must be cleared in software)
    EXT_INT1_InterruptFlagClear();
//    Code
    
//    Callback function gets called every time this ISR executes
    if(INT1_InterruptHandler){
        INT1_InterruptHandler();
    }
}
void INT2_ISR(void){
//    The INT2 external interrupt occurred (must be cleared in software)
    EXT_INT2_InterruptFlagClear();
//    Code
    
//    Callback function gets called every time this ISR executes
    if(INT2_InterruptHandler){
        INT2_InterruptHandler();
    }
}

Std_ReturnType NTERUPT_INTx_DEInit(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }else{
        ret = NTERUPT_INTx_Disable(int_obj);
        
    }
    return ret;
}
Std_ReturnType NTERUPT_RBx_Init(const interrupt_RBx_t *rb_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == rb_obj){
        ret = E_NOT_OK;
    }else{
        //This routine clears the interrupt enable for the external interrupt, RBX
        EXT_RBX_InterruptDisable();
        //This routine clears the interrupt flag for the external interrupt, RBX
        EXT_RBX_InterruptFlagClear();
        ret = NTERUPT_RBx_Priority_Init(rb_obj);
//        Configure External Interrupt I/O Pint
        ret |= NTERUPT_RBx_Pin_Init(rb_obj);
//        Configure External Interrupt CallBack
        ret |= INTERRUPT_RBx_SetInterruptHandler(rb_obj);
        //This routine sets the interrupt enable for the external interrupt, RBX
        EXT_RBX_InterruptEnable();
    }
    return ret;
}
Std_ReturnType NTERUPT_RBx_DEInit(const interrupt_RBx_t *rb_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == rb_obj){
        ret = E_NOT_OK;
    }else{
        
        
    }
    return ret;
}


void RB4_ISR(uint8 RB4_Source){
//    The RBx external interrupt occurred (must be cleared in software)
    EXT_RBX_InterruptFlagClear();
//    Code
    
//    Callback function gets called every time this ISR executes
    if(0== RB4_Source){
        if(RB4_InterruptHandlerLow){
        RB4_InterruptHandlerLow();
        }else{ /* Nothing */}
    }else{
        if(RB4_InterruptHandlerHigh){
        RB4_InterruptHandlerHigh();
        }else{ /* Nothing */}
        
    }
    
}
void RB5_ISR(uint8 RB5_Source){
//    The RBx external interrupt occurred (must be cleared in software)
    EXT_RBX_InterruptFlagClear();
//    Code
    
//    Callback function gets called every time this ISR executes
    if(0== RB5_Source){
        if(RB5_InterruptHandlerLow){
        RB5_InterruptHandlerLow();
        }else{ /* Nothing */}
    }else{
        if(RB5_InterruptHandlerHigh){
        RB5_InterruptHandlerHigh();
        }else{ /* Nothing */}
        
    }
    
}
void RB6_ISR(uint8 RB6_Source){
//    The RBx external interrupt occurred (must be cleared in software)
    EXT_RBX_InterruptFlagClear();
//    Code
    
//    Callback function gets called every time this ISR executes
    if(0== RB6_Source){
        if(RB6_InterruptHandlerLow){
        RB6_InterruptHandlerLow();
        }else{ /* Nothing */}
    }else{
        if(RB6_InterruptHandlerHigh){
        RB6_InterruptHandlerHigh();
        }else{ /* Nothing */}
        
    }
    
}
void RB7_ISR(uint8 RB7_Source){
//    The RBx external interrupt occurred (must be cleared in software)
    EXT_RBX_InterruptFlagClear();
//    Code
    
//    Callback function gets called every time this ISR executes
    if(0== RB7_Source){
        if(RB7_InterruptHandlerLow){
        RB7_InterruptHandlerLow();
        }else{ /* Nothing */}
    }else{
        if(RB7_InterruptHandlerHigh){
        RB7_InterruptHandlerHigh();
        }else{ /* Nothing */}
        
    }
    
}
 
//----------------------------- INTx --------------------------------------------

static Std_ReturnType NTERUPT_INTx_Enable(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }else{
        switch(int_obj->source){
            case INTERRUPT_EXTERNAL_INT0: 
                #if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                /* INT0 Interrupt is always High Priority Interrupt */
                INTERRUPT_GlobalInterruptHighEnable(); 
                #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
                #endif
                EXT_INT0_InterruptEnable(); 
                ret = E_OK; 
                break;
            case INTERRUPT_EXTERNAL_INT1: 
                #if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                if(int_obj->priority == INTERRUPT_HIGH_PRIORITY){
                     INTERRUPT_GlobalInterruptHighEnable();   
                }else if( int_obj->priority == INTERRUPT_LOW_PRIORITY){
                    INTERRUPT_GlobalInterruptLowEnable();
                }else{
                    /*  Nothing  */
                }  
                #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
                #endif
                EXT_INT1_InterruptEnable(); 
                ret = E_OK; 
                break;
            case INTERRUPT_EXTERNAL_INT2: 
                #if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
                INTERRUPT_PriorityLevelsEnable();
                if(int_obj->priority == INTERRUPT_HIGH_PRIORITY){
                     INTERRUPT_GlobalInterruptHighEnable();   
                }else if( int_obj->priority == INTERRUPT_LOW_PRIORITY){
                    INTERRUPT_GlobalInterruptLowEnable();
                }else{
                    /*  Nothing  */
                }  
                #else
                INTERRUPT_GlobalInterruptEnable();
                INTERRUPT_PeripheralInterruptEnable();
                #endif
                EXT_INT2_InterruptEnable(); 
                ret = E_OK; 
                break;
            default: ret = E_NOT_OK;
        }
        
    }
    return ret;
}

static Std_ReturnType NTERUPT_INTx_Disable(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }else{
        switch(int_obj->source){
            case INTERRUPT_EXTERNAL_INT0: EXT_INT0_InterruptDisable(); ret = E_OK; break;
            case INTERRUPT_EXTERNAL_INT1: EXT_INT1_InterruptDisable(); ret = E_OK; break;
            case INTERRUPT_EXTERNAL_INT2: EXT_INT2_InterruptDisable(); ret = E_OK; break;
            default: ret = E_NOT_OK;
        }
        
    }
    return ret;
}
#if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
static Std_ReturnType NTERUPT_INTx_Priority_Init(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }else{
        switch(int_obj->source){
            case INTERRUPT_EXTERNAL_INT1: 
                if(int_obj->priority == INTERRUPT_LOW_PRIORITY){
                    EXT_INT1_InterruptLowPriority();
                    ret = E_OK; 
                }else if(int_obj->priority == INTERRUPT_HIGH_PRIORITY){
                    EXT_INT1_InterruptHighPriority();
                    ret = E_OK; 
                }else{
                   ret = E_NOT_OK;  
                }
                break;
            case INTERRUPT_EXTERNAL_INT2: 
                if(int_obj->priority == INTERRUPT_LOW_PRIORITY){
                    EXT_INT2_InterruptLowPriority();
                    ret = E_OK; 
                }else if(int_obj->priority == INTERRUPT_HIGH_PRIORITY){
                    EXT_INT2_InterruptHighPriority();
                    ret = E_OK; 
                }else{
                   ret = E_NOT_OK;  
                }
                break;
            default: ret = E_NOT_OK;
        }
        
    }
    return ret;
}
#endif
static Std_ReturnType NTERUPT_INTx_Edge_Init(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }else{
        switch(int_obj->source){
            case INTERRUPT_EXTERNAL_INT0: 
                if(int_obj->edge == INTERRUPT_RISING_EDGE){
                    EXT_INT0_RisingEdgeSet();
                    ret = E_OK; 
                }else if(int_obj->edge == INTERRUPT_FALLING_EDGE){
                    EXT_INT0_FallingEdgeSet();
                    ret = E_OK; 
                }else{
                    ret = E_NOT_OK;
                } 
                break;
            case INTERRUPT_EXTERNAL_INT1: 
                if(int_obj->edge == INTERRUPT_RISING_EDGE){
                    EXT_INT1_RisingEdgeSet();
                    ret = E_OK; 
                }else if(int_obj->edge == INTERRUPT_FALLING_EDGE){
                    EXT_INT1_FallingEdgeSet();
                    ret = E_OK; 
                }else{
                    ret = E_NOT_OK;
                } 
                break;
            case INTERRUPT_EXTERNAL_INT2:
                if(int_obj->edge == INTERRUPT_RISING_EDGE){
                    EXT_INT2_RisingEdgeSet();
                    ret = E_OK; 
                }else if(int_obj->edge == INTERRUPT_FALLING_EDGE){
                    EXT_INT2_FallingEdgeSet();
                    ret = E_OK; 
                }else{
                    ret = E_NOT_OK;
                } 
                break;
            default: ret = E_NOT_OK;
        }
        
    }
    return ret;
}
static Std_ReturnType NTERUPT_INTx_Pin_Init(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }else{
        ret = gpio_pin_direction_initialize(&(int_obj->mcu_pin));
    }
    return ret;
}

static Std_ReturnType NTERUPT_INTx_Clear_Flag(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }else{
        switch(int_obj->source){
            case INTERRUPT_EXTERNAL_INT0: 
                EXT_INT0_InterruptFlagClear();
                ret = E_OK; 
                break;
            case INTERRUPT_EXTERNAL_INT1: 
                EXT_INT1_InterruptFlagClear();
                ret = E_OK; 
                break;
            case INTERRUPT_EXTERNAL_INT2:
                EXT_INT2_InterruptFlagClear();
                ret = E_OK; 
                break;
            default: ret = E_NOT_OK;
        }
        
    }
    return ret;
}


static Std_ReturnType INT0_SetInterruptHandler(void (*InterruptHandler)(void)){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler){
        ret = E_NOT_OK;
    }else{
        INT0_InterruptHandler = InterruptHandler;
        ret = E_OK; 
    }
    return ret;
}
static Std_ReturnType INT1_SetInterruptHandler(void (*InterruptHandler)(void)){
    
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler){
        ret = E_NOT_OK;
    }else{
        INT1_InterruptHandler = InterruptHandler;
        ret = E_OK; 
    }
    return ret;
    
}
static Std_ReturnType INT2_SetInterruptHandler(void (*InterruptHandler)(void)){
    
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == InterruptHandler){
        ret = E_NOT_OK;
    }else{
        INT2_InterruptHandler = InterruptHandler;
        ret = E_OK; 
    }
    return ret;
}
static Std_ReturnType INTERRUPT_INTx_SetInterruptHandler(const interrupt_INTx_t *int_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == int_obj){
        ret = E_NOT_OK;
    }else{
        switch(int_obj->source){
            case INTERRUPT_EXTERNAL_INT0: 
                ret = INT0_SetInterruptHandler(int_obj->EXT_InterruptHandler);
                break;
            case INTERRUPT_EXTERNAL_INT1: 
                ret = INT1_SetInterruptHandler(int_obj->EXT_InterruptHandler);
                break;
            case INTERRUPT_EXTERNAL_INT2:
                ret = INT2_SetInterruptHandler(int_obj->EXT_InterruptHandler);
                break;
            default: ret = E_NOT_OK;
        }
        
    }
    return ret;
}

//------------------------ RBX -----------------------------------------------------
static Std_ReturnType NTERUPT_RBx_Priority_Init(const interrupt_RBx_t *rb_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == rb_obj){
        ret = E_NOT_OK;
    }else{
        #if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(rb_obj->priority == INTERRUPT_LOW_PRIORITY){
           INTERRUPT_GlobalInterruptLowEnable();
           EXT_RBX_InterruptLowPriority();
        }else if(rb_obj->priority == INTERRUPT_HIGH_PRIORITY){
            INTERRUPT_GlobalInterruptHighEnable();
           EXT_RBX_InterruptHighPriority();
        }else{ /*     Nothing   */}
        #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
    ret = E_OK;
    }
    return ret;
    
        
}
static Std_ReturnType NTERUPT_RBx_Pin_Init(const interrupt_RBx_t *rb_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == rb_obj){
        ret = E_NOT_OK;
    }else{
        ret = gpio_pin_direction_initialize(&(rb_obj->mcu_pin));
    }
    return ret;
}

static Std_ReturnType INTERRUPT_RBx_SetInterruptHandler(const interrupt_RBx_t *rb_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == rb_obj){
        ret = E_NOT_OK;
    }else{
        switch(rb_obj->mcu_pin.pin){
            case GPIO_PIN4:
                RB4_InterruptHandlerHigh = rb_obj->EXT_InterruptHandlerHigh;
                RB4_InterruptHandlerLow = rb_obj->EXT_InterruptHandlerLow;
                ret = E_OK;
                break;
            case GPIO_PIN5:
                RB5_InterruptHandlerHigh = rb_obj->EXT_InterruptHandlerHigh;
                RB5_InterruptHandlerLow = rb_obj->EXT_InterruptHandlerLow;
                ret = E_OK;
                break;
            case GPIO_PIN6:
                RB6_InterruptHandlerHigh = rb_obj->EXT_InterruptHandlerHigh;
                RB6_InterruptHandlerLow = rb_obj->EXT_InterruptHandlerLow;
                ret = E_OK;
                break;
            case GPIO_PIN7:
                RB7_InterruptHandlerHigh = rb_obj->EXT_InterruptHandlerHigh;
                RB7_InterruptHandlerLow = rb_obj->EXT_InterruptHandlerLow;
                ret = E_OK;
                break;
            default: 
                ret = E_NOT_OK;
                break;
        }
        
    }
    return ret;
}