/* 
 * File:   hal_usart.c
 * Author: Administrator
 *
 * Created on June 30, 2023, 6:34 PM
 */
#include "hal_usart.h"

static void EUSART_BAUD_RATE_Calculation(const usart_t *_eusart);
static void EUSART_ASYNC_TX_Init(const usart_t *_eusart);
static void EUSART_ASYNC_RX_Init(const usart_t *_eusart);
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void(*EUSART_TxInterruptHandler)(void) = NULL;
#endif
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    static void(*EUSART_RxInterruptHandler)(void) = NULL;
    static void(*EUSART_FramingErrorHandler)(void) = NULL;
    static void(*EUSART_OverrunErrorHandler)(void) = NULL;
#endif

Std_ReturnType EUSART_ASYNC_Init(const usart_t *_eusart){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _eusart){
        ret = E_NOT_OK;
    }else{
        EUSART_DISABLE();
        /*  Configure TX, and RX Bits as Input*/
        TRISCbits.RC6 = GPIO_DIRECTION_INPUT;
        TRISCbits.RC7 = GPIO_DIRECTION_INPUT;
        EUSART_BAUD_RATE_Calculation(_eusart);
        if(EUSART_ASYNCHRONOUS_TX_ENABLE == _eusart->usart_tx_cfg.usart_tx_enable){
            EUSART_ASYNC_TX_Init(_eusart);
        }else{/* Nothing  */}
        if(EUSART_ASYNCHRONOUS_RX_ENABLE == _eusart->usart_rx_cfg.usart_rx_enable){
            EUSART_ASYNC_RX_Init(_eusart);
        }else{/* Nothing  */}
        EUSART_ENABLE();
        ret =E_OK;
    }
    return ret;
}

Std_ReturnType EUSART_ASYNC_DeInit(const usart_t *_eusart){
    Std_ReturnType ret = E_NOT_OK;
    if(NULL == _eusart){
        ret = E_NOT_OK;
    }else{
        EUSART_DISABLE();
        ret =E_OK;
    }
    return ret;
}

Std_ReturnType EUSART_ASYNC_ReadByteBlocking(uint8 *_data){
    Std_ReturnType ret = E_NOT_OK;
    while(!(PIR1bits.RCIF));
    *_data = RCREG;
    ret =E_OK;
    return ret;
}

Std_ReturnType EUSART_ASYNC_ReadByteNonBlocking(uint8 *_data){
    Std_ReturnType ret = E_NOT_OK;
    if(PIR1bits.RCIF){
        *_data = RCREG;
        ret =E_OK;
    }else{
        ret = E_NOT_OK;
    }
    return ret;
}

Std_ReturnType EUSART_ASYNC_RX_Restart(void){
    Std_ReturnType ret = E_NOT_OK;
    RCSTAbits.CREN = 0; /* Disables receiver */
    RCSTAbits.CREN = 1; /*Enables receiver */
    ret =E_OK;
    return ret;
}

Std_ReturnType EUSART_ASYNC_WriteByteBlocking(uint8 _data){
    Std_ReturnType ret = E_NOT_OK;
    /* Wait (Block Operation untilShift Register become Empty )*/
    while(!(TXSTAbits.TRMT));
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    EUSART_TXInterruptEnable();
#endif
    TXREG = _data;
    ret =E_OK;
    return ret;
}
Std_ReturnType EUSART_ASYNC_WriteStringeBlocking(uint8 *_data, uint16 str_len){
    Std_ReturnType ret = E_NOT_OK;
    uint16 counter = ZERO_INIT;
    for(counter=ZERO_INIT; counter<str_len; counter++){
        ret = EUSART_ASYNC_WriteByteBlocking(_data[counter]);
    }
    ret =E_OK;
    return ret;
}

Std_ReturnType EUSART_ASYNC_WriteByteNonBlocking(uint8 _data){
    Std_ReturnType ret = E_NOT_OK;
    /* Delay (Wait Operation untilShift Register become Empty )*/
    __delay_ms(1);
    if(TXSTAbits.TRMT){
#if EUSART_TX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
    EUSART_TXInterruptEnable();
#endif
    TXREG = _data;
    ret =E_OK;
    }else{
        TXREG = _data;
        ret = E_NOT_OK;
    }
    return ret;

}

Std_ReturnType EUSART_ASYNC_WriteStringeNonBlocking(uint8 *_data, uint16 str_len){
    Std_ReturnType ret = E_NOT_OK;
    uint16 counter = ZERO_INIT;
    for(counter=ZERO_INIT; counter<str_len; counter++){
        ret = EUSART_ASYNC_WriteByteNonBlocking(_data[counter]);
    }
    ret =E_OK;
    return ret;
}


/*----------------------- Helper Functions -----------------------------*/
static void EUSART_BAUD_RATE_Calculation(const usart_t *_eusart){
    float BAUD_RATE_TEMP = 0;
    switch(_eusart->baudrate_gen_config){
        case BAUDRATE_ASYNC_8BIT_LOW_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_08BIT_BAUDRATE_GEN;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_LOW_SPEED;
            BAUD_RATE_TEMP = ((_XTAL_FREQ / (float)_eusart->baudrate) /64) - 1 ;
            break;
        case BAUDRATE_ASYNC_8BIT_HIGH_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_08BIT_BAUDRATE_GEN;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_HIGH_SPEED;
            BAUD_RATE_TEMP = ((_XTAL_FREQ / (float)_eusart->baudrate) /16) - 1 ;
            break;
        case BAUDRATE_ASYNC_16BIT_LOW_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_16BIT_BAUDRATE_GEN;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_LOW_SPEED;
            BAUD_RATE_TEMP = ((_XTAL_FREQ / (float)_eusart->baudrate) /16) - 1 ;
            break;
        case BAUDRATE_ASYNC_16BIT_HIGH_SPEED:
            TXSTAbits.SYNC = EUSART_ASYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_16BIT_BAUDRATE_GEN;
            TXSTAbits.BRGH = EUSART_ASYNCHRONOUS_HIGH_SPEED;
            BAUD_RATE_TEMP = ((_XTAL_FREQ / (float)_eusart->baudrate) /4) - 1 ;
            break;
        case BAUDRATE_SYNC_8BIT:
            TXSTAbits.SYNC = EUSART_SYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_08BIT_BAUDRATE_GEN;
            BAUD_RATE_TEMP = ((_XTAL_FREQ / (float)_eusart->baudrate) /4) - 1 ;
            break;
        case BAUDRATE_SYNC_16BIT:
            TXSTAbits.SYNC = EUSART_SYNCHRONOUS_MODE;
            BAUDCONbits.BRG16 = EUSART_16BIT_BAUDRATE_GEN;
            BAUD_RATE_TEMP = ((_XTAL_FREQ / (float)_eusart->baudrate) /4) - 1 ;
            break;
        default: ;  
    }
    
    SPBRG  = (uint8)((uint32)BAUD_RATE_TEMP);
    SPBRGH = (uint8)(((uint32)BAUD_RATE_TEMP) >> 8);
}

static void EUSART_ASYNC_TX_Init(const usart_t *_eusart){
    /* Enable the transmission Configuration*/
    TXSTAbits.TXEN = _eusart->usart_tx_cfg.usart_tx_enable;
    EUSART_TxInterruptHandler = _eusart->EUSART_TXDefaultInterruptHandler;
    /* UART Interrupt Transmit Configuration */
    if(EUSART_ASYNCHRONOUS_TX_INTERRUPT_ENABLE == _eusart->usart_tx_cfg.usart_tx_interrupt_enable){
       PIE1bits.TX1IE = _eusart->usart_tx_cfg.usart_tx_interrupt_enable;
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
  EUSART_TXInterruptEnable();
  #if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == _eusart->usart_tx_cfg.usart_tx_int_priority){
       INTERRUPT_GlobalInterruptHighEnable();
       EUSART_TX_InterruptHighPriority();
    }else if(INTERRUPT_LOW_PRIORITY == _eusart->usart_tx_cfg.usart_tx_int_priority){
     INTERRUPT_GlobalInterruptLowEnable();
     EUSART_TX_InterruptLowPriority();
    }else{ /* Nothing  */}
  #else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
  #endif    
#endif        
    }else {/* Nothing */}
    
    /*  9-bit transmission Configuration*/
    TXSTAbits.TX9 = _eusart->usart_tx_cfg.usart_tx_9bit_enable;
}

static void EUSART_ASYNC_RX_Init(const usart_t *_eusart){
    /* Enable the Receiver Configuration*/
    RCSTAbits.CREN = _eusart->usart_rx_cfg.usart_rx_enable;
    EUSART_RxInterruptHandler = _eusart->EUSART_RXDefaultInterruptHandler;
    EUSART_FramingErrorHandler = _eusart->EUSART_FramingErrorHandler;
    EUSART_OverrunErrorHandler = _eusart->EUSART_OverrunErrorHandler;
    /* UART Interrupt Receiver Configuration */
    if(EUSART_ASYNCHRONOUS_RX_INTERRUPT_ENABLE == _eusart->usart_rx_cfg.usart_rx_interrupt_enable){
       PIE1bits.RCIE = _eusart->usart_rx_cfg.usart_rx_interrupt_enable; 
#if EUSART_RX_INTERRUPT_FEATURE_ENABLE == INTERRUPT_FEATURE_ENABLE
  EUSART_RX_InterruptEnable();
  #if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE
    INTERRUPT_PriorityLevelsEnable();
    if(INTERRUPT_HIGH_PRIORITY == _eusart->usart_rx_cfg.usart_rx_int_priority){
       INTERRUPT_GlobalInterruptHighEnable();
       EUSART_RX_InterruptHighPriority();
    }else if(INTERRUPT_LOW_PRIORITY == _eusart->usart_rx_cfg.usart_rx_int_priority){
     INTERRUPT_GlobalInterruptLowEnable();
     EUSART_RX_InterruptLowPriority();
    }else{ /* Nothing  */}
    
  #else
    INTERRUPT_GlobalInterruptEnable();
    INTERRUPT_PeripheralInterruptEnable();
  #endif   
  #endif 
    }else {/* Nothing */}
    
    /*  9-bit Receiver Configuration*/
    RCSTAbits.RX9 = _eusart->usart_rx_cfg.usart_rx_9bit_enable;
    
}

void EUSART_TX_ISR(void){
    EUSART_TXInterruptDisable();
    if(EUSART_TxInterruptHandler){
        EUSART_TxInterruptHandler();
    }else {/* Nothing */}
}
void EUSART_RX_ISR(void){
    if(EUSART_RxInterruptHandler){
        EUSART_RxInterruptHandler();
    }else {/* Nothing */}
    
}

void EUSART_FRAMIN_ERROR_ISR(void){
    if(EUSART_FramingErrorHandler){
        EUSART_FramingErrorHandler();
    }else {/* Nothing */}
}
void EUSART_OVERRUN_ERROR_ISR(void){
    if(EUSART_OverrunErrorHandler){
        EUSART_OverrunErrorHandler();
    }else {/* Nothing */}
    
}


    