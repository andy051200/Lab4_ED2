/*------------------------------------------------------------------------------
Archivo: mainsproject.s
Microcontrolador: PIC16F887
Autor: Andy Bonilla
Compilador: pic-as (v2.30), MPLABX v5.45
    
Programa: laboratorio 4
Hardware: PIC16F887
    
Creado: 8 de agosto de 2021    
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
#include "LCD.h"
/*-----------------------------------------------------------------------------
 ------------------------DIRECTIVAS DE COMPILADOR------------------------------
 -----------------------------------------------------------------------------*/
#define _XTAL_FREQ 8000000

/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);

/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
void __interrupt() isr(void)
{}

/*-----------------------------------------------------------------------------
 ---------------------------- MAIN  Y LOOP ------------------------------------
 -----------------------------------------------------------------------------*/
void main(void) 
{
    setup();
    lcd_clear();        //se limpia la lcd inicialmente
    lcd_init();         //invoco la funcion de inicializacion de la lcd
	cmd(0x90);          //invocao la funcion de configurcion de comandos lc
    __delay_ms(1);
    while(1)
    {
        //------llamado de funciones para comunicacion i2c
        I2C_Master_Start();             //inicializacion de comunicacion
        I2C_Master_Write(0x50);         //se pone direccion
        I2C_Master_Write(PORTA);        //se manda la informacion
        I2C_Master_Stop();              //se para la comunicacion
        __delay_ms(200);
       
        I2C_Master_Start();             //inicializacion de comunicacion
        I2C_Master_Write(0x51);         //se pone direccion
        PORTE = I2C_Master_Read(0);     //se recibe la informacion
        I2C_Master_Stop();              //se para la comunicacion
        __delay_ms(200);
        PORTA++;   
        
        //------llamado de funciones para desplegar valores en lcd
        lcd_linea(1,1);             //selecciono la linea 1 para escribir
        show(" S1   S2   S3 ");     //mensaje a enviar linea 1
        lcd_linea(2,1);             //selecciono la linea 2 para escibrir
        show(PORTA);                //mensaje a enviar linea 2
    }
    return;
}
/*-----------------------------------------------------------------------------
 ---------------------------------- SET UP -----------------------------------
 -----------------------------------------------------------------------------*/
void setup(void)
{
    ANSEL = 0;
    ANSELH = 0;
    TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    I2C_Master_Init(100000);        // Inicializar Comuncaci�n I2C
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/