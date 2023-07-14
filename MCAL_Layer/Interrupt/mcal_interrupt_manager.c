/* 
 * File:   mcal_interrupt_manager.c
 * Author: Eng A_Esam
 *
 * Created on May 29, 2023, 9:49 PM
 */

#include "mcal_interrupt_manager.h"

static volatile uint8 RB4_Flag = 1, RB5_Flag = 1, RB6_Flag = 1, RB7_Flag = 1;

#if INTERUPT_PRIORITY_LEVELS_ENABLE == INTERRUPT_FEATURE_ENABLE

void __interrupt() InterruptManagerHigh(void){
    
    
    if((INTERRUPT_ENABLE == INTCON3bits.INT2IE) && (INTERRUPT_OCCUR == INTCON3bits.INT2IF)){
        INT2_ISR();
    }else{
        /* Nothing */
    }
    if((INTERRUPT_ENABLE == INTCONbits.INT0IE) && (INTERRUPT_OCCUR == INTCONbits.INT0IF)){
        INT0_ISR();
    }else{
        /* Nothing */
    }
    /* On Change Interrupt RBx*/
    // RB4
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF)
            && (GPIO_HIGH == PORTBbits.RB4)  && (RB4_Flag == 1 )){
        RB4_Flag = 0;
        RB4_ISR(1);
    }else{
        /* Nothing */
    }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF)
            && (GPIO_LOW == PORTBbits.RB4)&& (RB4_Flag == 0 )){
        RB4_Flag = 1;
        RB4_ISR(0);
    }else{
        /* Nothing */
    }
//    RB5
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) 
            && (GPIO_HIGH == PORTBbits.RB5)&& (RB5_Flag == 1 )){
        RB5_Flag = 0;
        RB5_ISR(1);
    }else{
        /* Nothing */
    }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) 
            && (GPIO_LOW == PORTBbits.RB5)&& (RB5_Flag == 0 )){
        RB5_Flag = 1;
        RB5_ISR(0);
    }else{
        /* Nothing */
    }
    //    RB6
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) 
            && (GPIO_HIGH == PORTBbits.RB6)&& (RB6_Flag == 1 )){
        RB6_Flag = 0;
        RB6_ISR(1);
    }else{
        /* Nothing */
    }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) 
            && (GPIO_LOW == PORTBbits.RB5)&& (RB6_Flag == 0 )){
        RB6_Flag = 1;
        RB6_ISR(0);
    }else{
        /* Nothing */
    }
//    RB7
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) 
            && (GPIO_HIGH == PORTBbits.RB7)&& (RB7_Flag == 1 )){
        RB7_Flag = 0;
        RB7_ISR(1);
    }else{
        /* Nothing */
    }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) 
            && (GPIO_LOW == PORTBbits.RB7)&& (RB7_Flag == 0 )){
        RB7_Flag = 1;
        RB7_ISR(0);
    }else{
        /* Nothing */
    }
}

void __interrupt(low_priority) InterruptManagerLow(void){
    if((INTERRUPT_ENABLE == INTCON3bits.INT1IE) && (INTERRUPT_OCCUR == INTCON3bits.INT1IF)){
        INT1_ISR();
    }else{
        /* Nothing */
    }
    
}

#else

void __interrupt() InterruptManager(void){
    if((INTERRUPT_ENABLE == INTCONbits.INT0IE) && (INTERRUPT_OCCUR == INTCONbits.INT0IF)){
        INT0_ISR();
    }else{
        /* Nothing */
    }
    if((INTERRUPT_ENABLE == INTCON3bits.INT1IE) && (INTERRUPT_OCCUR == INTCON3bits.INT1IF)){
        INT1_ISR();
    }else{
        /* Nothing */
    }
    if((INTERRUPT_ENABLE == INTCON3bits.INT2IE) && (INTERRUPT_OCCUR == INTCON3bits.INT2IF)){
        INT2_ISR();
    }else{
        /* Nothing */
    }
    
    /* On Change Interrupt RBx*/
    // RB4
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF)
            && (GPIO_HIGH == PORTBbits.RB4)  && (RB4_Flag == 1 )){
        RB4_Flag = 0;
        RB4_ISR(1);
    }else{
        /* Nothing */
    }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF)
            && (GPIO_LOW == PORTBbits.RB4)&& (RB4_Flag == 0 )){
        RB4_Flag = 1;
        RB4_ISR(0);
    }else{
        /* Nothing */
    }
//    RB5
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) 
            && (GPIO_HIGH == PORTBbits.RB5)&& (RB5_Flag == 1 )){
        RB5_Flag = 0;
        RB5_ISR(1);
    }else{
        /* Nothing */
    }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) 
            && (GPIO_LOW == PORTBbits.RB5)&& (RB5_Flag == 0 )){
        RB5_Flag = 1;
        RB5_ISR(0);
    }else{
        /* Nothing */
    }
//    RB6
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) 
            && (GPIO_HIGH == PORTBbits.RB6)&& (RB6_Flag == 1 )){
        RB6_Flag = 0;
        RB6_ISR(1);
    }else{
        /* Nothing */
    }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) 
            && (GPIO_LOW == PORTBbits.RB5)&& (RB6_Flag == 0 )){
        RB6_Flag = 1;
        RB6_ISR(0);
    }else{
        /* Nothing */
    }
//    RB7
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) 
            && (GPIO_HIGH == PORTBbits.RB7)&& (RB7_Flag == 1 )){
        RB7_Flag = 0;
        RB7_ISR(1);
    }else{
        /* Nothing */
    }
    if((INTERRUPT_ENABLE == INTCONbits.RBIE) && (INTERRUPT_OCCUR == INTCONbits.RBIF) 
            && (GPIO_LOW == PORTBbits.RB7)&& (RB7_Flag == 0 )){
        RB7_Flag = 1;
        RB7_ISR(0);
    }else{
        /* Nothing */
    }
    
    
    /*   Internal Interrupt */
//    ADC Interrupt 
    if((INTERRUPT_ENABLE == PIE1bits.ADIE) && (INTERRUPT_OCCUR == PIR1bits.ADIF)){
        ADC_ISR();
    }else{
        /* Nothing */
    }
    
//    TIMER0 Interrupt 
    if((INTERRUPT_ENABLE == INTCONbits.TMR0IE) && (INTERRUPT_OCCUR == INTCONbits.TMR0IF)){
        TIMER0_ISR();
    }else{
        /* Nothing */
    }
    
//    TIMER1 Interrupt 
    if((INTERRUPT_ENABLE == PIE1bits.TMR1IE) && (INTERRUPT_OCCUR == PIR1bits.TMR1IF)){
        TIMER1_ISR();
    }else{
        /* Nothing */
    }
   
//    TIMER2 Interrupt 
    if((INTERRUPT_ENABLE == PIE1bits.TMR2IE) && (INTERRUPT_OCCUR == PIR1bits.TMR2IF)){
        TIMER2_ISR();
    }else{
        /* Nothing */
    }
//    TIMER3 Interrupt 
    if((INTERRUPT_ENABLE == PIE2bits.TMR3IE) && (INTERRUPT_OCCUR == PIR2bits.TMR3IF)){
        TIMER3_ISR();
    }else{
        /* Nothing */
    }

//    CCP1 Interrupt 
    if((INTERRUPT_ENABLE == PIE1bits.CCP1IE) && (INTERRUPT_OCCUR == PIR1bits.CCP1IF)){
        CCP1_ISR();
    }else{
        /* Nothing */
    }
//    CCP2 Interrupt 
    if((INTERRUPT_ENABLE == PIE2bits.CCP2IE) && (INTERRUPT_OCCUR == PIR2bits.CCP2IF)){
        CCP2_ISR();
    }else{
        /* Nothing */
    } 
    
//    TX EUSART Interrupt 
    if((INTERRUPT_ENABLE == PIE1bits.TXIE) && (INTERRUPT_OCCUR == PIR1bits.TXIF)){
        EUSART_TX_ISR();
    }else{
        /* Nothing */
    }
//    RX EUSART Interrupt 
    if((INTERRUPT_ENABLE == PIE1bits.RCIE) && (INTERRUPT_OCCUR == PIR1bits.RCIF)){
        EUSART_RX_ISR();
    }else{
        /* Nothing */
    } 
//    Overrun Error EUSART Interrupt 
    if((INTERRUPT_ENABLE == PIE1bits.RCIE) && (INTERRUPT_OCCUR == RCSTAbits.OERR)){
        EUSART_OVERRUN_ERROR_ISR();
    }else{
        /* Nothing */
    } 
    
//    Framing EUSART Interrupt 
    if((INTERRUPT_ENABLE == PIE1bits.RCIE) && (INTERRUPT_OCCUR == RCSTAbits.FERR)){
        EUSART_FRAMIN_ERROR_ISR();
    }else{
        /* Nothing */
    }
    
//    MSSP I2C Interrupt 
    if((INTERRUPT_ENABLE == PIE1bits.SSPIE)){
        if((INTERRUPT_OCCUR == PIR1bits.SSPIF)){
            MSSP_I2C_ISR();
        }else{/* Nothing */ }
        if((INTERRUPT_OCCUR == PIR2bits.BCLIF)){
            MSSP_I2C_BUS_COL_ISR();
        }else{/* Nothing */ }   
    }else{
        /* Nothing */
    }
}

#endif
