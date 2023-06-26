/* 
 * File:   hal_eeprom.c
 * Author: Eng A_Esam
 *
 * Created on May 31, 2023, 6:36 PM
 */
#include "hal_eeprom.h"

Std_ReturnType Data_EEPROM_Write_Byte(uint16 bAdd, uint8 bData){
    Std_ReturnType ret = E_OK;
    /*  Read The Interrupt Status Enable Or Disable */
    uint8 Global_Interrupt_Status = INTCONbits.GIEH;
    /* Update The Address Registers */
    EEADRH = (uint8)((bAdd >> 8) & 0x03);
    EEADR  = (uint8)(bAdd & 0xFF);
    /* Update The Data Registers */
    EEDATA = bData;
    /* Select Access Data EEPROM Memory */
    EECON1bits.EEPGD = Access_EEPROM_Program_Memory;
    EECON1bits.CFGS = Access_Flash_EEPROM_Memory;
    /* Allow Write Cycle to Flash Program/Data EEPROM */
    EECON1bits.WREN = Allow_Write_Cycle_Flash_EEPROM;
    /* Disable All interrupts General Interrupt */
    GlobalInterruptDisable();
    /* Write The Required Sequence : 0x55 -> 0xAA */
    EECON2 = 0x55;
    EECON2 = 0xAA;
    /* Initiate The Data EEPROMerase/write Cycle */
    EECON1bits.WR = Initiate_Data_EEPROM_Write_Erase;
    /* Wait for Write to complete */
    while(EECON1bits.WR);
    /* Inhibits Write Cycles to flash program/data EEPROM */
    EECON1bits.WREN = Inhibts_Write_Cycle_Flash_EEPROM;
    /* Restore The interrupt status "Enable or Disable */
    INTCONbits.GIEH = Global_Interrupt_Status;
    
    return ret;
}
Std_ReturnType Data_EEPROM_read_Byte(uint16 bAdd,  uint8 *bData){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == bData){
        ret = E_NOT_OK;
    }else{
        /* Update The Address Registers */
        EEADRH = ((bAdd >> 8) & 0x03);
        EEADR  = (bAdd & 0xFF);
        /* Select Access Data EEPROM */
        EECON1bits.CFGS = Access_Flash_EEPROM_Memory;
        EECON1bits.EEPGD = Access_EEPROM_Program_Memory;
        /* Initiate a Data EEPROM Read Cycle */
        EECON1bits.RD = Initiate_Data_EEPROM_Read;
        /* NOPs May be required for latency at high frequencies*/
        NOP();
        /* NOPs May be required for latency at high frequencies*/
        NOP();
        
        *bData = EEDATA;
                
        ret = E_OK;
    }
    
    return ret;
}
