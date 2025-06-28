/* 
 * File:   Practica-3-main.c
 * Authors: 
 * -Cisneros Araujo Karen
 * -Diaz Ortiz Brandon
 * -Escarcega Hernandez Steven Araujo
 * -Reyes Ortiz Marlon
 * Created on 5 de junio de 2025, 02:30 AM
 */

#pragma config FOSC = HS, WDTE = OFF, PWRTE = ON, BOREN = ON
#pragma config LVP  = OFF, CPD  = OFF, WRT = OFF, CP  = OFF

#include <xc.h>
#include <stdint.h>
#include "lcd.h"
#include "kbd4x4.h"
#include "funciones.h"

#define LED_ALARM      PORTCbits.RC0
#define LED_ALARM_DIR  TRISCbits.TRISC0
#define LED_PWM_DIR    TRISCbits.TRISC2
#define PWM1_DIR       TRISCbits.TRISC1

#define TOTAL_PAGES 4

const uint8_t idxRef[4] = {3,5, 13,15};
const uint8_t idxPWM[4] = {2,3, 7,8};

char line0[17] = "A1:0.02:0.03:0.0";
char ref0[17]  = "R1:-.-****R3:-.-";
char ref1[17]  = "Voltajes No.2   ";
char cntStr[17]= "CNT:00000       ";
char pwmStr[17]= "F:01kD:00       ";

volatile uint16_t pulseCnt = 0;

char *activeBuf = ref0;
const uint8_t *map = idxRef;
uint8_t lenMap = 4;
uint8_t page = 0, pos = 0;
char key;

int renglon = 1;
int columna = 1;

void __interrupt() ISR(void)
{
    static char rRow=0, rCol=0, rChr=0, st=0;
    if (RCIF) {
        char d = RCREG;
        if      (st==0){ rRow=d; st=1; }
        else if (st==1){ rCol=d; st=2; }
        else { rChr=d; st=0; Lcd_Set_Cursor(rRow, rCol); Lcd_Write_Char(rChr); Lcd_Set_Cursor(renglon, columna); }
    }
    if (T0IF) { pulseCnt += 256; T0IF = 0; }
}

static inline uint8_t toDeciv(uint16_t v){ return (uint8_t)((v*50U+511U)/1023U); }

static void ADC_Init(void){ AN0_DIR=AN1_DIR=AN2_DIR=1; ADCON1=0b10001110; AN0_SELECT(); ADCON0bits.ADON=1; __delay_ms(2); }

static inline uint16_t adcRead(uint8_t ch){ if(ch==1)AN1_SELECT(); else if(ch==2)AN2_SELECT(); else AN0_SELECT(); __delay_us(15); GO_nDONE=1; while(GO_nDONE) ; return ((uint16_t)ADRESH<<8)|ADRESL; }

static void PWM_LED_Init(void){ LED_PWM_DIR=0; CCP1CON=0b00001100; T2CON=0b00000111; PR2=124; }

static void PWM_LED_Set(uint8_t pct){ uint16_t r=pct*5; if(r>500) r=500; CCPR1L=(uint8_t)(r>>2); CCP1CONbits.CCP1Y=(r>>1)&1; CCP1CONbits.CCP1X=r&1; }

static void PWM1_Init(void){ PWM1_DIR=0; CCP2CON=0b00001100; }

static void PWM1_Set(uint8_t kHz,uint8_t duty){ if(kHz<1)kHz=1; if(kHz>62)kHz=62; PR2=62-kHz; uint16_t r=duty*5; if(r>500) r=500; CCPR2L=(uint8_t)(r>>2); CCP2CONbits.CCP2Y=(r>>1)&1; CCP2CONbits.CCP2X=r&1; }

static inline uint8_t refToDeciv(const char *p){ return (p[0]=='-'||p[2]=='-')?255:(uint8_t)((p[0]-'0')*10 + p[2]-'0'); }

static inline void buildLine0(uint8_t d0,uint8_t d1,uint8_t d2){ line0[3]=d0/10+'0'; line0[4]='.'; line0[5]=d0%10+'0'; line0[8]=d1/10+'0'; line0[9]='.'; line0[10]=d1%10+'0'; line0[13]=d2/10+'0'; line0[14]='.'; line0[15]=d2%10+'0'; }

static inline void cursorLCD(void){ Lcd_Set_Cursor(2, map[pos]+1); }

static void showPage(uint8_t pg){
    Lcd_Clear();
    switch(pg){
        case 0:
            Lcd_Set_Cursor(1,1); Lcd_Write_String(line0);
            Lcd_Set_Cursor(2,1); Lcd_Write_String(ref0);
            activeBuf=ref0; map=idxRef; lenMap=4; pos=0;
            Lcd_Blink(); cursorLCD(); break;
        case 1:
            Lcd_Set_Cursor(1,1); Lcd_Write_String("SEGUNDA PANTALLA");
            Lcd_Set_Cursor(2,1); Lcd_Write_String(ref1);
            activeBuf=ref1; map=idxRef; lenMap=4; pos=0;
            Lcd_Blink(); cursorLCD(); break;
        case 2:
            Lcd_Set_Cursor(1,1); Lcd_Write_String(cntStr);
            Lcd_Set_Cursor(2,1); Lcd_Write_String(pwmStr);
            activeBuf=pwmStr; map=idxPWM; lenMap=4; pos=0;
            Lcd_Blink(); cursorLCD(); break;
        case 3:
            Lcd_Set_Cursor(1,1); Lcd_Write_String("SEGUNDA PANTALLA");
            Lcd_Set_Cursor(2,1); Lcd_Write_String("Segundo PWM");
            Lcd_NoBlink(); break;
    }
}

void main(void)
{
    LED_ALARM_DIR=0; LED_ALARM=0; BUZZER_DIR=0; BUZZER=1;

    PWM_LED_Init(); PWM1_Init(); ADC_Init();

    TMR0=0; OPTION_REG=0b00000001; T0IE=1; GIE=1;

    Keypad_Init(); Lcd_Init(); showPage(page);

    while(1){
        if(page==0){
            uint8_t d0=toDeciv(adcRead(0));
            uint8_t d1=toDeciv(adcRead(1));
            uint8_t d2=toDeciv(adcRead(2));
            buildLine0(d0,d1,d2);
            Lcd_Set_Cursor(1,1); Lcd_Write_String(line0);

            uint8_t r1=refToDeciv(&ref0[3]);
            LED_ALARM = (r1!=255 && d0>r1);

            PWM_LED_Set(d1*2);

            uint8_t r3=refToDeciv(&ref0[13]);
            BUZZER = (r3!=255 && d2>r3) ? 0 : 1;
        }

        if(page==2){
            uint16_t total=pulseCnt+TMR0;
            cntStr[4]=(total/10000)%10+'0';
            cntStr[5]=(total/1000)%10+'0';
            cntStr[6]=(total/100)%10+'0';
            cntStr[7]=(total/10)%10+'0';
            cntStr[8]= total%10+'0';
            Lcd_Set_Cursor(1,1); Lcd_Write_String(cntStr);
        }

        key=Keypad_Get_Char(); if(!key) continue;

        if(key=='D'){ page=(page+1)%TOTAL_PAGES; showPage(page); __delay_ms(120); continue; }
        if(page==3) continue;

        switch(key){
            case '*': pos=(pos==0)?lenMap-1:pos-1; cursorLCD(); break;
            case '#': pos=(pos==lenMap-1)?0:pos+1; cursorLCD(); break;
            case 'B': activeBuf[map[pos]]='-'; Lcd_Set_Cursor(2,1); Lcd_Write_String(activeBuf); cursorLCD(); break;
            default:
                if(key>='0'&&key<='9'){
                    activeBuf[map[pos]]=key;
                    Lcd_Set_Cursor(2,1); Lcd_Write_String(activeBuf);
                    pos=(pos==lenMap-1)?0:pos+1; cursorLCD();
                    if(page==2){
                        uint8_t k=(pwmStr[2]-'0')*10+(pwmStr[3]-'0');
                        uint8_t d=(pwmStr[7]-'0')*10+(pwmStr[8]-'0');
                        PWM1_Set(k,d);
                    }
                }
        }
    }
}