/* 
 * File:   ecu_chr_lcd.c
 * Author: Eng A_Esam
 *
 * Created on May 15, 2023, 2:43 PM
 */
#include "ecu_chr_lcd.h"
static Std_ReturnType lcd_send_4bit(const chr_lcd_4bit_t *lcd, uint8 _data_command);
static Std_ReturnType lcd_4bit_send_enable_signal(const chr_lcd_4bit_t *lcd);
static Std_ReturnType lcd_8bit_send_enable_signal(const chr_lcd_8bit_t *lcd);
static Std_ReturnType  lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd, uint8 row, uint8 column);
static Std_ReturnType  lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd, uint8 row, uint8 column);


Std_ReturnType lcd_4bit_initialize(const chr_lcd_4bit_t *lcd){
    Std_ReturnType ret = E_OK;
    uint8 l_data_pins_counter = ZERO_INIT;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
        ret = gpio_pin_initialize(&(lcd->lcd_en));
        ret = gpio_pin_initialize(&(lcd->lcd_rs));
        for(l_data_pins_counter = 0; l_data_pins_counter<4;l_data_pins_counter++){
            ret = gpio_pin_initialize(&(lcd->lcd_data[l_data_pins_counter]));
        }
     __delay_ms(20);
     ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
     __delay_ms(5);
     ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
     __delay_us(150);
     ret = lcd_4bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);  
     ret = lcd_4bit_send_command(lcd, _LCD_CLEAR); 
     ret = lcd_4bit_send_command(lcd, _LCD_RETURN_HOME); 
     ret = lcd_4bit_send_command(lcd, _LCD_ENTRY_MODE); 
     ret = lcd_4bit_send_command(lcd, _LCD_CURSOR_OFF_DISPLAY_ON); 
     ret = lcd_4bit_send_command(lcd, _LCD_4BIT_MODE_2_LINE);  
     ret = lcd_4bit_send_command(lcd, _LCD_DDRAM_START);  
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_command(const chr_lcd_4bit_t *lcd, uint8 command){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_LOW);
//        Send the high order four bit first and then the low
        ret = lcd_send_4bit(lcd, command >> 4) ;   
        ret = lcd_4bit_send_enable_signal(lcd);
        ret = lcd_send_4bit(lcd, command ) ;  
        ret = lcd_4bit_send_enable_signal(lcd);
        
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_char_data(const chr_lcd_4bit_t *lcd, uint8 data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_HIGH);
        ret = lcd_send_4bit(lcd, data >> 4) ;   
        ret = lcd_4bit_send_enable_signal(lcd);
        ret = lcd_send_4bit(lcd, data ) ;  
        ret = lcd_4bit_send_enable_signal(lcd);
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_char_data_pos(const chr_lcd_4bit_t *lcd,uint8 row,uint8 column, uint8 data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
       ret = lcd_4bit_set_cursor(lcd, row,column);
       ret = lcd_4bit_send_char_data(lcd, data); 
       
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_string(const chr_lcd_4bit_t *lcd, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd || NULL == str){
        ret = E_NOT_OK;
    }else{
        while(*str){
            ret = lcd_4bit_send_char_data(lcd, *str++);
        }
       
       
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_string_pos(const chr_lcd_4bit_t *lcd, uint8 row, 
                                        uint8 column,uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd || NULL == str){
        ret = E_NOT_OK;
    }else{
        ret = lcd_4bit_set_cursor(lcd, row, column);
        while(*str){
            ret = lcd_4bit_send_char_data(lcd, *str++);
        }
       
    }
    return ret;
}

Std_ReturnType lcd_4bit_send_custom_char(const chr_lcd_4bit_t *lcd,
                                        uint8 row, uint8 column,
                                        const uint8 _chr[], uint8 mempos){
    Std_ReturnType ret = E_OK;
    uint8 lcd_counter = ZERO_INIT;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
      ret = lcd_4bit_send_command(lcd, _LCD_CGRAM_START + (8* mempos));
        for(lcd_counter=0; lcd_counter<=7; lcd_counter++){
            ret = lcd_4bit_send_char_data(lcd, _chr[lcd_counter]);
        }
        ret = lcd_4bit_send_char_data_pos(lcd, row, column, mempos);
    }
    return ret;
}



//--------------------------------------------------------------------------------------------
Std_ReturnType lcd_8bit_initialize(const chr_lcd_8bit_t *lcd){
    Std_ReturnType ret = E_OK;
    uint8 l_data_pins_counter = ZERO_INIT;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
        ret = gpio_pin_initialize(&(lcd->lcd_en));
        ret = gpio_pin_initialize(&(lcd->lcd_rs));
        for(l_data_pins_counter = 0; l_data_pins_counter<8;l_data_pins_counter++){
            ret = gpio_pin_initialize(&(lcd->lcd_data[l_data_pins_counter]));
        }
       
         __delay_ms(20);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        __delay_ms(5);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        __delay_us(150);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        ret = lcd_8bit_send_command(lcd, _LCD_CLEAR);
        ret = lcd_8bit_send_command(lcd, _LCD_RETURN_HOME);
        ret = lcd_8bit_send_command(lcd, _LCD_ENTRY_MODE);
        ret = lcd_8bit_send_command(lcd, _LCD_CURSOR_OFF_DISPLAY_ON);
        ret = lcd_8bit_send_command(lcd, _LCD_8BIT_MODE_2_LINE);
        
        ret = lcd_8bit_send_command(lcd, _LCD_DDRAM_START); 
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_command(const chr_lcd_8bit_t *lcd, uint8 command){
    Std_ReturnType ret = E_OK;
    uint8 l_pin_counter = ZERO_INIT;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_LOW);
        for(l_pin_counter =0; l_pin_counter< 8; l_pin_counter++){
            ret = gpio_pin_write_logic(&(lcd->lcd_data[l_pin_counter]), (command>>l_pin_counter) & (uint8)0x01);
        }
        ret = lcd_8bit_send_enable_signal(lcd);
        
       
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_char_data(const chr_lcd_8bit_t *lcd, uint8 data){
    Std_ReturnType ret = E_OK;
    uint8 l_pin_counter = ZERO_INIT;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
        ret = gpio_pin_write_logic(&(lcd->lcd_rs), GPIO_HIGH);
        for(l_pin_counter =0; l_pin_counter< 8; l_pin_counter++){
            ret = gpio_pin_write_logic(&(lcd->lcd_data[l_pin_counter]), (data>>l_pin_counter) & (uint8)0x01);
        }
        ret = lcd_8bit_send_enable_signal(lcd);
       
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_char_data_pos(const chr_lcd_8bit_t *lcd,
                                            uint8 row,uint8 column, uint8 data){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
        ret = lcd_8bit_set_cursor(lcd, row,column);
        ret = lcd_8bit_send_char_data(lcd, data);
       
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_string(const chr_lcd_8bit_t *lcd, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd || NULL == str){
        ret = E_NOT_OK;
    }else{
//        loop Un till reach to null termination
        while(*str){
            ret = lcd_8bit_send_char_data(lcd, *str++);
        }
       
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_string_pos(const chr_lcd_8bit_t *lcd, 
                                   uint8 row, uint8 column,uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd || NULL == str){
        ret = E_NOT_OK;
    }else{
        ret = lcd_8bit_set_cursor(lcd,row, column);
        ret = lcd_8bit_send_string(lcd , str);
    }
    return ret;
}

Std_ReturnType lcd_8bit_send_custom_char(const chr_lcd_8bit_t *lcd, 
                                        uint8 row, uint8 column, 
                                        const uint8 _chr[], uint8 mempos){
    Std_ReturnType ret = E_OK;
    uint8 lcd_counter = ZERO_INIT;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
        ret = lcd_8bit_send_command(lcd, _LCD_CGRAM_START + (8* mempos));
        for(lcd_counter=0; lcd_counter<=7; lcd_counter++){
            ret = lcd_8bit_send_char_data(lcd, _chr[lcd_counter]);
        }
        ret = lcd_8bit_send_char_data_pos(lcd, row, column, mempos);
    }
    return ret;
}

//------------------------------------------------------------------------------------------

Std_ReturnType convert_byte_to_string(uint8 value, uint8 *str){
    Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }else{
        memset(str, '\0', 4);
        sprintf(str, "%i", value);
    }
    return ret;
}

Std_ReturnType convert_short_to_string(uint16 value, uint8 *str){
    Std_ReturnType ret = E_OK;
     if(NULL == str){
        ret = E_NOT_OK;
    }else{
        memset(str, '\0', 6);
        sprintf(str, "%i", value);
    }
    return ret;
}

Std_ReturnType convert_int_to_string(uint32 value, uint8 *str){
   Std_ReturnType ret = E_OK;
    if(NULL == str){
        ret = E_NOT_OK;
    }else{
        memset(str, '\0', 11);
        sprintf(str, "%i", value);
    }
    return ret;  
}

//------------------------------------------------------------------------------

static Std_ReturnType lcd_send_4bit(const chr_lcd_4bit_t *lcd, uint8 _data_command){
    Std_ReturnType ret = E_OK;
    uint8 l_data_pins_counter = ZERO_INIT;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
        for(l_data_pins_counter=0; l_data_pins_counter< 4; l_data_pins_counter++){
            ret = gpio_pin_write_logic(&(lcd->lcd_data[l_data_pins_counter]), ((_data_command >> l_data_pins_counter) & (uint8)0x01));
        }           
    }
    return ret;
}

static Std_ReturnType lcd_4bit_send_enable_signal(const chr_lcd_4bit_t *lcd){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
          ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_HIGH);
          __delay_us(5);
          ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_LOW);
    }
    return ret;
}

static Std_ReturnType lcd_8bit_send_enable_signal(const chr_lcd_8bit_t *lcd){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
          ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_HIGH);
          __delay_us(5);
          ret = gpio_pin_write_logic(&(lcd->lcd_en), GPIO_LOW);
    }
    return ret;
}

static Std_ReturnType  lcd_8bit_set_cursor(const chr_lcd_8bit_t *lcd, uint8 row, uint8 column){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
        column--;
        switch(row){
            case ROW1:
                ret = lcd_8bit_send_command(lcd, (0x80 + column));
                break;
            case ROW2:
                ret = lcd_8bit_send_command(lcd, (0xc0 + column));
                break;
            case ROW3:
                ret = lcd_8bit_send_command(lcd, (0x94 + column));
                break;
            case ROW4:
                ret = lcd_8bit_send_command(lcd, (0xd4 + column));
                break;
            default: ;
        }      
    }
    return ret;
}

static Std_ReturnType  lcd_4bit_set_cursor(const chr_lcd_4bit_t *lcd, uint8 row, uint8 column){
    Std_ReturnType ret = E_OK;
    if(NULL == lcd){
        ret = E_NOT_OK;
    }else{
        column--;
        switch(row){
            case ROW1:
                ret = lcd_4bit_send_command(lcd, (0x80 + column));
                break;
            case ROW2:
                ret = lcd_4bit_send_command(lcd, (0xc0 + column));
                break;
            case ROW3:
                ret = lcd_4bit_send_command(lcd, (0x94 + column));
                break;
            case ROW4:
                ret = lcd_4bit_send_command(lcd, (0xd4 + column));
                break;
            default: ;
        }      
    }
    return ret;
}