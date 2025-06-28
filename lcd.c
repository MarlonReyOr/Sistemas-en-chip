/* 
 * File:   lcd.c
 * Authors: 
 * -Cisneros Araujo Karen
 * -Diaz Ortiz Brandon
 * -Escarcega Hernandez Steven Araujo
 * -Reyes Ortiz Marlon
 * Created on 5 de junio de 2025, 02:21 AM
 */

#include "lcd.h"

void Lcd_Port(char a)
{
    (a & 1) ? (D4 = 1) : (D4 = 0);
    (a & 2) ? (D5 = 1) : (D5 = 0);
    (a & 4) ? (D6 = 1) : (D6 = 0);
    (a & 8) ? (D7 = 1) : (D7 = 0);
}

void Lcd_Cmd(char a)
{
    RS = 0;
    Lcd_Port(a);
    EN = 1;
    __delay_us(1);  // Pulso mínimo
    EN = 0;
    __delay_ms(2);  // Tiempo para procesar comando
}

void Lcd_Clear(void)
{
    Lcd_Cmd(0);
    Lcd_Cmd(1);
    __delay_ms(2);  // Clear necesita más tiempo
}

void Lcd_Set_Cursor(char a, char b)
{
    char temp,z,y;
    if(a == 1)
    {
        temp = 0x80 + b - 1;
        z = temp>>4;
        y = temp & 0x0F;
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    }
    else if(a == 2)
    {
        temp = 0xC0 + b - 1;
        z = temp>>4;
        y = temp & 0x0F;
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    }
    else if(a == 3)
    {
        temp = 0x94 + b - 1;
        z = temp>>4;
        y = temp & 0x0F;
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    }
    else if(a == 4)
    {
        temp = 0xD4 + b - 1;
        z = temp>>4;
        y = temp & 0x0F;
        Lcd_Cmd(z);
        Lcd_Cmd(y);
    }
}

void Lcd_Init(void)
{
    // Configurar pines como salida
    RS_DIR = 0;
    EN_DIR = 0;
    D4_DIR = 0;
    D5_DIR = 0;
    D6_DIR = 0;
    D7_DIR = 0;
    
    // Inicializar pines en estado bajo
    RS = 0;
    EN = 0;
    Lcd_Port(0x00);
    
    // Esperar estabilización del LCD (>15ms después de Vcc)
    __delay_ms(50);
    
    // Secuencia de inicialización para modo 4 bits
    // Primer intento - modo 8 bits
    Lcd_Port(0x03);
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_ms(5);
    
    // Segundo intento - modo 8 bits
    Lcd_Port(0x03);
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_ms(1);
    
    // Tercer intento - modo 8 bits
    Lcd_Port(0x03);
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_ms(1);
    
    // Cambiar a modo 4 bits
    Lcd_Port(0x02);
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_ms(1);
    
    // Ahora el LCD está en modo 4 bits
    // Configurar: 4 bits, 2 líneas, 5x8 matriz
    Lcd_Cmd(0x02);  // Nibble alto de 0x28
    Lcd_Cmd(0x08);  // Nibble bajo de 0x28
    
    // Display OFF
    Lcd_Cmd(0x00);  // Nibble alto de 0x08
    Lcd_Cmd(0x08);  // Nibble bajo de 0x08
    
    // Clear display
    Lcd_Cmd(0x00);  // Nibble alto de 0x01
    Lcd_Cmd(0x01);  // Nibble bajo de 0x01
    __delay_ms(2);
    
    // Entry mode: incrementar cursor, no shift
    Lcd_Cmd(0x00);  // Nibble alto de 0x06
    Lcd_Cmd(0x06);  // Nibble bajo de 0x06
    
    // Display ON, cursor OFF, blink OFF
    Lcd_Cmd(0x00);  // Nibble alto de 0x0C
    Lcd_Cmd(0x0C);  // Nibble bajo de 0x0C
    
    __delay_ms(2);
}

void Lcd_Write_Char(char a)
{
    char temp,y;
    temp = a & 0x0F;      // Nibble bajo
    y = (a & 0xF0) >> 4;  // Nibble alto
    
    RS = 1;  // Modo datos
    
    // Enviar nibble alto
    Lcd_Port(y);
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_us(50);
    
    // Enviar nibble bajo
    Lcd_Port(temp);
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_us(50);
}

void Lcd_Write_String(const char *a)
{
    int i;
    for(i=0; a[i]!='\0' && i<16; i++)  // Limitar a 16 caracteres
        Lcd_Write_Char(a[i]);
}

void Lcd_Blink(void)
{
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x0F);
}

void Lcd_NoBlink(void)
{
    Lcd_Cmd(0x00);
    Lcd_Cmd(0x0C);
}

void Lcd_Delete_Char(int renglon, int columna) {
    Lcd_Set_Cursor(renglon,columna);
    Lcd_Write_Char(' ');  
    Lcd_Set_Cursor(renglon,columna);
}