/* 
 * File:   mcal_interrupt_config.h
 * Author: Eng A_Esam
 *
 * Created on May 29, 2023, 9:45 PM
 */

#ifndef MCAL_INTERRUPT_CONFIG_H
#define	MCAL_INTERRUPT_CONFIG_H

/* --------------------------------  Includes  --------------------------------*/
#include <pic18f4620.h>
#include "../mcal_std_types.h"
#include "mcal_interrupt_gen_cfg.h"
#include "../GPIO/hal_gpio.h"

/* ---------------------------  Macro Declarations  ---------------------------*/
#define INTERRUPT_ENABLE            1
#define INTERRUPT_DISABLE           0
#define INTERRUPT_OCCUR             1
#define INTERRUPT_NOT_OCCUR         0
#define INTERRUPT_PRIORITY_ENABLE   1
#define INTERRUPT_PRIORITY_DISABLE  0

/* ----------------------  Macro Functions Declarations  ----------------------*/
// This macro will Enable priority level on interrupts
#define INTERRUPT_PriorityLevelsEnable()     (RCONbits.IPEN = 1)
// This macro will Disable priority level on interrupts(Compatibility mode)
#define INTERRUPT_PriorityLevelsDisable()     (RCONbits.IPEN = 0)

#if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
// priority level Enabled
// This macro will Enable high priority global interrupts
#define INTERRUPT_GlobalInterruptHighEnable()   (INTCONbits.GIEH =  1)
// This macro will Disable high priority global interrupts
#define INTERRUPT_GlobalInterruptHighDisable()   (INTCONbits.GIEH =  0)
// This macro will Enable low priority global interrupts
#define INTERRUPT_GlobalInterruptLowEnable()   (INTCONbits.GIEL =  1)
// This macro will Disable low priority global interrupts
#define INTERRUPT_GlobalInterruptLowDisable()   (INTCONbits.GIEL =  0)

#else
// priority level Disabled (Compatibility mode)
// This macro will Enable Global Interrupt Enable
#define INTERRUPT_GlobalInterruptEnable()  (INTCONbits.GIE =  1)
// This macro will Disable Global Interrupt Enable
#define INTERRUPT_GlobalInterruptDisable()  (INTCONbits.GIE =  0)
// This macro will Enable peripheral interrupts
#define INTERRUPT_PeripheralInterruptEnable()  (INTCONbits.PEIE =  1)
// This macro will Disable peripheral interrupts
#define INTERRUPT_PeripheralInterruptDisable()  (INTCONbits.PEIE =  0)

#endif
/* -------------------------  Data Type Declarations  -------------------------*/

typedef enum{
     INTERRUPT_LOW_PRIORITY = 0,
    INTERRUPT_HIGH_PRIORITY,
}interrupt_priority_cfg;

/* --------------------------  Function Declarations  -------------------------*/


#endif	/* MCAL_INTERRUPT_CONFIG_H */

