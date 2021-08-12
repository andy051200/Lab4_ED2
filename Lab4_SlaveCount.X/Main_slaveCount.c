/*------------------------------------------------------------------------------
Archivo: mainsproject.s
Microcontrolador: PIC16F887
Autor: Andy Bonilla
Compilador: pic-as (v2.30), MPLABX v5.45
    
Programa: laboratorio 4
Hardware: PIC16F887
    
Creado: 16 de julio de 2021    
Descripcion: 
------------------------------------------------------------------------------*/
// CONFIG1
#pragma config FOSC = EXTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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

/*-----------------------------------------------------------------------------
 ----------------------------LIBRERIAS-----------------------------------------
 -----------------------------------------------------------------------------*/
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include <xc.h>
#include <proc/pic16f887.h>
/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/
#define _XTAL_FREQ 8000000
uint8_t z;
uint8_t dato;
unsigned char antirrebote1; //control de suma
unsigned char antirrebote2; //control de resta
unsigned char cuenta;
/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);
void botones(void);
/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void)
{
    if (INTCONbits.RBIF)
    {
        switch(PORTB)
        {
            case(0b11111110):
                antirrebote1=1;
                break;
                
            case(0b11111101):
                antirrebote2=1;
                break;
            default:
                antirrebote1=0;
                antirrebote2=0;
                break;
        }
        INTCONbits.RBIF=0;
    }
    
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL))
        {
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) 
        {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupci�n recepci�n/transmisi�n SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepci�n se complete
            PORTA = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepci�n
            __delay_us(250);
            
        }
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW)
        {
            z = SSPBUF;
            BF = 0;
            SSPBUF = cuenta;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}
/*-----------------------------------------------------------------------------
 ---------------------------- MAIN Y LOOP -------------------------------------
 -----------------------------------------------------------------------------*/
void main(void) {
    setup();
    
    while(1)
    {
        botones();
        
    }
    return;
}
/*-----------------------------------------------------------------------------
 ---------------------------------- SET UP -----------------------------------
 -----------------------------------------------------------------------------*/
void setup(void){
    //-------CONFIGURACION ENTRADAS ANALOGICAS
    ANSEL = 0;                          //no hay entrada analogicas
    ANSELH = 0;                         //no hay entrada analogicas
    //-------CONFIGURACION IN/OUT
    TRISA = 0;                          //PORTA como salida
    TRISBbits.TRISB0=1;                 //RA0 como entrada, boton suma
    TRISBbits.TRISB1=1;                 //RA1 como entrada, boton resta
    TRISD = 0;                          //PortD como salida para contador 4bits
    //-------LIMPIEZA DE PUERTOS
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    //-------CONFIGURACION DE WEAK PULL UPS
    OPTION_REGbits.nRBPU=0;             //se activan WPUB
    WPUBbits.WPUB0=1;                   //RB0, boton suma
    WPUBbits.WPUB1=1;                   //RB1, boton resta
    //-------CONFIGURACION DE I2C
    I2C_Slave_Init(0x50);               //se define direccion de i2c 
    //-------CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE=1;                   //se habilitan interrupciones globales
    INTCONbits.RBIE=1;                  //se  habilita IntOnChange B
    INTCONbits.RBIF=0;                  //se  apaga bandera IntOnChange B
    IOCBbits.IOCB0=1;                   //habilita IOCB RB0
    IOCBbits.IOCB1=1;                   //habilita IOCB RB1
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void botones(void)
{
    //-----antirrebote boton SUMA
    if (antirrebote1==1 && PORTBbits.RB0==0)
    {
        cuenta++;
        PORTD=cuenta;
        antirrebote1=0;
    }
    //-----antirrebote boton RESTA
    if (antirrebote2==1 && PORTBbits.RB1==0)
    {
        cuenta--;
        PORTD=cuenta;
        antirrebote2=0;
    }
    if (cuenta >=16 && cuenta <=255)
        cuenta=0;
    
    return;
}