/* 
 * File:   mainl10.c
 * Author: saras
 *
 * Created on May 4, 2022, 2:58 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>

/*------------------------------------------------------------------------------
 * CONSTANTES 
 ------------------------------------------------------------------------------*/
#define _XTAL_FREQ 1000000

/*------------------------------------------------------------------------------
 * VARIABLES 
 ------------------------------------------------------------------------------*/
char val;
uint8_t number;

/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/

void setup (void);
void pollo(char data);
void string(char *str);
void setup(void);

/*------------------------------------------------------------------------------
 * INTERRUPCIONES 
 ------------------------------------------------------------------------------*/
void __interrupt() isr (void){
    if(PIR1bits.ADIF){                      // Fue interrupción del ADC?
        if(ADCON0bits.CHS == 0){            // Verificamos sea AN0 el canal seleccionado
            RCREG = ADRESH;
            PIR1bits.ADIF = 0;                  // Limpiamos bandera de interrupción
        }
        return;
    }
}

/*
 * ----------------------------- MAIN -------------- --------------------------- 
 */


void main (void) {
    setup();
    while(1)
    {
        string("\r Menu \r");
        string("1. Leer potenciometro \r");
        string("2. Enviar Ascii \r");
        
        while(PIR1bits.RCIF == 0);
        val = RCREG;
        
        switch(val)
        {
            case('1'):
                string("\r Valor de potenciometro \r");
                number = 1;
                while(number == 1){
                    if(ADCON0bits.GO == 0){             // No hay proceso de conversion
                        __delay_us(40);
                        ADCON0bits.GO = 1;              // Iniciamos proceso de conversión
                        }
  
                }
                    number = 0;
                break;
                
                
            case('2'):
                string("\r Enviar Ascii \r");
                number = 1;
                while(number == 1){
                    if(PIR1bits.RCIF){         // Hay datos recibidos?
                    val = RCREG;     // Guardamos valor recibido en el arreglo mensaje
                    TXREG = val;
                    string("\r --------------------- \r");
                    number = 0;
                    break;
                }
                }


        }
        return;
}
}

/*
 * -------------------------------- Funciones --------------------------------
 */
void setup (void) {
    //Configuración de puertos
    ANSEL = 0b1;
    ANSELH = 0;
    
    
    TRISA = 0b1;
    PORTA = 0b1;
    
    //Configuración del oscilador a 1MHz
    OSCCONbits.IRCF = 0b110;
    OSCCONbits.SCS = 1;
    
    // Configuración ADC
    ADCON0bits.ADCS = 0b01;     // Fosc/8
    ADCON1bits.VCFG0 = 0;       // VDD
    ADCON1bits.VCFG1 = 0;       // VSS
    ADCON0bits.CHS = 0b0000;    // Seleccionamos el AN0
    ADCON1bits.ADFM = 0;        // Justificado a la izquierda
    ADCON0bits.ADON = 1;        // Habilitamos modulo ADC
    __delay_us(40);             // Sample time
    
    
    //Configuración de TX y RX
    TXSTAbits.SYNC = 0;
    TXSTAbits.BRGH = 0;
    BAUDCTLbits.BRG16 = 1;
    
    SPBRG = 25;                 //SPBRGH : SPBRG = 25
    SPBRGH = 0;
    
    RCSTAbits.SPEN = 1;
    RCSTAbits.RX9 = 0;          //Modo 8 bits
    RCSTAbits.CREN = 1;         //Habilitmaos la recpeción
    TXSTAbits.TXEN = 1;         //Habilitamos la transmisión
    //TXSTAbits.TX9 = 0;
    
     // Configuracion interrupciones
    PIR1bits.ADIF = 0;          // Limpiamos bandera de ADC
    PIE1bits.ADIE = 1;          // Habilitamos interrupcion de ADC
    INTCONbits.PEIE = 1;        // Habilitamos int. de perifericos
    INTCONbits.GIE = 1;         // Habilitamos int. globales
}

void pollo(char data)
    {
        while(TXSTAbits.TRMT == 0);
        TXREG = data;
    }

    void string (char *str){
        while(*str != '\0'){
            pollo(*str);
            str++;
        }
    }
    
    