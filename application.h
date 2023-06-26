/* 
 * File:   application.h
 * Author: Eng A_Esam
 *
 * Created on April 28, 2023, 3:00 AM
 */

#ifndef APPLICATION_H
#define	APPLICATION_H



/* Section : Includes */
#include "ECU_Layer/ecu_layer_init.h"
#include "MCAL_Layer/Interrupt/mcal_external_interrupt.h"
#include "MCAL_Layer/EEPROM/hal_eeprom.h"
#include "MCAL_Layer/ADC/hal_adc.h"
#include "MCAL_Layer/Timer0/hal_timer0.h"
#include "MCAL_Layer/Timer1/hal_timer1.h"
#include "MCAL_Layer/Timer2/hal_timer2.h"
#include "MCAL_Layer/Timer3/hal_timer3.h"
/* Section: Macro Declarations */

/* Section: Macro Functions Declarations */

/* Section: Data Type Declarations */


/* Section: Function Declarations */
void application_intialize(void);


#endif	/* APPLICATION_H */

