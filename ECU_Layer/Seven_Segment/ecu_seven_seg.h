/* 
 * File:   ecu_seven_seg.h
 * Author: Eng A_Esam
 *
 * Created on April 26, 2023, 3:01 AM
 */

#ifndef ECU_SEVEN_SEG_H
#define	ECU_SEVEN_SEG_H


/* Section : Includes  */
#include "ecu_seven_seg_cfg.h"
#include "../../MCAL_Layer/GPIO/hal_gpio.h"
/* Section : Macro Declarations */

#define SEGMENT_PIN0 0
#define SEGMENT_PIN1 1
#define SEGMENT_PIN2 2
#define SEGMENT_PIN3 3

/* Section: Macro Functions Declarations */

/* Section : Data Type Declarations*/
typedef enum{
    Segment_Common_Anode,
    Segment_Common_Cathode
}segment_type_t;

typedef struct{
    pin_config_t segment_pins[4];
    segment_type_t  segment_type;
}segment_t;
/* Section : Function Declarations */

Std_ReturnType Seven_Segment_Initialize(const segment_t *seg);
Std_ReturnType Seven_Segment_Write_Number(const segment_t *seg, uint8 number);


#endif	/* ECU_SEVEN_SEG_H */

