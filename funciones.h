/* 
 * File:   funciones.h
 * Authors: 
 * -Cisneros Araujo Karen
 * -Diaz Ortiz Brandon
 * -Escarcega Hernandez Steven Araujo
 * -Reyes Ortiz Marlon
 * Created on 5 de junio de 2025, 02:18 AM
 */

 
#ifndef FUNCIONES_H
#define FUNCIONES_H

#define BUZZER PORTCbits.RC4
#define BUZZER_DIR TRISCbits.TRISC4

#define AN0_PIN       PORTAbits.RA0      // lectura de nivel
#define AN0_DIR       TRISAbits.TRISA0   // 1 = entrada
#define AN0_SELECT()  (ADCON0 = (ADCON0 & 0b11000101) | (0 << 3))

/* Pin físico ? RA1  ?  AN1 */
#define AN1_PIN       PORTAbits.RA1
#define AN1_DIR       TRISAbits.TRISA1
#define AN1_SELECT()  (ADCON0 = (ADCON0 & 0b11000101) | (1 << 3))

/* Pin físico ? RA2  ?  AN2 */
#define AN2_PIN       PORTAbits.RA2
#define AN2_DIR       TRISAbits.TRISA2
#define AN2_SELECT()  (ADCON0 = (ADCON0 & 0b11000101) | (2 << 3))

/* Para dejar claros los bits de configuración analógica */
#define ANALOG_INPUTS_MASK  0b00000111   // RA0-RA2

extern int renglon;
extern int columna;

void Buzzer_On(unsigned int duration_ms);
void USART_Init(long baud);
void USART_Write(char data);
void enviarInformacion(int renglon, int columna, char caracter);
void __interrupt() ISR();

#endif