/* 
 * File:   hal_adc.c
 * Author: Eng A_Esam
 *
 * Created on June 14, 2023, 11:05 PM
 */

#include "hal_adc.h"

static inline void adc_input_channel_port_configure(const adc_channel_select_t channel);
static inline void select_result_format(const adc_conf_t *_adc);
static inline void configure_voltage_refrence(const adc_conf_t *_adc);
#if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void (*ADC_InterruptHandler)(void) = NULL;
#endif
           

Std_ReturnType ADC_Init(const adc_conf_t *_adc){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _adc){
       ret = E_NOT_OK; 
    }else{
        /* Disable the ADC */
        ADC_CONVERTER_DISABLE();
        /* Configure The acquisition time */
        ADCON2bits.ACQT = _adc->acquisition_time;
        /* Configure the conversion clock */
        ADCON2bits.ADCS = _adc->conversion_clock;
        /* Configure the default channel */
        ADCON0bits.CHS = _adc->adc_channel;
        adc_input_channel_port_configure(_adc->adc_channel);
        /* Configure The Interrupt */
#if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
        
        ADC_InterruptEnable();
        ADC_InterruptFlagClear();
        
        #if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == _adc->priority){
            INTERRUPT_GlobalInterruptHighEnable();
            ADC_InterruptHighPriority(); 
        }else if(INTERRUPT_LOW_PRIORITY == _adc->priority){
            INTERRUPT_GlobalInterruptLowEnable();
            ADC_InterruptLowPriority(); 
        }else{/* Nothing */} 
        #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
        ADC_InterruptHandler = _adc->ADC_InterruptHandler;
#endif 
        /* Configure The result Format */
        select_result_format(_adc);
        /* Configure The Voltage reference */
        configure_voltage_refrence(_adc);
        /* Enable The ADC */
        ADC_CONVERTER_ENABLE();
        ret = E_OK;
        
    }
    return ret;
}
Std_ReturnType ADC_DeInit(const adc_conf_t *_adc){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _adc){
       ret = E_NOT_OK; 
    }else{
        /* Disable the ADC */
        ADC_CONVERTER_DISABLE();
        /* Disable The Interrupt */
#if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    ADC_InterruptDisable();
#endif
        
        ret = E_OK;
    }
    return ret;
}
Std_ReturnType ADC_SelectChannel(const adc_conf_t *_adc, adc_channel_select_t channel){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _adc){
       ret = E_NOT_OK; 
    }else{
        /* Configure the Select channel */
        ADCON0bits.CHS = channel;
        adc_input_channel_port_configure(channel);
        ret = E_OK;
    }
    return ret;
}
Std_ReturnType ADC_StartConversion(const adc_conf_t *_adc){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _adc){
       ret = E_NOT_OK; 
    }else{
       ADC_START_CONVERSION(); 
       ret = E_OK;
    }
    return ret;
}
Std_ReturnType ADC_IsConversionDone(const adc_conf_t *_adc, uint8 *conversion_status){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _adc) || (NULL == conversion_status)){
       ret = E_NOT_OK; 
    }else{
      *conversion_status = (uint8)(!( ADC_CONVERSION_STATUS()));  
       ret = E_OK;
    }
    return ret;
}
Std_ReturnType ADC_GetConversionResult(const adc_conf_t *_adc, 
                                        adc_result_t *conversion_result){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _adc) || (NULL == conversion_result)){
       ret = E_NOT_OK; 
    }else{
            if(_adc->result_format == ADC_RESULT_RIGHT){
                *conversion_result = (adc_result_t)((ADRESH << 8) + ADRESL);
            }else if(_adc->result_format == ADC_RESULT_LEFT){ 
                *conversion_result = (adc_result_t)(((ADRESH << 8) + ADRESL) >> 6);
            }
            else{ 
                *conversion_result = (adc_result_t)((ADRESH << 8) + ADRESL);
            }
            ret = E_OK;
    }
    return ret;
}
Std_ReturnType ADC_GetConversion_Blocking(const adc_conf_t *_adc, 
                                        adc_channel_select_t channel,
                                        adc_result_t *conversion_result){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == _adc) || (NULL == conversion_result)){
       ret = E_NOT_OK; 
    }else{
        /* Select A/D Channel */
        ret = ADC_SelectChannel(_adc, channel);
        /* Start The Conversion */
        ret = ADC_StartConversion(_adc);
        /* Check if conversion is complete */
        while(ADC_CONVERSION_STATUS());
         ret = ADC_GetConversionResult(_adc, conversion_result);
    }
    return ret;
    
}

Std_ReturnType ADC_StartConversion_Interrupt(const adc_conf_t *_adc, adc_channel_select_t channel)
{
   Std_ReturnType ret = E_NOT_OK;
    if(NULL == _adc){
       ret = E_NOT_OK; 
    }else{
        /* Select A/D Channel */
        ret = ADC_SelectChannel(_adc, channel);
        /* Start The Conversion */
        ret = ADC_StartConversion(_adc);
    }
    return ret; 
}



/* Local Functions */

static inline void adc_input_channel_port_configure(const adc_channel_select_t channel){
    switch (channel){
        case ADC_CHANNEL_AN0:
             SET_BIT(TRISA,  _TRISA_RA0_POSN);
             break;
        case ADC_CHANNEL_AN1: SET_BIT(TRISA,  _TRISA_RA1_POSN); break;
        case ADC_CHANNEL_AN2: SET_BIT(TRISA,  _TRISA_RA2_POSN); break;
        case ADC_CHANNEL_AN3: SET_BIT(TRISA,  _TRISA_RA3_POSN); break;     
        case ADC_CHANNEL_AN4: SET_BIT(TRISA,  _TRISA_RA5_POSN); break;     
        case ADC_CHANNEL_AN5: SET_BIT(TRISE,  _TRISE_RE0_POSN); break;     
        case ADC_CHANNEL_AN6: SET_BIT(TRISE,  _TRISE_RE1_POSN); break;          
        case ADC_CHANNEL_AN7: SET_BIT(TRISE,  _TRISE_RE2_POSN); break;
        case ADC_CHANNEL_AN8: SET_BIT(TRISB,  _TRISB_RB2_POSN); break;
        case ADC_CHANNEL_AN9: SET_BIT(TRISB,  _TRISB_RB3_POSN); break;
        case ADC_CHANNEL_AN10:SET_BIT(TRISB,  _TRISB_RB1_POSN); break;     
        case ADC_CHANNEL_AN11:SET_BIT(TRISB,  _TRISB_RB4_POSN); break;     
        case ADC_CHANNEL_AN12:SET_BIT(TRISB,  _TRISB_RB0_POSN); break;      
        default: ;  
    }      
}

static inline void select_result_format(const adc_conf_t *_adc){
    if(_adc->result_format == ADC_RESULT_RIGHT){
        ADC_RESULT_RIGHT_FORMAT();
    }else if(_adc->result_format == ADC_RESULT_LEFT){
        ADC_RESULT_LEFT_FORMAT();
    }else{ /* */ }
    
}

static inline void configure_voltage_refrence(const adc_conf_t *_adc){
    if(_adc->voltage_refrence == ADC_VOLTAGE_REFRENCE_ENABLE){
        ADC_ENABLE_VOLTAGE_REFRENCE();
    }else if(_adc->voltage_refrence == ADC_VOLTAGE_REFRENCE_DISABLE){
        ADC_DISABLE_VOLTAGE_REFRENCE();
    }else{ /* */ }
    
}

void ADC_ISR(void){
//    The ADC interrupt occurred (must be cleared in software)
    ADC_InterruptFlagClear();
//    Code
    
//    Callback function gets called every time this ISR executes
    if(ADC_InterruptHandler){
        ADC_InterruptHandler();
}
    
    
}