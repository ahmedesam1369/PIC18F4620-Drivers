/* 
 * File:   mcal_external_interrupt.h
 * Author: Eng A_Esam
 *
 * Created on May 29, 2023, 9:51 PM
 */

#ifndef MCAL_EXTERNAL_INTERRUPT_H
#define	MCAL_EXTERNAL_INTERRUPT_H

/* --------------------------------  Includes  --------------------------------*/
#include "mcal_interrupt_config.h"

/* ---------------------------  Macro Declarations  ---------------------------*/


/* ----------------------  Macro Functions Declarations  ----------------------*/

#if EXTERNAL_INTERRUPT_INTX_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE

//This routine sets the interrupt enable for the external interrupt, INT0
#define EXT_INT0_InterruptEnable()   (INTCONbits.INT0E  = 1)
//This routine clears the interrupt enable for the external interrupt, INT0
#define EXT_INT0_InterruptDisable()   (INTCONbits.INT0E  = 0)
//This routine clears the interrupt flag for the external interrupt, INT0
#define EXT_INT0_InterruptFlagClear()   (INTCONbits.INT0IF  = 0)
//This routine set the edge detect of the external interrupt to positive edge, INT0
#define EXT_INT0_RisingEdgeSet()   (INTCON2bits.INTEDG0  = 1)
//This routine set the edge detect of the external interrupt to negative edge, INT0
#define EXT_INT0_FallingEdgeSet()   (INTCON2bits.INTEDG0  = 0)

//This routine sets the interrupt enable for the external interrupt, INT1
#define EXT_INT1_InterruptEnable()   (INTCON3bits.INT1E  = 1)
//This routine clears the interrupt enable for the external interrupt, INT1
#define EXT_INT1_InterruptDisable()   (INTCON3bits.INT1E  = 0)
//This routine clears the interrupt flag for the external interrupt, INT1
#define EXT_INT1_InterruptFlagClear()   (INTCON3bits.INT1IF  = 0)
//This routine set the edge detect of the external interrupt to positive edge, INT1
#define EXT_INT1_RisingEdgeSet()   (INTCON2bits.INTEDG1  = 1)
//This routine set the edge detect of the external interrupt to negative edge, INT1
#define EXT_INT1_FallingEdgeSet()   (INTCON2bits.INTEDG1  = 0)

//This routine sets the interrupt enable for the external interrupt, INT2
#define EXT_INT2_InterruptEnable()   (INTCON3bits.INT2IE  = 1)
//This routine clears the interrupt enable for the external interrupt, INT2
#define EXT_INT2_InterruptDisable()   (INTCON3bits.INT2IE  = 0)
//This routine clears the interrupt flag for the external interrupt, INT2
#define EXT_INT2_InterruptFlagClear()   (INTCON3bits.INT2IF  = 0)
//This routine set the edge detect of the external interrupt to positive edge, INT2
#define EXT_INT2_RisingEdgeSet()   (INTCON2bits.INTEDG2  = 1)
//This routine set the edge detect of the external interrupt to negative edge, INT2
#define EXT_INT2_FallingEdgeSet()   (INTCON2bits.INTEDG2  = 0)

#if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
// This macro will Enable high priority interrupts for INT1
#define EXT_INT1_InterruptHighPriority()   (INTCON3bits.INT1P =  1)
// This macro will Enable low priority interrupts for INT1
#define EXT_INT1_InterruptLowPriority()   (INTCON3bits.INT1P =  0)
// This macro will Enable high priority interrupts for INT2
#define EXT_INT2_InterruptHighPriority()   (INTCON3bits.INT2IP =  1)
// This macro will Enable low priority interrupts for INT2
#define EXT_INT2_InterruptLowPriority()   (INTCON3bits.INT2IP =  0)
#endif

#endif

#if EXTERNAL_INTERRUPT_onChange_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
//This routine sets the interrupt enable for the external interrupt, RBX
#define EXT_RBX_InterruptEnable()   (INTCONbits.RBIE  = 1)
//This routine clears the interrupt enable for the external interrupt, RBX
#define EXT_RBX_InterruptDisable()   (INTCONbits.RBIE  = 0)
//This routine clears the interrupt flag for the external interrupt, RBX
#define EXT_RBX_InterruptFlagClear()   (INTCONbits.RBIF  = 0)
 
#if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
// This macro will Enable high priority interrupts for RBX
#define EXT_RBX_InterruptHighPriority()   (INTCON2bits.RBIP =  1)
// This macro will Enable low priority interrupts for RBX
#define EXT_RBX_InterruptLowPriority()   (INTCON2bits.RBIP =  0)
#endif

#endif

/* -------------------------  Data Type Declarations  -------------------------*/
typedef enum{
    INTERRUPT_FALLING_EDGE = 0,
    INTERRUPT_RISING_EDGE      
    
}interrupt_INTx_edge;

typedef enum{
    INTERRUPT_EXTERNAL_INT0 = 0,
    INTERRUPT_EXTERNAL_INT1,
    INTERRUPT_EXTERNAL_INT2
}interrupt_INTx_src;

typedef struct{
    void (* EXT_InterruptHandler)(void);
    pin_config_t mcu_pin;
    interrupt_INTx_edge edge;
    interrupt_INTx_src source;
    interrupt_priority_cfg priority;
}interrupt_INTx_t;

typedef struct{
    void (* EXT_InterruptHandlerHigh)(void);
    void (* EXT_InterruptHandlerLow)(void);
    pin_config_t mcu_pin;
    interrupt_priority_cfg priority;
}interrupt_RBx_t;


/* --------------------------  Function Declarations  -------------------------*/
Std_ReturnType NTERUPT_INTx_Init(const interrupt_INTx_t *int_obj);
Std_ReturnType NTERUPT_INTx_DEInit(const interrupt_INTx_t *int_obj);
Std_ReturnType NTERUPT_RBx_Init(const interrupt_RBx_t *rb_obj);
Std_ReturnType NTERUPT_RBx_DEInit(const interrupt_RBx_t *rb_obj);

void INT0_ISR(void);
void INT1_ISR(void);
void INT2_ISR(void);
void RB4_ISR(uint8 RB4_Source);
void RB5_ISR(uint8 RB5_Source);
void RB6_ISR(uint8 RB6_Source);
void RB7_ISR(uint8 RB7_Source);
#endif	/* MCAL_EXTERNAL_INTERRUPT_H */

