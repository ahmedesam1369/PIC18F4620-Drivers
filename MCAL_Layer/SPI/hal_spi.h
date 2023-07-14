/* 
 * File:   hal_spi.h
 * Author: Administrator
 *
 * Created on July 3, 2023, 7:15 PM
 */

#ifndef HAL_SPI_H
#define	HAL_SPI_H
/*------------------------------ Includes ------------------------------*/
#include "hal_spi_cfg.h"
#include "../Interrupt/mcal_internal_interrupt.h"
/*------------------------- Macro Declaration --------------------------*/
#define SPI_ENABLE                1
#define SPI_DISABLE               0

#define  BUFFER_STATUS_RECEIVE_COMPLETE         1
#define  BUFFER_STATUS_RECEIVE_NOT_COMPLETE     0

#define CKE_CLOCK_SELECT_ACTIVE_TO_IDLE           1
#define CKE_CLOCK_SELECT_IDLE_TO_ACTIVE           0

#define SMP_SAMPLE_SELECT_MASTER_END              1
#define SMP_SAMPLE_SELECT_MASTER_MIDDLE           0
#define SMP_SAMPLE_SELECT_SLAVE                   0

#define CKP_CLOCK_POLARITY_SELECT_HIGH            1
#define CKP_CLOCK_POLARITY_SELECT_LOW             0


/*--------------------- Macro Functions Declaration --------------------*/
/* SPI Enable/ Disable */
#define SPI_ENABLE_CFG()    (SSPCON1bits.SSPEN = 1)
#define SPI_DISABLE_CFG()   (SSPCON1bits.SSPEN = 0)
/* SPI OverFlow Error */
#define SPI_OVERFLOW_ERROR_STATUS  (SSPCON1bits.SSPOV)
#define SPI_OVERFLOW_ERROR_CLEAR_BIT()  (SSPCON1bits.SSPOV = 0)
/* SPI OverFlow COLLISION */
#define SPI_COLLISION_ERROR_STATUS  (SSPCON1bits.WCOL)
#define SPI_COLLISION_ERROR_CLEAR_BIT()  (SSPCON1bits.WCOL = 0)
/*------------------------ Data Type Declarations ----------------------*/
typedef enum{
    SPI_MASTER_MODE_FOSC_DIV_4 = 0,
    SPI_MASTER_MODE_FOSC_DIV_16 ,        
    SPI_MASTER_MODE_FOSC_DIV_64,
    SPI_MASTER_MODE_FOSC_TMR2_OUTPUT_DIV_4,   
    SPI_SLAVE_MODE_SS_ENABLED,
    SPI_SLAVE_MODE_SS_DISABLED,        
}mode_select_t;

typedef struct{
    mode_select_t mode;
    uint8 clock_active_idle;
    uint8 sample;
    uint8 clock_polarity;
}spi_t;
/*------------------- Software Interfaces  Declarations -----------------*/
Std_ReturnType SPI_Init(const spi_t *_spi);

Std_ReturnType SPI_DeInit(const spi_t *_spi);

Std_ReturnType SPI_ReadByteBlocking(uint8 *_data);
Std_ReturnType SPI_ReadByteNotBlocking(uint8 *_data);

Std_ReturnType SPI_WriteByteBlocking(uint8 _data);
Std_ReturnType SPI_WriteByteNotBlocking(uint8 _data);

Std_ReturnType SPI_WriteStringeBlocking(uint8 *_data, uint16 str_len);
Std_ReturnType SPI_WriteStringeNotBlocking(uint8 *_data, uint16 str_len);

#endif	/* HAL_SPI_H */

