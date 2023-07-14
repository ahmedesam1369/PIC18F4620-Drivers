/* 
 * File:   hal_ccp1.c
 * Author: Eng A_Esam
 *
 * Created on June 26, 2023, 2:09 PM
 */
#include "hal_ccp.h"

#if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void (* CCP1_InterruptHandler)(void) = NULL;
#endif
#if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void (* CCP2_InterruptHandler)(void) = NULL;
#endif
    
static void CPP_CAPTURE_MOde_Config(const ccp_t *_ccp_obj);
static void CPP_Compare_Mode_Config(const ccp_t *_ccp_obj);
static void CCP_PWM_Mode_Config(const ccp_t *_ccp_obj);
static void CCP_Interrupt_Config(const ccp_t *_ccp_obj);
static void CCP_Mode_Timer_Select(const ccp_t *_ccp_obj);


Std_ReturnType CCP_Init(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }else{
        /* Disable CCP Module*/
        if(CCP1_INST == _ccp_obj->cpp_inst){
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
        }else if(CCP2_INST == _ccp_obj->cpp_inst){
            CCP2_SET_MODE(CCP_MODULE_DISABLE);
        }else{/* Nothing */ }
        
        if(CCP_CAPTURE_MODE_SELECTED == _ccp_obj->ccp_mode){
          CPP_CAPTURE_MOde_Config(_ccp_obj);        
          ret = E_OK;
        }else if(CCP_COMPARE_MODE_SELECTED == _ccp_obj->ccp_mode){
            CPP_Compare_Mode_Config(_ccp_obj);
            ret = E_OK;
        }else if(CCP_PWM_MODE_SELECTED == _ccp_obj->ccp_mode){
           CCP_PWM_Mode_Config(_ccp_obj);
           ret = E_OK;
        }else{/* Nothing */}
        
        /* Pin Configurations */
        ret = gpio_pin_initialize(&(_ccp_obj->ccp_pin));
        
        /* Interrupt Configurations */
        CCP_Interrupt_Config(_ccp_obj);
        
    }
    return ret;
}


Std_ReturnType CCP_DeInit(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }else{
        if(CCP1_INST == _ccp_obj->cpp_inst){
            CCP1_SET_MODE(CCP_MODULE_DISABLE);
            #if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            CCP1_InterruptDisable();
            #endif
            ret = E_OK;
        }else if(CCP2_INST == _ccp_obj->cpp_inst){
            CCP2_SET_MODE(CCP_MODULE_DISABLE);
            #if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
            CCP2_InterruptDisable();
            #endif
            ret = E_OK;
        }else{ret = E_NOT_OK;}     
    }
    return ret;
}

#if ( CCP_CFG_SELECTED_MODE == CCP_CFG_CAPTURE_MODE_SELECTED )
Std_ReturnType CCP_IsCapturedDataReady(const ccp_t *_ccp_obj, uint8 * _capture_status){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _capture_status) || (NULL == _ccp_obj)){
        ret = E_NOT_OK;
    }else{
        if(CCP1_INST == _ccp_obj->cpp_inst){
            if(PIR1bits.CCP1IF == INTERRUPT_OCCUR){
            *_capture_status = CCP_CAPTURE_READY;
            CCP1_InterruptFlagClear();
            }else{
                *_capture_status = CCP_CAPTURE_NOT_READY;
            }
            ret = E_OK;
        }else if(CCP2_INST == _ccp_obj->cpp_inst){
            if(PIR2bits.CCP2IF == INTERRUPT_OCCUR){
            *_capture_status = CCP_CAPTURE_READY;
            CCP2_InterruptFlagClear();
            }else{
                *_capture_status = CCP_CAPTURE_NOT_READY;
            }
            ret = E_OK;
        }else{
            ret = E_NOT_OK;
        }
        
    }
    return ret;
}

Std_ReturnType CCP_CaptureModeReadValue(const ccp_t *_ccp_obj, uint16 *capture_value){
    Std_ReturnType ret = E_NOT_OK;
    CCP_REG_T capture_temp_value = {.ccp_16Bit  = ZERO_INIT };
    if((NULL == capture_value) || (NULL == _ccp_obj)){
        ret = E_NOT_OK;
    }else{
        if(CCP1_INST == _ccp_obj->cpp_inst){
            capture_temp_value.ccp_low = CCPR1L;
            capture_temp_value.ccp_high = CCPR1H;
            *capture_value = capture_temp_value.ccp_16Bit;
            ret = E_OK;
        }else if(CCP2_INST == _ccp_obj->cpp_inst){
            capture_temp_value.ccp_low = CCPR2L;
            capture_temp_value.ccp_high = CCPR2H;
            *capture_value = capture_temp_value.ccp_16Bit;
            ret = E_OK;
        
        }else{ ret = E_NOT_OK; } 
    }
    return ret;
}

#endif  

#if ( CCP_CFG_SELECTED_MODE == CCP_CFG_COMPARE_MODE_SELECTED )
Std_ReturnType CCP_IsCompareComplete(const ccp_t *_ccp_obj, uint8 * _compare_status){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _compare_status){
        ret = E_NOT_OK;
    }else{
        if(CCP1_INST == _ccp_obj->cpp_inst){
            if(PIR1bits.CCP1IF == INTERRUPT_OCCUR){
            *_compare_status = CCP_COMPARE_READY;
            CCP1_InterruptFlagClear();
            }else{
                *_compare_status = CCP_COMPARE_NOT_READY;
            }
            ret = E_OK;
        }else if(CCP2_INST == _ccp_obj->cpp_inst){
            if(PIR2bits.CCP2IF == INTERRUPT_OCCUR){
            *_compare_status = CCP_COMPARE_READY;
            CCP2_InterruptFlagClear();
            }else{
                *_compare_status = CCP_COMPARE_NOT_READY;
            }
            ret = E_OK;
        }else{ret = E_NOT_OK;}   
        
        
    }
    return ret;
}

Std_ReturnType CCP_CompareModeSetValue (const ccp_t *_ccp_obj, uint16 compare_value){
    Std_ReturnType ret = E_NOT_OK;
    CCP_REG_T compare_temp_value = {.ccp_16Bit  = ZERO_INIT };
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }else{
        compare_temp_value.ccp_16Bit = compare_value;
        if(CCP1_INST == _ccp_obj->cpp_inst){
            CCPR1L = compare_temp_value.ccp_low;
            CCPR1H = compare_temp_value.ccp_high;
            ret = E_OK;
        }else if(CCP2_INST == _ccp_obj->cpp_inst){
            CCPR2L = compare_temp_value.ccp_low;
            CCPR2H = compare_temp_value.ccp_high;
            ret = E_OK;
        }else{ret = E_NOT_OK;}   
        
        
                
        
    }
    return ret;
}

#endif      

#if ( CCP_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED )
Std_ReturnType CCP_PWM_Set_Duty(const ccp_t *_ccp_obj, const uint8 _duty){
    Std_ReturnType ret = E_NOT_OK;
    uint16 l_duty_temp = ZERO_INIT;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }else{
        l_duty_temp = (uint16)( 4 * (PR2 +1) * (_duty/100.0) );
        if(CCP1_INST == _ccp_obj->cpp_inst){
            CCP1CONbits.DC1B = (uint8)(l_duty_temp & 0x0003);
            CCPR1L = (uint8)(l_duty_temp >> 2);
            ret = E_OK;
        }else if(CCP2_INST == _ccp_obj->cpp_inst){
            CCP2CONbits.DC2B = (uint8)(l_duty_temp & 0x0003);
            CCPR2L = (uint8)(l_duty_temp >> 2);
            ret = E_OK;
        }else{
           ret = E_NOT_OK; 
        } 
    }   
    return ret;
       
        
}
Std_ReturnType CCP_PWM_Start(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }else{
        if(CCP1_INST == _ccp_obj->cpp_inst){
            CCP1CONbits.CCP1M = CCP_PWM_MODE;
            ret = E_OK;
        }else if(CCP2_INST == _ccp_obj->cpp_inst){
            CCP2CONbits.CCP2M = CCP_PWM_MODE;
            ret = E_OK;
        }else{
           ret = E_NOT_OK; 
        } 
    }   
    return ret;
}

Std_ReturnType CCP_PWM_Stop(const ccp_t *_ccp_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _ccp_obj){
        ret = E_NOT_OK;
    }else{  
        if(CCP1_INST == _ccp_obj->cpp_inst){
            CCP1CONbits.CCP1M = CCP_MODULE_DISABLE;
            ret = E_OK;
        }else if(CCP2_INST == _ccp_obj->cpp_inst){
            CCP2CONbits.CCP2M = CCP_MODULE_DISABLE;
            ret = E_OK;
        }else{
           ret = E_NOT_OK; 
        } 
    }   
    return ret;
}
#endif

void CCP1_ISR(void){
    //    The CCP1 interrupt occurred (must be cleared in software)
    CCP1_InterruptFlagClear();
    //    Callback function gets called every time this ISR executes
    if(CCP1_InterruptHandler){
        CCP1_InterruptHandler();
    }else{ /* Nothing */ }
}
void CCP2_ISR(void){
    //    The CCP1 interrupt occurred (must be cleared in software)
    CCP2_InterruptFlagClear();
    //    Callback function gets called every time this ISR executes
    if(CCP2_InterruptHandler){
        CCP2_InterruptHandler();
    }else{ /* Nothing */ }
}

static void CPP_CAPTURE_MOde_Config(const ccp_t *_ccp_obj){
    if(CCP1_INST == _ccp_obj->cpp_inst){
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_CAPTURE_MODE_1_FALLING_EDGE:
                CCP1_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE); break;
            case CCP_CAPTURE_MODE_1_RISING_EDGE:
                CCP1_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);  break;
            case CCP_CAPTURE_MODE_4_RISING_EDGE:
                CCP1_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);  break;
            case CCP_CAPTURE_MODE_16_RISING_EDGE:
                CCP1_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE); break;
             default: ;}
    }else if(CCP2_INST == _ccp_obj->cpp_inst){
        switch(_ccp_obj->ccp_mode_variant){
            case CCP_CAPTURE_MODE_1_FALLING_EDGE:
                CCP2_SET_MODE(CCP_CAPTURE_MODE_1_FALLING_EDGE);  break;
            case CCP_CAPTURE_MODE_1_RISING_EDGE:
                CCP2_SET_MODE(CCP_CAPTURE_MODE_1_RISING_EDGE);  break;
            case CCP_CAPTURE_MODE_4_RISING_EDGE:
                CCP2_SET_MODE(CCP_CAPTURE_MODE_4_RISING_EDGE);  break;
            case CCP_CAPTURE_MODE_16_RISING_EDGE:
                CCP2_SET_MODE(CCP_CAPTURE_MODE_16_RISING_EDGE);  break;
            default: ;}
    }else{/* Nothing */ }   
    CCP_Mode_Timer_Select(_ccp_obj);
}

static void CPP_Compare_Mode_Config(const ccp_t *_ccp_obj){
    if(CCP1_INST == _ccp_obj->cpp_inst){
     switch(_ccp_obj->ccp_mode_variant){
        case CCP_COMPARE_MODE_SET_PIN_HIGH:
            CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);break;
        case CCP_COMPARE_MODE_SET_PIN_LOW:
            CCP1_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW); break;
        case CCP_COMPARE_MODE_TOGGLE_ON_MATCH:
            CCP1_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH);break;
        case CCP_COMPARE_MODE_GEN_SW_INTERRUPT:
            CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT); break;
        case CCP_COMPARE_MODE_GEN_EVENT:
            CCP1_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT); break;
        default:;}   
    }else if(CCP2_INST == _ccp_obj->cpp_inst){
      switch(_ccp_obj->ccp_mode_variant){
        case CCP_COMPARE_MODE_SET_PIN_HIGH:
            CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_HIGH);  break;
        case CCP_COMPARE_MODE_SET_PIN_LOW:
            CCP2_SET_MODE(CCP_COMPARE_MODE_SET_PIN_LOW);  break;
        case CCP_COMPARE_MODE_TOGGLE_ON_MATCH:
            CCP2_SET_MODE(CCP_COMPARE_MODE_TOGGLE_ON_MATCH); break;
        case CCP_COMPARE_MODE_GEN_SW_INTERRUPT:
            CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_SW_INTERRUPT); break;
        case CCP_COMPARE_MODE_GEN_EVENT:
            CCP2_SET_MODE(CCP_COMPARE_MODE_GEN_EVENT); break;
        default: ;}  
    }else{;} 
    CCP_Mode_Timer_Select(_ccp_obj);
}

static void CCP_PWM_Mode_Config(const ccp_t *_ccp_obj){
#if CCP_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED
    if((CCP_PWM_MODE == _ccp_obj->ccp_mode_variant) && (CCP1_INST == _ccp_obj->cpp_inst)){
        CCP1_SET_MODE(CCP_PWM_MODE); 
    }else if((CCP_PWM_MODE == _ccp_obj->ccp_mode_variant) && (CCP2_INST == _ccp_obj->cpp_inst)){
        CCP2_SET_MODE(CCP_PWM_MODE); 
    }else{/* Nothing */ }
            
    PR2 = (uint8)((_XTAL_FREQ / (_ccp_obj->PWM_Frequency * 4.0 * 
            _ccp_obj->ccp_timer2_prescaler_value * _ccp_obj->ccp_timer2_postscaler_value)) - 1);
#endif
}

static void CCP_Interrupt_Config(const ccp_t *_ccp_obj){
    #if CCP1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        CCP1_InterruptEnable();
        CCP1_InterruptFlagClear();
        CCP1_InterruptHandler = _ccp_obj->CCP1_InterruptHandler;
        
        #if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _ccp_obj->CCP1_priority){
           INTERRUPT_GlobalInterruptHighEnable();
           CCP1_InterruptHighPriority();
       }else if(INTERRUPT_LOW_PRIORITY == _ccp_obj->CCP1_priority){
           INTERRUPT_GlobalInterruptLowEnable();
           CCP1_InterruptLowPriority();
       }else{/* Nothing */}
        #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
#endif
#if CCP2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        CCP2_InterruptEnable();
        CCP2_InterruptFlagClear();
        CCP2_InterruptHandler = _ccp_obj->CCP2_InterruptHandler;
        
        #if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _ccp_obj->CCP2_priority){
           INTERRUPT_GlobalInterruptHighEnable();
           CCP2_InterruptHighPriority();
       }else if(INTERRUPT_LOW_PRIORITY == _ccp_obj->CCP2_priority){
           INTERRUPT_GlobalInterruptLowEnable();
           CCP2_InterruptLowPriority();
       }else{/* Nothing */}
        #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
#endif
}

static void CCP_Mode_Timer_Select(const ccp_t *_ccp_obj){
    if(CCP1_CCP2_TIMER3 == _ccp_obj->cpp_capture_timer){
        T3CONbits.T3CCP2 = 1; 
        T3CONbits.T3CCP1 = 1;
    }else if(CCP1_TIMER1_CCP2_TIMER3 == _ccp_obj->cpp_capture_timer){
        T3CONbits.T3CCP2 = 0; 
        T3CONbits.T3CCP1 = 1;
    }else if(CCP1_CCP2_TIMER1 == _ccp_obj->cpp_capture_timer){
        T3CONbits.T3CCP2 = 0; 
        T3CONbits.T3CCP1 = 0;
    }else{/* Nothing */}
}


