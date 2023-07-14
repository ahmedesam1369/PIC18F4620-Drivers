/* 
 * File:   hal_usart.h
 * Author: Administrator
 *
 * Created on June 30, 2023, 6:33 PM
 */
#ifndef HAL_USART_H
#define	HAL_USART_H
/*------------------------------ Includes ------------------------------*/
#include "hal_usart_cfg.h"
#include "../Interrupt/mcal_internal_interrupt.h"
/*------------------------- Macro Declaration --------------------------*/
/* Selecting EUART Working Mode */
#define EUSART_SYNCHRONOUS_MODE    1
#define EUSART_ASYNCHRONOUS_MODE   0
/* Baud Rate Generator Asynchronous Speed Mode*/
#define EUSART_ASYNCHRONOUS_HIGH_SPEED 1
#define EUSART_ASYNCHRONOUS_LOW_SPEED  0
/* Baud Rate Generator Asynchronous Register Size */
#define EUSART_16BIT_BAUDRATE_GEN   1
#define EUSART_08BIT_BAUDRATE_GEN   0
/* EUSART Transmit Enable */
#define EUSART_ASYNCHRONOUS_TX_ENABLE   1
#define EUSART_ASYNCHRONOUS_TX_DISABLE   0
/* EUSART Transmit Interrupt Transmit Enable*/
#define EUSART_ASYNCHRONOUS_TX_INTERRUPT_ENABLE   1
#define EUSART_ASYNCHRONOUS_TX_INTERRUPT_DISABLE   0
/* EUSART Transmit 9-bit Transmit Enable */
#define EUSART_ASYNCHRONOUS_TX_9BIT_ENABLE   1
#define EUSART_ASYNCHRONOUS_TX_9BIT_DISABLE   0
/* EUSART RECEIVER Enable */
#define EUSART_ASYNCHRONOUS_RX_ENABLE   1
#define EUSART_ASYNCHRONOUS_RX_DISABLE   0
/* EUSART RECEIVER Interrupt Transmit Enable*/
#define EUSART_ASYNCHRONOUS_RX_INTERRUPT_ENABLE   1
#define EUSART_ASYNCHRONOUS_RX_INTERRUPT_DISABLE   0
/* EUSART RECEIVER 9-bit Transmit Enable */
#define EUSART_ASYNCHRONOUS_RX_9BIT_ENABLE   1
#define EUSART_ASYNCHRONOUS_RX_9BIT_DISABLE   0
/* EUSART Framing Error */
#define EUSART_FRAMING_ERROR_DETECTED  1
#define EUSART_FRAMING_ERROR_CLEARED   0
/* EUSART Overrun Error */
#define EUSART_OVERRUN_ERROR_DETECTED  1
#define EUSART_OVERRUN_ERROR_CLEAREED  0
/*--------------------- Macro Functions Declaration --------------------*/
#define EUSART_ENABLE()       (RCSTAbits.SPEN = 1)
#define EUSART_DISABLE()      (RCSTAbits.SPEN = 1)

/*------------------------ Data Type Declarations ----------------------*/
typedef enum{
    BAUDRATE_ASYNC_8BIT_LOW_SPEED,
    BAUDRATE_ASYNC_8BIT_HIGH_SPEED,
    BAUDRATE_ASYNC_16BIT_LOW_SPEED,
    BAUDRATE_ASYNC_16BIT_HIGH_SPEED,
    BAUDRATE_SYNC_8BIT,
    BAUDRATE_SYNC_16BIT
}baudrate_gen_t;

typedef struct{
    uint8 usart_tx_enable: 1;
    uint8 usart_tx_interrupt_enable : 1;
    uint8 usart_tx_9bit_enable : 1;
    uint8 usart_tx_reserved: 5;
    interrupt_priority_cfg usart_tx_int_priority;
}usart_tx_cfg_t;
typedef struct{
    uint8 usart_rx_enable: 1;
    uint8 usart_rx_interrupt_enable : 1;
    uint8 usart_rx_9bit_enable : 1;
    uint8 usart_tx_reserved: 5;
    interrupt_priority_cfg usart_rx_int_priority;
}usart_rx_cfg_t;

typedef union{
    struct{
    uint8 usart_ferr: 1;
    uint8 usart_oerr: 1;
    uint8 usart_reserved: 1;
    };
    uint8 status; 
}usart_error_status_t;

typedef struct{
    uint32 baudrate;
    uint8 baudrate_mode;
    baudrate_gen_t baudrate_gen_config;
    usart_tx_cfg_t usart_tx_cfg;
    usart_rx_cfg_t usart_rx_cfg;
    usart_error_status_t error_status;
    void (*EUSART_TXDefaultInterruptHandler)(void);
    void (*EUSART_RXDefaultInterruptHandler)(void);
    void (*EUSART_FramingErrorHandler)(void);
    void (*EUSART_OverrunErrorHandler)(void);
}usart_t;

/*------------------- Software Interfaces  Declarations -----------------*/
Std_ReturnType EUSART_ASYNC_Init(const usart_t *_eusart);
Std_ReturnType EUSART_ASYNC_DeInit(const usart_t *_eusart);

Std_ReturnType EUSART_ASYNC_ReadByteBlocking(uint8 *_data);
Std_ReturnType EUSART_ASYNC_ReadByteNonBlocking(uint8 *_data);
Std_ReturnType EUSART_ASYNC_RX_Restart(void);

Std_ReturnType EUSART_ASYNC_WriteByteBlocking(uint8 _data);
Std_ReturnType EUSART_ASYNC_WriteStringeBlocking(uint8 *_data, uint16 str_len);


// Not Working Without Delay
Std_ReturnType EUSART_ASYNC_WriteByteNonBlocking(uint8 _data);
Std_ReturnType EUSART_ASYNC_WriteStringeNonBlocking(uint8 *_data, uint16 str_len);

void EUSART_TX_ISR(void);
void EUSART_RX_ISR(void);
void EUSART_FRAMIN_ERROR_ISR(void);
void EUSART_OVERRUN_ERROR_ISR(void);
#endif	/* HAL_USART_H */

