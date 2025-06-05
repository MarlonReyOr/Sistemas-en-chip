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

void USART_Init(long baud) {
    TRISC6 = 0; // TX pin como salida
    TRISC7 = 1; // RX pin como entrada
    SPBRG = (_XTAL_FREQ / (64 * baud)) - 1;
    TXEN = 1; // Habilita transmisión
    SYNC = 0; // Asíncrono
    SPEN = 1; // Habilita el puerto serial
    CREN = 1; // Habilita recepción continua
}

void USART_Write(char data) {
    while(!TXIF); // Espera que se pueda transmitir
    TXREG = data; // Escribe el dato
}

void __interrupt() ISR() {
    static char renglon_recibido = 0, columna_recibida = 0, caracter_recibido = 0;
    static char estado = 0;

    if (RCIF) { // Si se recibe algo por UART
        char dato = RCREG;

        if (estado == 0) {
            renglon_recibido = dato;    // Primer dato: fila
            estado = 1;
        }
        else if (estado == 1) {
            columna_recibida = dato; // Segundo dato: columna
            estado = 2;
        }
        else if (estado == 2) {
            caracter_recibido = dato; // Tercer dato: carácter
            estado = 0;

            // Muestra el carácter recibido en la LCD
            Lcd_Set_Cursor(renglon_recibido, columna_recibida);
            Lcd_Write_Char(caracter_recibido);
            Lcd_Set_Cursor(renglon, columna);
        }
    }
}

void enviarInformacion(int renglon,int columna,char caracter){
    USART_Write((char)renglon);
    USART_Write((char)columna);
    USART_Write(caracter);
}