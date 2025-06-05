/* 
 * File:   funciones.h
 * Author: Marlon
 *
 * Created on 5 de junio de 2025, 02:18 AM
 */

#ifndef FUNCIONES_H
#define FUNCIONES_H

#define BUZZER PORTAbits.RA4
#define BUZZER_DIR TRISAbits.TRISA4
#define PIN ADCON1bits.PCFG
extern int renglon;
extern int columna;

void Buzzer_On(unsigned int duration_ms);
void USART_Init(long baud);
void USART_Write(char data);
void enviarInformacion(int renglon, int columna, char caracter);
void __interrupt() ISR();

#endif
