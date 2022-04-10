/* 
 * File:   Laboratorio7.C
 * Author: Florencio Alexander Calí Ajquijay
 * DESCRIPCION: Configure la interrupción del timer0 para incrementar otro contador cada 100ms.
 *  
 *

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
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdint.h>

#define _tmr0_value 177        //177 Para 5ms

/*------------------------------------------------------------------------------
 * CONSTANTES 
 ------------------------------------------------------------------------------*/
#define BOTON1 PORTBbits.RB0     // Asignamos un nombre al RB0
#define BOTON2 PORTBbits.RB1     // Asignamos un nombre al RB1


// Variables 
uint8_t muxeo;
uint8_t Centena;
uint8_t Decena;
uint8_t Unidad;



void setup(void);
int Display(int Numero);
void Decimales (void);

//Interrupciones
void __interrupt() isr (void)
{
    
    //---
    
   
    //---
    if (T0IF)
    { 
        switch(muxeo){
            case(0x01) : muxeo = 0x02;
            break;
            case(0x02) : muxeo = 0x04;
            break;
            case(0x04) : muxeo = 0x01;
            break;
            
        }
        PORTA++;
        T0IF = 0;
        TMR0 = _tmr0_value;     
    }
  
// INTERRUPCIÓN DEL PUERTO B
    
    if (RBIF)
    {
        if (RB0 == 0){
            PORTD++;
            
        }
        if (RB1 == 0){
            PORTD--;   
        }
        RBIF = 0;
    }      
}

void main(void){    // Configuración inicial 
    
setup();

muxeo = 0x01;
while(1)
{
Decimales();
    
switch(muxeo){
    case(0x01):
        PORTC = 0;
        PORTE = 0x01;
        Display(Centena);
        break;
        
    case(0x02):
        PORTC = 0;
        PORTE = 0x02;
        Display(Decena);
        break;
        
    case(0x04):
        PORTC = 0;
        PORTE = 0x04;
        Display(Unidad);
        break;
}   
}
}


void setup (void)
{
    //Configuracion del Oscilador
    OSCCONbits.SCS = 1;
    OSCCONbits.IRCF2 = 0;
    OSCCONbits.IRCF1 = 0;   // 125 KHz
    OSCCONbits.IRCF0 = 1;
  
    PORTA = 0X00;
    PORTC = 0X00;
    PORTB = 0X03;
    PORTD = 0X00;  
    PORTE = 0X00;
    
    ANSEL = 0X00;
    ANSELH = 0X00;
    TRISA = 0X00;
    TRISC = 0X00;
    TRISB = 0X03;
    TRISD = 0X00;
    TRISE = 0X00;


    //ENABLE PULLUP PORTB
    OPTION_REGbits.nRBPU = 0;
    IOCB = 0X0F;
    
    WPUB = 0X0F;

    // CONFIGURAR TMR0
    OPTION_REGbits.T0CS = 0;
    OPTION_REGbits.PSA = 0; 
    OPTION_REGbits.PS2 = 0; 
    OPTION_REGbits.PS1 = 0;
    OPTION_REGbits.PS0 = 0;
    TMR0 = _tmr0_value;
    
    //INTCONbits.RBIF = 0;
    INTCONbits.T0IF = 0;

    //INTCONbits.RBIE = 1;
    INTCONbits.T0IE = 1;
    INTCONbits.GIE = 1;
    
    
      
}

// Conversor

void Decimales (void){
    int Temporal;
    Centena = PORTD /100;
    Temporal = PORTD % 100;
    Decena = Temporal /10;
    Unidad = Temporal % 10;  
}

// Tabla para configurar el valor del DISPLAY de 7 segmentos 

int Display (int Numero){
    switch (Numero){
        case(0): PORTC = (0b0111111);
        break;
        case(1): PORTC = (0b0000110);
        break;
        case(2): PORTC = (0b1011011);
        break;
        case(3): PORTC = (0b1001111);
        break;
        case(4): PORTC = (0b1100110);
        break;
        case(5): PORTC = (0b1101101);
        break;
        case(6): PORTC = (0b1111101);
        break;
        case(7): PORTC = (0b1111111);
        break;
        case(8): PORTC = (0b1111111);
        break;
        case(9): PORTC = (0b1100111);
        break;
        
        
    }
    
    
    
}