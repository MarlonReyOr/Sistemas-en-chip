/* 
 * File:   lcd.h
 * Author: Marlon
 *
 * Created on 5 de junio de 2025, 01:57 AM
 */

#include <xc.h>
#define _XTAL_FREQ 4000000

//#define USE_CGRAM_LCD

#define RS PORTCbits.RC0
#define EN PORTCbits.RC2
#define D4 PORTBbits.RB4
#define D5 PORTBbits.RB5
#define D6 PORTBbits.RB6
#define D7 PORTBbits.RB7

#define RS_DIR TRISCbits.TRISC0
#define EN_DIR TRISCbits.TRISC2
#define D4_DIR TRISBbits.TRISB4
#define D5_DIR TRISBbits.TRISB5
#define D6_DIR TRISBbits.TRISB6
#define D7_DIR TRISBbits.TRISB7

void Lcd_Port(char a);
void Lcd_Cmd(char a);
void Lcd_Clear(void);
void Lcd_Set_Cursor(char a, char b);
void Lcd_Init(void);
void Lcd_Write_Char(char a);
void Lcd_Write_String(const char *a);
void Lcd_Shift_Right(void);
void Lcd_Shift_Left(void);
void Lcd_Blink(void);
void Lcd_NoBlink(void);
void Lcd_Delete_Char(int renglon, int columna);


#ifdef USE_CGRAM_LCD
//void Lcd_CGRAM_CreateChar(char add, const char* chardata);
void Lcd_CGRAM_Init(void);
void Lcd_CGRAM_Close(void);
#endif
