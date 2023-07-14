/* 
 * File:   hal_ccp1.h
 * Author: Eng A_Esam
 *
 * Created on June 26, 2023, 2:09 PM
 */

#ifndef HAL_CCP_H
#define	HAL_CCP_H
/*------------------------------ Includes ------------------------------*/
#include "hal_cpp_cfg.h"
#include "../Interrupt/mcal_internal_interrupt.h"
/*------------------------- Macro Declaration --------------------------*/
#define CCP_MODULE_DISABLE                 ((uint8)0x00)
#define CCP_CAPTURE_MODE_1_FALLING_EDGE    ((uint8)0X04)
#define CCP_CAPTURE_MODE_1_RISING_EDGE     ((uint8)0X05)
#define CCP_CAPTURE_MODE_4_RISING_EDGE     ((uint8)0X06)
#define CCP_CAPTURE_MODE_16_RISING_EDGE    ((uint8)0X07)
#define CCP_COMPARE_MODE_SET_PIN_HIGH      ((uint8)0x08)
#define CCP_COMPARE_MODE_SET_PIN_LOW       ((uint8)0x09)
#define CCP_COMPARE_MODE_TOGGLE_ON_MATCH   ((uint8)0x02)
#define CCP_COMPARE_MODE_GEN_SW_INTERRUPT  ((uint8)0x0A)
#define CCP_COMPARE_MODE_GEN_EVENT         ((uint8)0x0B)
#define CCP_PWM_MODE                       ((uint8)0x0C)

/* CCP Capture Mode State  */ 
#define CCP_CAPTURE_NOT_READY         0x00
#define CCP_CAPTURE_READY             0x01

/* CCP1 Compare MODE State */
#define CCP_COMPARE_NOT_READY         0x00
#define CCP_COMPARE_READY             0x01

/* CCP1_Timer2 Input Clock Pre-Scaler */
#define  CCP_TIMER2_PRESCALLER_DIV_BY_1     1
#define  CCP_TIMER2_PRESCALLER_DIV_BY_4     4
#define  CCP_TIMER2_PRESCALLER_DIV_BY_16    16
/* CCP1_Timer2 Input Clock Post-Scaler */
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_1    1
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_2    2
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_3    3
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_4    4
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_5    5
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_6    6
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_7    7
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_8    8
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_9    9
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_10   10
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_11   11
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_12   12
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_13   13
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_14   14
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_15   15
#define  CCP_TIMER2_POSTSCALLER_DIV_BY_16   16

/*--------------------- Macro Functions Declaration --------------------*/
#define CCP1_SET_MODE(_CONFIG)       (CCP1CONbits.CCP1M = _CONFIG)
#define CCP2_SET_MODE(_CONFIG)       (CCP2CONbits.CCP2M = _CONFIG)
/*------------------------ Data Type Declarations ----------------------*/
typedef union{
    struct{
        uint8 ccp_low;
        uint8 ccp_high;
    };
    struct{
        uint16 ccp_16Bit;
    };
}CCP_REG_T;

typedef enum{
    CCP_CAPTURE_MODE_SELECTED  = 0,
    CCP_COMPARE_MODE_SELECTED,
    CCP_PWM_MODE_SELECTED     
}ccp_mode_t;

typedef enum{
    CCP1_INST = 0,
    CCP2_INST
}ccp_inst_t;

typedef enum{
    CCP1_CCP2_TIMER3 = 0,
    CCP1_TIMER1_CCP2_TIMER3, 
    CCP1_CCP2_TIMER1
}cpp_capture_timer_t;

typedef struct{
    ccp_inst_t cpp_inst;
    ccp_mode_t ccp_mode;
    uint8 ccp_mode_variant;
    pin_config_t ccp_pin;
    cpp_capture_timer_t cpp_capture_timer;
    
    #if CCP1_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        void (* CCP1_InterruptHandler)(void);
        interrupt_priority_cfg CCP1_priority;
    #endif  
    #if CCP2_INTERRUPT_FEATURE_ENABLE==INTERRUPT_FEATURE_ENABLE
        void (* CCP2_InterruptHandler)(void);
        interrupt_priority_cfg CCP2_priority;
    #endif  
    #if ( CCP_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED )
        uint32 PWM_Frequency;
        uint8  ccp_timer2_postscaler_value ;
        uint8  ccp_timer2_prescaler_value ;
    #endif    
    
}ccp_t;

/*------------------- Software Interfaces  Declarations -----------------*/

Std_ReturnType CCP_Init(const ccp_t *_ccp_obj);
Std_ReturnType CCP_DeInit(const ccp_t *_ccp_obj);

#if ( CCP_CFG_SELECTED_MODE == CCP_CFG_CAPTURE_MODE_SELECTED )
Std_ReturnType CCP_IsCapturedDataReady(const ccp_t *_ccp_obj, uint8 * _capture_status);
Std_ReturnType CCP_CaptureModeReadValue(const ccp_t *_ccp_obj, uint16 * capture_value);
#endif  
#if ( CCP_CFG_SELECTED_MODE == CCP_CFG_COMPARE_MODE_SELECTED )
Std_ReturnType CCP_IsCompareComplete(const ccp_t *_ccp_obj, uint8 * _compare_status);
Std_ReturnType CCP_CompareModeSetValue (const ccp_t *_ccp_obj, uint16 compare_value);
#endif      
#if ( CCP_CFG_SELECTED_MODE == CCP_CFG_PWM_MODE_SELECTED )
Std_ReturnType CCP_PWM_Set_Duty(const ccp_t *_ccp_obj, const uint8 _duty);
Std_ReturnType CCP_PWM_Start(const ccp_t *_ccp_obj);
Std_ReturnType CCP_PWM_Stop(const ccp_t *_ccp_obj);
#endif    

void CCP1_ISR(void);
void CCP2_ISR(void);

#endif	/* HAL_CCP1_H */

