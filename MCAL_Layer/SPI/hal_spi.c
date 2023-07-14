/* 
 * File:   hal_spi.c
 * Author: Administrator
 *
 * Created on July 3, 2023, 7:15 PM
 */
#include "hal_spi.h"
#if MSSP_SPI_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
static void (*SPI_ImterruptHandler)(void) = NULL;
#endif
static void Pin_Directions_Initialize(const spi_t *_spi);
    
Std_ReturnType SPI_Init(const spi_t *_spi){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _spi){
        ret = E_NOT_OK;
    }else{
        /* Disable SPI Module */
        SPI_DISABLE_CFG();
        /* Configure SPI GPIO Module */
        Pin_Directions_Initialize(_spi);
        /* Select Mode Master/Slave */
        SSPCON1bits.SSPM = _spi->mode;
        /* SPI Clock Polarity */
        SSPCON1bits.CKP = _spi->clock_polarity;
        /* SPI Clock Phase */
        SSPSTATbits.CKE = _spi->clock_active_idle;
        /* SPI Sampled Data */
        SSPSTATbits.SMP = _spi->sample;
        /* Enable SPI Module */
        SPI_ENABLE_CFG();
        ret = E_OK;
    }
}

Std_ReturnType SPI_DeInit(const spi_t *_spi){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _spi){
        ret = E_NOT_OK;
    }else{
        /* Disable SPI Module */
        SPI_DISABLE_CFG();
        ret = E_OK;
    }
}

Std_ReturnType SPI_ReadByteBlocking(uint8 *_data){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _data){
        ret = E_NOT_OK;
    }else{
        while(!(SSPSTATbits.BF));
        *_data = SSPBUF;
        ret = E_OK;
    }
}

Std_ReturnType SPI_ReadByteNotBlocking(uint8 *_data){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _data){
        ret = E_NOT_OK;
    }else{
        if(PIR1bits.SSPIF){
            *_data = SSPBUF;
            PIR1bits.SSPIF = 0;
            ret = E_OK;
        }else{
          ret = E_NOT_OK;  
        } 
    }
}


Std_ReturnType SPI_WriteByteBlocking(uint8 _data){
    Std_ReturnType ret = E_NOT_OK;
    uint8 buff_removed = 0;
    SSPBUF = _data;
    while(!(SSPSTATbits.BF));
    PIR1bits.SSPIF = 0;
    buff_removed =  SSPBUF;   
    ret = E_OK;
    return ret;
}

Std_ReturnType SPI_WriteByteNotBlocking(uint8 _data){
    Std_ReturnType ret = E_NOT_OK;
    if((1 == SSPSTATbits.BF) || (1 == PIR1bits.SSPIF)){
        PIR1bits.SSPIF = 0;
        SSPBUF = _data;
    }
    ret = E_OK;
    return ret;
}

Std_ReturnType SPI_WriteStringeBlocking(uint8 *_data, uint16 str_len){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _data){
        ret = E_NOT_OK;
    }else{
    uint16 counter = ZERO_INIT;
    for(counter=ZERO_INIT; counter<str_len; counter++){
        ret = SPI_WriteByteNotBlocking(_data[counter]);
    }
        ret = E_OK;
    }
}

// Helper Functions
static void Pin_Directions_Initialize(const spi_t *_spi){
    Std_ReturnType ret = E_NOT_OK;
    if((_spi->mode == 0) || (_spi->mode == 1) || (_spi->mode == 2) || (_spi->mode == 3)){
        TRISCbits.RC4 = 1; /* SDI input */
        TRISCbits.RC3 = 0; /* SCK output */
        TRISAbits.RA5 = 0; /* SS' output */
        TRISCbits.RC5 = 0; /* SDO output */
        }else if((_spi->mode == 4) || (_spi->mode == 5)){
        TRISCbits.RC4 = 1; /* SDI input */
        TRISCbits.RC3 = 1; /* SCK input */
        TRISAbits.RA5 = 1; /* SS' input */
        TRISCbits.RC5 = 0; /* SDO output */
        }else{
        ret = E_NOT_OK;
        }
    
   
}