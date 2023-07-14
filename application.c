/* 
 * File:   application.c
 * Author: Eng A_Esam
 *
 * Created on April 26, 2023, 2:51 PM
 */
#include "application.h"
#define MASTER  0
#define SLAVE_1 1
#define SLAVE_2 2
#define MICROCONTROLER_SELECT   SLAVE_2

mssp_i2c_t i2c_obj;
volatile uint8 slave_ack;
#define slave_1   0x60     /* Address = 0x30 + W bit(0) = 0x60*/
#define slave_2   0x62     /* Address = 0x31 + W bit(0) = 0x62*/
uint32 app_counter;
static volatile uint8 i2c_slave_1_rec, i2c_slave_2rec;
void MSSP_I2C_DefaultInterruptHandler(){
#if MICROCONTROLER_SELECT == MASTER
#elif MICROCONTROLER_SELECT == SLAVE_1
    /* Read Data */
    MSSP_I2C_SLAVE_Read_Write(&i2c_obj, &i2c_slave_1_rec);
    
#elif MICROCONTROLER_SELECT == SLAVE_2
   /* Read Data */
    MSSP_I2C_SLAVE_Read_Write(&i2c_obj, &i2c_slave_2rec);
#else
#endif   
}
void MSSP_I2C_Send_1Byte(uint8 slave_add, uint8 _data){
    Std_ReturnType ret = E_NOT_OK;
    ret = MSSP_I2C_Master_Send_Start(&i2c_obj);
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, slave_add, &slave_ack);
    ret = MSSP_I2C_Master_Write_Blocking(&i2c_obj, _data, &slave_ack);
    ret = MSSP_I2C_Master_Send_Stop(&i2c_obj); 
}

int main() {
    Std_ReturnType ret = E_NOT_OK;
    application_intialize();
    
#if MICROCONTROLER_SELECT == MASTER
    i2c_obj.i2c_clock = 100000;
    i2c_obj.i2c_cfg.i2c_mode = MSSP_I2C_MASTER_MODE;
    i2c_obj.i2c_cfg.i2c_mode_cfg = MSSP_I2C_MASTER_MODE_DEFINED_CLOCK;
    i2c_obj.i2c_cfg.i2c_slew_rate = I2C_SLEW_RATE_DISABLE;
    i2c_obj.i2c_cfg.i2c_SMBus_control = I2C_SMBus_DISABLE;
    #if MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    i2c_obj.I2C_DefaultInterruptHandler = NULL;
    i2c_obj.I2C_Report_Write_Collision = NULL;
    i2c_obj.I2C_Report_Receive_Overflow = NULL;
    #endif
#elif MICROCONTROLER_SELECT == SLAVE_1
    i2c_obj.i2c_cfg.i2c_mode = MSSP_I2C_SLAVE_MODE;
    i2c_obj.i2c_cfg.i2c_slave_address = slave_1;
    i2c_obj.i2c_cfg.i2c_mode_cfg = MSSP_I2C_SLAVE_MODE_7BIT_ADDRESS;
    i2c_obj.i2c_cfg.i2c_slew_rate = I2C_SLEW_RATE_DISABLE;
    i2c_obj.i2c_cfg.i2c_SMBus_control = I2C_SMBus_DISABLE;
    i2c_obj.i2c_cfg.i2c_general_call = I2C_GENERAL_CALL_DISABLE;
    #if MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    i2c_obj.I2C_DefaultInterruptHandler = MSSP_I2C_DefaultInterruptHandler;
    i2c_obj.I2C_Report_Write_Collision = NULL;
    i2c_obj.I2C_Report_Receive_Overflow = NULL;
    #endif
#elif MICROCONTROLER_SELECT == SLAVE_2
   i2c_obj.i2c_cfg.i2c_mode = MSSP_I2C_SLAVE_MODE;
    i2c_obj.i2c_cfg.i2c_slave_address = slave_2;
    i2c_obj.i2c_cfg.i2c_mode_cfg = MSSP_I2C_SLAVE_MODE_7BIT_ADDRESS;
    i2c_obj.i2c_cfg.i2c_slew_rate = I2C_SLEW_RATE_DISABLE;
    i2c_obj.i2c_cfg.i2c_SMBus_control = I2C_SMBus_DISABLE;
    i2c_obj.i2c_cfg.i2c_general_call = I2C_GENERAL_CALL_DISABLE;
    #if MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    i2c_obj.I2C_DefaultInterruptHandler = MSSP_I2C_DefaultInterruptHandler;
    i2c_obj.I2C_Report_Write_Collision = NULL;
    i2c_obj.I2C_Report_Receive_Overflow = NULL;
    #endif
#else
#endif
    
    
    ret = MSSP_I2C_Init(&i2c_obj);

    extern led_t led_D_0;
    
    while(1){  
        app_counter++;
#if MICROCONTROLER_SELECT == MASTER
        __delay_ms(1000);
        MSSP_I2C_Send_1Byte(slave_1, 'a');
        __delay_ms(1000);
        MSSP_I2C_Send_1Byte(slave_2, 'b');
        __delay_ms(1000);
        MSSP_I2C_Send_1Byte(slave_1, 'c');
        __delay_ms(1000);
        MSSP_I2C_Send_1Byte(slave_2, 'd');    
#elif MICROCONTROLER_SELECT == SLAVE_1
        if(i2c_slave_1_rec == 'a'){
          led_turn_on(&led_D_0);  
        }else if(i2c_slave_1_rec == 'c'){
            led_turn_off(&led_D_0);
        }else{}
#elif MICROCONTROLER_SELECT == SLAVE_2
        if(i2c_slave_2rec == 'b'){
          led_turn_on(&led_D_0);  
        }else if(i2c_slave_2rec == 'd'){
            led_turn_off(&led_D_0);
        }else{}
   
#else
#endif
        
    }
    return (EXIT_SUCCESS);

}

void application_intialize(void){
    Std_ReturnType ret = E_NOT_OK;
    ecu_layer_initialize();
}


