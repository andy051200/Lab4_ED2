/*------------------------------------------------------------------------------
Archivo: mainsproject.s
Microcontrolador: PIC16F887
Autor: Andy Bonilla
Compilador: pic-as (v2.30), MPLABX v5.50
    
Programa: laboratorio 4
Hardware: PIC16F887
    
Creado: 10 de agosto de 2021    
Descripcion: un laboratoria bien fumado tbh pero chilero
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

/*-----------------------------------------------------------------------------
 ----------------------------LIBRERIAS-----------------------------------------
 -----------------------------------------------------------------------------*/
#include <stdint.h>
#include <pic16f887.h>
#include "I2C.h"
#include <xc.h>

/*-----------------------------------------------------------------------------
 ------------------------DIRECTIVAS DE COMPILADOR------------------------------
 -----------------------------------------------------------------------------*/
#define _XTAL_FREQ 8000000

/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/
uint8_t z;
uint8_t dato;
unsigned char antirrebote1=0; //variable de antirrebote para boton SUMA
unsigned char antirrebote2=0; //variable de antirrebote para boton RESTA
unsigned char cuenta;        //variable para contar PORTD


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
            case(0b11111110):     //si se apacha el boton de SUMA
                antirrebote1=1;
                break;
            
            case(0b11111101):     //si se apacha el boton de RESTA
                antirrebote2=1;
                break;
            
            default:                    //si alguno no es apachado
                antirrebote1=0;
                antirrebote2=0;
                break;
        }
        INTCONbits.RBIF=0;              //se apaga bandera de interrupcion
    }
      
    if(PIR1bits.SSPIF == 1)
    { 
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
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupci�n recepci�n/transmisi�n SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepci�n se complete
            PORTA = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepci�n
            __delay_us(10);
            
        }
        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW)
        {
            z = SSPBUF;
            BF = 0;
            SSPBUF = PORTD;         //se escriba al buffer el valor delPortD
            SSPCONbits.CKP = 1;
            __delay_us(10);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}
/*-----------------------------------------------------------------------------
 ---------------------------- MAIN  Y LOOP ------------------------------------
 -----------------------------------------------------------------------------*/
void main(void) 
{
    setup();
    
    while(1)
    {
        botones();      //se jala función de botones
        if (cuenta==15)
            cuenta=0;
        PORTE=PORTD;
    }
    return;
}
/*-----------------------------------------------------------------------------
 ---------------------------------- SET UP -----------------------------------
 -----------------------------------------------------------------------------*/
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISBbits.TRISB0=1;     //RB0 como entrada, boton SUMA
    TRISBbits.TRISB1=1;     //RB1 como entrada, boton RESTA
    TRISD = 0;
    TRISE = 0;
    
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    cuenta=0;
    //WEAK PULL UPs PORTB
    OPTION_REGbits.nRBPU = 0;   // enable Individual pull-ups
    WPUBbits.WPUB0 = 1;         // enable Pull-Up de RB0 
    WPUBbits.WPUB1 = 1;         // enable Pull-Up de RB1 
    
    //CONFIGURACION DE ESCLAVO I2C
    I2C_Slave_Init(0x50);       //configuracion y asignacion de direccion
    
    //CONFIGURACION DE INTERRUPCIONES
    INTCONbits.GIE=1;           //se habilitan las interrupciones globales
    INTCONbits.RBIE=1;          // se habilita IntOnChange
    INTCONbits.RBIF=0;          // se apaga la bandera de IntOnChangeB  
    IOCBbits.IOCB0=1;           //se habilita IOCB RB0
    IOCBbits.IOCB1=1;           //se habilita IOCB RB1
    return;
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
    
    return;
}