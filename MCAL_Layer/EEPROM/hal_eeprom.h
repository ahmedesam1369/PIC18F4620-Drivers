/* 
 * File:   hal_eeprom.h
 * Author: Eng A_Esam
 *
 * Created on May 31, 2023, 6:36 PM
 */

#ifndef HAL_EEPROM_H
#define	HAL_EEPROM_H

/* --------------------------------  Includes  --------------------------------*/
#include <pic18f4620.h>
#include "../std_libraries.h"
#include "../Interrupt/mcal_internal_interrupt.h"
/* ---------------------------  Macro Declarations  ---------------------------*/
/* Flash Program or Data EEPROM Memory Select */
#define Access_Flash_Program_Memory       1
#define Access_EEPROM_Program_Memory      0
/* Flash Program/Data EEPROM or Configuration Select */
#define Access_Config_Registers           1
#define Access_Flash_EEPROM_Memory        0
/* Flash Program/Data Write Enable */
#define Allow_Write_Cycle_Flash_EEPROM    1
#define Inhibts_Write_Cycle_Flash_EEPROM  0
/* Write Control */
#define Initiate_Data_EEPROM_Write_Erase  1
#define Data_EEPROM_Write_Erase_Completed 0
/* Read Control */
#define Initiate_Data_EEPROM_Read         1

/* ----------------------  Macro Functions Declarations  ----------------------*/
#define GlobalInterruptEnable()  (INTCONbits.GIE =  1)
#define GlobalInterruptDisable()  (INTCONbits.GIE =  0)
/* -------------------------  Data Type Declarations  -------------------------*/

/* --------------------------  Function Declarations  -------------------------*/
Std_ReturnType Data_EEPROM_Write_Byte(uint16 bAdd, uint8 bData);
Std_ReturnType Data_EEPROM_read_Byte(uint16 bAdd,  uint8 *bData);

#endif	/* HAL_EEPROM_H */

