/* 
 * File:   Practica-3-main.c
 * Author: Marlon
 *
 * Created on 5 de junio de 2025, 02:01 AM
 */
// CONFIGURACI�N DEL PIC
#pragma config FOSC = HS        // Oscilador: Cristal de alta velocidad
#pragma config WDTE = OFF       // Watchdog Timer deshabilitado
#pragma config PWRTE = ON      // Power-up Timer deshabilitado
#pragma config BOREN = ON      // Brown-out Reset deshabilitado
#pragma config LVP = OFF        // Programaci�n en baja tensi�n deshabilitada
#pragma config CPD = OFF        // Protecci�n de datos EEPROM deshabilitada
#pragma config WRT = OFF        // Protecci�n de escritura de memoria Flash deshabilitada
#pragma config CP = OFF         // Protecci�n de c�digo deshabilitada

#define _XTAL_FREQ 4000000
#include <xc.h>
#include <string.h>
#include <stdlib.h>
#include "lcd.h"          // Libreria de la pantalla lcd
#include "kbd4x4.h"       // Libreria del teclado matricial 4x4
#include "ascii_chars.h"  // Archivo de caracteres
#include "funciones.h"

int renglon=1;
int columna=1;
    
void main()
{
	PIN = 0x0F;  // Configura los pines como digitales
    Keypad_Init();  // Inicializa el teclado matricial
    Lcd_Init();  // Inicializa la pantalla LCD
    Lcd_Blink();
    BUZZER_DIR=0;
    BUZZER=1;
    
	int CaracteresEscritos=0;
	int CaracterEnAsc=0;
    
	char caracter_asc[4];  // Arreglo para almacenar los caracteres ingresados
	char caracter;
	
	char tecla;
	Lcd_Set_Cursor(renglon,columna);
    
    USART_Init(9600); // Inicializa RS232 a 9600 baudios
    RCIE = 1; // Habilita interrupci�n de recepci�n UART
    PEIE = 1; // Habilita interrupciones perif�ricas
    GIE = 1;  // Habilita interrupciones globales
    
	while(1)
    {
		tecla = Keypad_Get_Char();
		if(tecla != 0)  // Verifica si se ha presionado alguna tecla
        {
			switch(tecla)
			{
				case 'A':
                    Buzzer_On(50);  // Hace sonar el zumbador durante 500 ms
					if(renglon==1){
                        Buzzer_On(50);  // Hace sonar el zumbador durante 500 ms
						renglon=2;
						Lcd_Set_Cursor(renglon,columna);
					}
					else
					{
						renglon=1;
						Lcd_Set_Cursor(renglon,columna);
					}
					break;
				case 'B':
                    if(renglon==2){
                        if(columna > 1)  
                        {
                            Lcd_Delete_Char(renglon, columna-1);  
                            columna--;  
                            enviarInformacion(renglon,columna,' ');
                            Buzzer_On(50);  // Hace sonar el zumbador durante 500 ms
                        }else{
                            renglon=1;
                            columna=16;
                            Lcd_Delete_Char(renglon, columna);  // Borra la �ltima tecla
                            enviarInformacion(renglon,columna,' ');
                            Buzzer_On(50);  // Hace sonar el zumbador durante 500 ms
                        }
                    }else{
                        if(columna > 1)  
                        {
                            Lcd_Delete_Char(renglon, columna-1); 
                            columna--;  
                            enviarInformacion(renglon,columna,' ');
                            Buzzer_On(50);  // Hace sonar el zumbador durante 500 ms
                        }else{
                            renglon=2;
                            columna=16;
                            Lcd_Delete_Char(renglon, columna); 
                            enviarInformacion(renglon,columna,' ');
                            Buzzer_On(50);  // Hace sonar el zumbador durante 500 ms
                        }
                    }
					break;
				case 'C':
					CaracteresEscritos=0;
                    memset(caracter_asc, 0, sizeof(caracter_asc));  // Borra todos los caracteres de la cadena
                    Buzzer_On(250);  // Hace sonar el zumbador durante 500 ms
					break;
				case 'D':
					 caracter_asc[3] = '\0';  // A�ade el car�cter nulo al final del arreglo
                    // Convertir la clave ingresada a un n�mero entero
                    CaracterEnAsc = atoi(caracter_asc);

                    // Verificar si el c�digo ASCII est� dentro del rango v�lido y es imprimible
                    if(CaracterEnAsc >= 32 && CaracterEnAsc < 127)
                    {
                        Buzzer_On(200);  // Hace sonar el zumbador durante 500 ms
                        caracter = ASCII_CHARS[CaracterEnAsc];  // Obtener el car�cter ASCII correspondiente
                        if(renglon==2){
                            if(columna==16){
                                Lcd_Write_Char(caracter);  // Mostrar el car�cter ASCII
                                enviarInformacion(renglon,columna,caracter);
                                renglon=1;
                                columna=1;
                                Lcd_Set_Cursor(renglon,columna);
                                CaracteresEscritos=0;
                                memset(caracter_asc, 0, sizeof(caracter_asc));
                            }else{
                                Lcd_Write_Char(caracter);  // Mostrar el car�cter ASCII
                                enviarInformacion(renglon,columna,caracter);
                                columna++;
                                Lcd_Set_Cursor(renglon,columna);
                                CaracteresEscritos=0;
                                memset(caracter_asc, 0, sizeof(caracter_asc));
                            }
                        }else{
                            if(columna==16)
                            {
                                Lcd_Write_Char(caracter);  // Mostrar el car�cter ASCII
                                enviarInformacion(renglon,columna,caracter);
                                renglon=2;
                                columna=1;
                                Lcd_Set_Cursor(renglon,columna);
                                CaracteresEscritos=0;
                                memset(caracter_asc, 0, sizeof(caracter_asc));
                            }else{
                                Lcd_Write_Char(caracter);  // Mostrar el car�cter ASCII
                                enviarInformacion(renglon,columna,caracter);
                                columna++;
                                Lcd_Set_Cursor(renglon,columna);
                                CaracteresEscritos=0;
                                memset(caracter_asc, 0, sizeof(caracter_asc));
                            }
                        }                    
                    }
                    else  // Si el c�digo ASCII no es v�lido o no es imprimible
                    {
                        Buzzer_On(700);  // Hace sonar el zumbador durante 500 ms
                        CaracteresEscritos=0;
                        memset(caracter_asc, 0, sizeof(caracter_asc));
                    }
					break;
				case '*':
                    Buzzer_On(50);  // Hace sonar el zumbador durante 500 ms
					if(renglon==2){
                        if(columna > 1)  
                        {
                            columna--;
                            Lcd_Set_Cursor(renglon,columna);
                        }else{
                            renglon--;
                            columna=16;
                            Lcd_Set_Cursor(renglon,columna);
                        }
                    }else{
                        if(columna > 1)  
                        {
                            columna--;
                            Lcd_Set_Cursor(renglon,columna);
                        }
                    }
					break;
				case '#':
                    Buzzer_On(50);  // Hace sonar el zumbador durante 500 ms
					if(renglon==1){
                        if(columna == 16)  
                        {
                            renglon++;
                            columna=1;
                            Lcd_Set_Cursor(renglon,columna);
                        }else{
                            columna++;
                            Lcd_Set_Cursor(renglon,columna);
                        }
                    }else{
                        if(columna < 16)  
                        {
                            columna++;
                            Lcd_Set_Cursor(renglon,columna);
                        }
                    }
					break;
				default:
					if(CaracteresEscritos>=3){
                        Buzzer_On(500);  // Hace sonar el zumbador durante 500 ms//zumbar lleno
                    }
                    else{
                        Buzzer_On(50);  // Hace sonar el zumbador durante 500 ms
                        caracter_asc[CaracteresEscritos]=tecla;
                        CaracteresEscritos++;
                    }
					break;
			}
		}
    }
}