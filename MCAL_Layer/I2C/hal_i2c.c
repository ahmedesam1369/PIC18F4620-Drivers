/* 
 * File:   hal_i2c.c
 * Author: Eng A_Esam
 *
 * Created on July 14, 2023, 1:26 AM
 */
#include "hal_i2c.h"
#if MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
static void (* I2CInterruptHandler)(void) = NULL;
static void (* I2C_BUS_COLInterruptHandler)(void) = NULL;
static void (* I2C_Report_Receive_OverFlowInterruptHandler)(void) = NULL;
#endif
static inline void MSSP_I2C_Interrupt_Configuration(const mssp_i2c_t *i2c_obj);

Std_ReturnType MSSP_I2C_Init(const mssp_i2c_t *i2c_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }else{
        /* Disable MSSP I2C Module */
        MSSP_MODULE_Disable_CFG();
        /* MSSP I2C Select Mode */
        if(MSSP_I2C_MASTER_MODE == i2c_obj->i2c_cfg.i2c_mode ){
            /* I2C Master mode Clock Configuration */
            SSPADD = (uint8)(((_XTAL_FREQ / 4.0) / i2c_obj->i2c_clock) - 1);
        }else if(MSSP_I2C_SLAVE_MODE == i2c_obj->i2c_cfg.i2c_mode){
            /* I2C Slave mode General Call Configuration */
            if(I2C_GENERAL_CALL_ENABLE == i2c_obj->i2c_cfg.i2c_general_call){
                I2C_GENERAL_CALL_ENABLE_CFG();
            }else if(I2C_GENERAL_CALL_DISABLE == i2c_obj->i2c_cfg.i2c_general_call){
                I2C_GENERAL_CALL_DISABLE_CFG();
            }else{/* Nothing */} 
            /* Clear the Write Collision Detect*/
            SSPCON1bits.WCOL = 0;
            /* Clear the Receive Overflow Indicator */
            SSPCON1bits.SSPOV = 0;
            /* Release The Clock */
            SSPCON1bits.CKP = 1;
            /* Assign The I2C Slave Address */
            SSPADD = i2c_obj->i2c_cfg.i2c_slave_address;
        }else{/* Nothing */}
        /* Mode Select */
        SSPCON1bits.SSPM = i2c_obj->i2c_cfg.i2c_mode_cfg;
        /* I2C  GPIO pin Configurations Make two pins ADA and SCL inputs 
         * MSSp module will convert it to output if needed */
            TRISCbits.TRISC3 = 1;
            TRISCbits.TRISC4 = 1;  
        /* MSSP I2C Slew Rate Enable/Disable */
        if(I2C_SLEW_RATE_ENABLE == i2c_obj->i2c_cfg.i2c_slew_rate){
                I2C_SLEW_RATE_ENABLE_CFG();
            }else if(I2C_SLEW_RATE_DISABLE == i2c_obj->i2c_cfg.i2c_slew_rate){
                I2C_SLEW_RATE_DISABLE_CFG();
            }else{/* Nothing */}     
        /* MSSP I2C SMBus Enable/Disable */
        if(I2C_SMBus_ENABLE == i2c_obj->i2c_cfg.i2c_SMBus_control){
                I2C_SMBus_ENABLE_CFG();
            }else if(I2C_SMBus_DISABLE == i2c_obj->i2c_cfg.i2c_SMBus_control){
                I2C_SMBus_DISABLE_CFG();
            }else{/* Nothing */}        
        /* Interrupt Configuration */
        MSSP_I2C_Interrupt_Configuration(i2c_obj);
        /* Enable I2C */
        MSSP_MODULE_Enable_CFG();
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType MSSP_I2C_DeInit(const mssp_i2c_t *i2c_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }else{
        /* Disable I2C */
        MSSP_MODULE_Disable_CFG();
        /* Disable MSSP I2C Module */
        MSSP_MODULE_Enable_CFG();
        /* Interrupt Disable */
        #if MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE 
            MSSP_I2C_InterruptDisable();
            MSSP_I2C_BUS_COL_InterruptDisable();
        #if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
            if((INTERRUPT_HIGH_PRIORITY == i2c_obj->mssp_i2c_priority) || (INTERRUPT_HIGH_PRIORITY == i2c_obj->mssp_i2c_bc_priority)){
                INTERRUPT_GlobalInterruptHighDisable();
            }else if((INTERRUPT_LOW_PRIORITY == i2c_obj->mssp_i2c_priority) || (INTERRUPT_LOW_PRIORITY == i2c_obj->mssp_i2c_bc_priority)){
                INTERRUPT_GlobalInterruptLowDisable();
            }else {}
        #else
           INTERRUPT_GlobalInterruptDisable();
           INTERRUPT_PeripheralInterruptDisable(); 
        #endif
        #endif
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Send_Start(const mssp_i2c_t *i2c_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }else{
        /* Initiate Start Condition on SDA and SCL pins */
        SSPCON2bits.SEN = 1; /* Initiates Start condition on SDA and SCL pins. Automatically cleared by hardware */
        /* Wait for completion of start condition */
        while(SSPCON2bits.SEN);
        /* Clear the MSSP interrupt flag -> SSPIF */
        PIR1bits.SSPIF = 0;
        /* Report The Start Condition Detection */
        if(START_BIT_DETECTED == SSPSTATbits.S){
            ret = E_OK;
        }else{
            ret = E_NOT_OK;
        }  
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Send_Repeated_Start(const mssp_i2c_t *i2c_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }else{
        /* Initiate Repeated Start Condition on SDA and SCL pins */
        SSPCON2bits.RSEN = 1; /* Initiates Repeated Start condition on SDA and SCL pins. Automatically cleared by hardware */
        /* Wait for completion of Repeated start condition */
        while(SSPCON2bits.RSEN);
        /* Clear the MSSP interrupt flag -> SSPIF */
        PIR1bits.SSPIF = 0;
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Send_Stop(const mssp_i2c_t *i2c_obj){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == i2c_obj){
        ret = E_NOT_OK;
    }else{
        /* Initiate Stop Condition on SDA and SCL pins */
        SSPCON2bits.PEN = 1; /*  Initiates Stop condition on SDA and SCL pins. Automatically cleared by hardware */
        /* Wait for completion of Stop condition */
        while(SSPCON2bits.PEN);
        /* Clear the MSSP interrupt flag -> SSPIF */
        PIR1bits.SSPIF = 0;
        /* Report The Start Condition Detection */
        if(STOP_BIT_DETECTED == SSPSTATbits.P){
            ret = E_OK;
        }else{
            ret = E_NOT_OK;
        }  
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Write_Blocking(const mssp_i2c_t *i2c_obj, uint8 i2c_data, uint8 *ack){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == i2c_obj) || (NULL == ack)){
        ret = E_NOT_OK;
    }else{
        /* Write Data to the Data Register */
        SSPBUF = i2c_data;
        /* Wait The Transmission to be Completed */
        while(!(PIR1bits.SSPIF));
        /* Clear The MSSP Interrupt Flag bit -> SSPIF*/
        PIR1bits.SSPIF = 0;
        /* Report The Acknowledge  Received From the slave*/
        *ack = SSPCON2bits.ACKSTAT;
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType MSSP_I2C_Master_Read_Blocking(const mssp_i2c_t *i2c_obj, uint8 ack,uint8 *i2c_data){
    Std_ReturnType ret = E_NOT_OK;
    if((NULL == i2c_obj) || (NULL == i2c_data)){
        ret = E_NOT_OK;
    }else{
        /* Master Mode Receive Enable */
        I2C_MASTER_MODE_RECEIVE_ENABLE_CFG();
        /* Wait For buffer full flag : A complete Byte Received */
        while(!(SSPSTATbits.BF));
        /* Copy A data Registers to buffer variable */
        *i2c_data = SSPBUF;
        /* Send ACK or Not ACK after read*/
        SSPCON2bits.ACKDT = ack;
        SSPCON2bits.ACKEN = 1;
        ret = E_OK;
    }
    return ret;
}

Std_ReturnType MSSP_I2C_SLAVE_Read_Write(const mssp_i2c_t *i2c_obj,uint8 *i2c_data){
    /* Holds Clock low (Clock Stretch ) */
    I2C_CLOCK_STRETCH_ENABLE();
    /* If Master wanna send data read or if master will read data then send data*/
    if((SSPSTATbits.R_nW == 0) && (SSPSTATbits.D_nA == 0)){
        /* Master Will Write and  last received byte was address */
            uint8 dummy_buffer = SSPBUF; /* Read The Last Byte to Clear the buffer */
            while(!(SSPSTATbits.BF));
            *i2c_data = SSPBUF; /* Read Data */        
    }else if(SSPSTATbits.R_nW == 1){
        /* Master Will Read */
        
    }else{/* Nothing*/}
    /* Release Clock */
    I2C_CLOCK_STRETCH_DISABLE();
}

/* Interrupt Handler  */

void MSSP_I2C_ISR(void){
    #if MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    MSSP_I2C_InterruptFlagClear();
    if(I2CInterruptHandler){
        I2CInterruptHandler();
    }else {/* Nothing */}
    #endif
}
void MSSP_I2C_BUS_COL_ISR(void){
    #if MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    MSSP_I2C_BUS_COL_InterruptFlagClear();
    if(I2C_BUS_COLInterruptHandler){
        I2C_BUS_COLInterruptHandler();
    }else {/* Nothing */}
    #endif
}
/* Helper Function */
static inline void MSSP_I2C_Interrupt_Configuration(const mssp_i2c_t *i2c_obj){
        #if MSSP_I2C_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE  
        I2CInterruptHandler = i2c_obj->I2C_DefaultInterruptHandler;
        I2C_BUS_COLInterruptHandler = i2c_obj->I2C_Report_Write_Collision;
        I2C_Report_Receive_OverFlowInterruptHandler = i2c_obj->I2C_Report_Receive_Overflow;
        MSSP_I2C_InterruptEnable();
        MSSP_I2C_BUS_COL_InterruptEnable();
        MSSP_I2C_InterruptFlagClear();
        MSSP_I2C_BUS_COL_InterruptFlagClear();
        #if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
        INTERRUPT_PriorityLevelsEnable();
        if(INTERRUPT_HIGH_PRIORITY == i2c_obj->mssp_i2c_priority){
           INTERRUPT_GlobalInterruptHighEnable();
           MSSP_I2C_InterruptHighPriority();
       }else if(INTERRUPT_LOW_PRIORITY == i2c_obj->mssp_i2c_priority){
           INTERRUPT_GlobalInterruptLowEnable();
           MSSP_I2C_InterruptLowPriority();
       }else{/* Nothing */}
        if(INTERRUPT_HIGH_PRIORITY == i2c_obj->mssp_i2c_bc_priority){
           INTERRUPT_GlobalInterruptHighEnable();
           MSSP_I2C_BUS_COL_InterruptHighPriority();
       }else if(INTERRUPT_LOW_PRIORITY == i2c_obj->mssp_i2c_bc_priority){
           INTERRUPT_GlobalInterruptLowEnable();
           MSSP_I2C_BUS_COL_InterruptLowPriority();
       }else{/* Nothing */}
        #else
        INTERRUPT_GlobalInterruptEnable();
        INTERRUPT_PeripheralInterruptEnable();
        #endif
#endif          
}