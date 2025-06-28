/* 
 * File:   funciones.c
 * Authors: 
 * -Cisneros Araujo Karen
 * -Diaz Ortiz Brandon
 * -Escarcega Hernandez Steven Araujo
 * -Reyes Ortiz Marlon
 * Created on 5 de junio de 2025, 02:20 AM
 */
#include <xc.h>
#include "lcd.h"
#include "funciones.h"


void Buzzer_On(unsigned int duration_ms)
{
    BUZZER = 0;  // Activa el zumbador
    while(duration_ms--)
    {
        __delay_ms(1);  // Espera 1 ms
    }
    BUZZER = 1;  // Desactiva el zumbador
}

void enviarInformacion(int renglon,int columna,char caracter){
    USART_Write((char)renglon);
    USART_Write((char)columna);
    USART_Write(caracter);
}
