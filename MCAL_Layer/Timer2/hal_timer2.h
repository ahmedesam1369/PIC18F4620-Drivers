/* 
 * File:   hal_timer2.h
 * Author: Eng A_Esam
 *
 * Created on June 25, 2023, 12:38 PM
 */

#ifndef HAL_TIMER2_H
#define	HAL_TIMER2_H
/* Section : Includes  */
#include "../Interrupt/mcal_internal_interrupt.h"
/* Section : Macro Declarations */
/* Timer2 Input Clock Pre-Scaler */
#define  TIMER2_PRESCALLER_DIV_BY_1   0
#define  TIMER2_PRESCALLER_DIV_BY_4   1
#define  TIMER2_PRESCALLER_DIV_BY_16  2
/* Timer2 Input Clock Post-Scaler */
#define  TIMER2_POSTSCALLER_DIV_BY_1    0
#define  TIMER2_POSTSCALLER_DIV_BY_2    1
#define  TIMER2_POSTSCALLER_DIV_BY_3    2
#define  TIMER2_POSTSCALLER_DIV_BY_4    3
#define  TIMER2_POSTSCALLER_DIV_BY_5    4
#define  TIMER2_POSTSCALLER_DIV_BY_6    5
#define  TIMER2_POSTSCALLER_DIV_BY_7    6
#define  TIMER2_POSTSCALLER_DIV_BY_8    7
#define  TIMER2_POSTSCALLER_DIV_BY_9    8
#define  TIMER2_POSTSCALLER_DIV_BY_10   9
#define  TIMER2_POSTSCALLER_DIV_BY_11   10
#define  TIMER2_POSTSCALLER_DIV_BY_12   11
#define  TIMER2_POSTSCALLER_DIV_BY_13   12
#define  TIMER2_POSTSCALLER_DIV_BY_14   12
#define  TIMER2_POSTSCALLER_DIV_BY_15   14
#define  TIMER2_POSTSCALLER_DIV_BY_16   15
/* Section: Macro Functions Declarations */
/*  Enable or Disable Timer1 Module       */
#define TIMER2_MODULE_ENABLE()             (T2CONbits.TMR2ON = 1)
#define TIMER2_MODULE_DISABLE()            (T2CONbits.TMR2ON = 0)
/* Timer2 Input Clock Pre-Scaler */
#define TIMER2_PRESCALER_SELECT(_Prescaler_)  (T2CONbits.T2CKPS = _Prescaler_)
/* Timer2 Input Clock Post-Scaler */
#define TIMER2_POSTSCALER_SELECT(_PostScaler)  (T2CONbits.TOUTPS = _PostScaler)
/* Section : Data Type Declarations*/
typedef struct{
#if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    void (* Timer2_InterruptHandler)(void);
    interrupt_priority_cfg priority;
#endif
    uint8 timer1_preload_value;
    uint8  timer1_postscaler_value : 4;
    uint8  timer1_prescaler_value : 2;
    uint8  timer1_reserved     : 2;
}timer2_t;

/* Section : Software Interfaces  Declarations */
Std_ReturnType Timer2_Init(const timer2_t *_timer);
Std_ReturnType Timer2_DeInit(const timer2_t *_timer);
Std_ReturnType Timer2_Write_Value(const timer2_t *_timer, uint8 _value);
Std_ReturnType Timer2_Read_Value(const timer2_t *_timer, uint8 *_value);

void TIMER2_ISR(void);


#endif	/* HAL_TIMER2_H */

