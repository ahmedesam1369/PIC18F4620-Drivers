/* 
 * File:   mcal_internal_interrupt.h
 * Author: Eng A_Esam
 *
 * Created on May 29, 2023, 9:50 PM
 */

#ifndef MCAL_INTERNAL_INTERRUPT_H
#define	MCAL_INTERNAL_INTERRUPT_H

/* --------------------------------  Includes  --------------------------------*/
#include "mcal_interrupt_config.h"
/* ---------------------------  Macro Declarations  ---------------------------*/
/*------------------ ADC -------------------------*/
#if ADC_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
//This routine clears the interrupt enable for ADC  Module
#define ADC_InterruptDisable()   (PIE1bits.ADIE = 0)
//This routine sets the interrupt enable for ADC  Module
#define ADC_InterruptEnable()   (PIE1bits.ADIE = 1)
//This routine clears the interrupt flag for ADC  Module
#define ADC_InterruptFlagClear()   (PIR1bits.ADIF  = 0)
#if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
// This macro will Enable high priority for ADC  Module
#define ADC_InterruptHighPriority()   (IPR1bits.ADIP =  1)
// This macro will Enable low priority for ADC  Module
#define ADC_InterruptLowPriority()   (IPR1bits.ADIP =  0)
#endif
#endif
/*------------------ TIMER0 -------------------------*/
#if TIMER0_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
//This routine clears the interrupt enable for TIMER0  Module
#define TIMER0_InterruptDisable()   (INTCONbits.TMR0IE = 0)
//This routine sets the interrupt enable for TIMER0  Module
#define TIMER0_InterruptEnable()   (INTCONbits.TMR0IE = 1)
//This routine clears the interrupt flag for TIMER0  Module
#define TIMER0_InterruptFlagClear()   (INTCONbits.TMR0IF  = 0)
#if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
// This macro will Enable high priority for TIMER0  Module
#define TIMER0_InterruptHighPriority()   (INTCON2bits.TMR0IP =  1)
// This macro will Enable low priority for TIMER0  Module
#define TIMER0_InterruptLowPriority()   (INTCON2bits.TMR0IP =  0)
#endif
#endif
/*------------------ TIMER1 -------------------------*/
#if TIMER1_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
//This routine clears the interrupt enable for TIMER1  Module
#define TIMER1_InterruptDisable()   (PIE1bits.TMR1IE = 0)
//This routine sets the interrupt enable for TIMER1  Module
#define TIMER1_InterruptEnable()   (PIE1bits.TMR1IE = 1)
//This routine clears the interrupt flag for TIMER1  Module
#define TIMER1_InterruptFlagClear()   (PIR1bits.TMR1IF = 0)
#if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
// This macro will Enable high priority for TIMER1  Module
#define TIMER1_InterruptHighPriority()   (IPR1bits.TMR1IP = 1)
// This macro will Enable low priority for TIMER1  Module
#define TIMER1_InterruptLowPriority()   (IPR1bits.TMR1IP = 0)
#endif
#endif
/*------------------ TIMER2 -------------------------*/
#if TIMER2_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
//This routine clears the interrupt enable for TIMER2  Module
#define TIMER2_InterruptDisable()        (PIE1bits.TMR2IE = 0)
//This routine sets the interrupt enable for TIMER2  Module
#define TIMER2_InterruptEnable()         (PIE1bits.TMR2IE = 1)
//This routine clears the interrupt flag for TIMER2  Module
#define TIMER2_InterruptFlagClear()      (PIR1bits.TMR2IF = 0)
#if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
// This macro will Enable high priority for TIMER2  Module
#define TIMER2_InterruptHighPriority()   (IPR1bits.TMR2IP = 1)
// This macro will Enable low priority for TIMER2  Module
#define TIMER2_InterruptLowPriority()    (IPR1bits.TMR2IP = 0)
#endif
#endif
/*------------------ TIMER3 -------------------------*/
#if TIMER3_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
//This routine clears the interrupt enable for TIMER3  Module
#define TIMER3_InterruptDisable()        (PIE2bits.TMR3IE = 0)
//This routine sets the interrupt enable for TIMER3  Module
#define TIMER3_InterruptEnable()         (PIE2bits.TMR3IE = 1)
//This routine clears the interrupt flag for TIMER3  Module
#define TIMER3_InterruptFlagClear()      (PIR2bits.TMR3IF = 0)
#if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
// This macro will Enable high priority for TIMER3  Module
#define TIMER3_InterruptHighPriority()   (IPR2bits.TMR3IP = 1)
// This macro will Enable low priority for TIMER3  Module
#define TIMER3_InterruptLowPriority()    (IPR2bits.TMR3IP = 0)
#endif
#endif
/* ----------------------  Macro Functions Declarations  ----------------------*/

/* -------------------------  Data Type Declarations  -------------------------*/

/* --------------------------  Function Declarations  -------------------------*/

#endif	/* MCAL_INTERNAL_INTERRUPT_H */

