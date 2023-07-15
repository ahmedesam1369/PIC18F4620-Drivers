#Software Interface Drivers for PIC18F4620
##Based on layered architecture pattern
##There Are two main layers
##( 1 ) MCAL Layer
Stands for Microcontroller Abstraction Layer
Which interacts with the internal registers of PIC MCU directly
> GPIO
> ADC
> CCP
> EEPROM
> Interrupt
> TIMER0
> TIMER1
> TIMER2
> TIMER3
> USART
> SPI
> I2C
##( 2 ) ECU Layer
Stands for Electronic Control Unit Abstraction Layer
Which is interact with MCAL Layer and Application Layer to perform the functionality
> Button
> LED
> Relay
> Seven Segment
> KeyPad
> LCD
> DC Motor

Made By Eng Ahmed Esam
