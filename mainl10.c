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

/*------------------------------------------------------------------------------
 * PROTOTIPO DE FUNCIONES 
 ------------------------------------------------------------------------------*/

void setup (void);
void pollo(char data);
void string(char *str);

/*
 * ----------------------------- MAIN CONFIGURACION --------------------------- 
 */
void main (void)
{
    setup ();
    
    //---------------------- Loop principal ------------------
    while (1)
    {
        __delay_ms(500);
        string("\r AMEN \r");
        

        }
        return;
}
/*
 * -------------------------------- Funciones --------------------------------
 */
void setup (void) 
{
    //Configuración de puertos
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0;
    PORTB = 0;
    
    TRISA = 0;
    PORTA = 0;
    
    //Configuración del oscilador a 1MHz
    OSCCONbits.IRCF = 0b110;
    OSCCONbits.SCS = 1;
    
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
}

    
    void pollo(char data)
    {
        while(TXSTAbits.TRMT == 0);
        TXREG = data    ;
    }

    void string (char *str){
        while(*str != '\0'){
            pollo(*str);
            str++;
        }
    }