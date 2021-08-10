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
#pragma config FOSC = INTRC_NOCLKOUT   //configuracion de oscilador interno
#pragma config WDTE = OFF       // Watchdog Timer Enable bit
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // master clear off 
#pragma config CP = OFF         // Code Protection bit off
#pragma config CPD = OFF        // Data Code Protection bit off
#pragma config BOREN = OFF      // Brown Out Reset Selection bits off
#pragma config IESO = OFF       // Internal External Switchover bit off
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit off 
#pragma config LVP = OFF        // Low Voltage Programming Enable bit off 

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit 
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits 

/*-----------------------------------------------------------------------------
 ----------------------------LIBRERIAS-----------------------------------------
 -----------------------------------------------------------------------------*/
#include <stdint.h>
#include <pic16f887.h>
//#include <PIC16F887.h>          //se incluye libreria del pic
#include "Osc_config.h"
#include "LCD.h"
#include "I2C.h"
#include <xc.h>
/*-----------------------------------------------------------------------------
 ------------------------DIRECTIVAS DE COMPILADOR------------------------------
 -----------------------------------------------------------------------------*/
#define _XTAL_FREQ 8000000

/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);           //prototipo de funcion de inicializacion pic

/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/

/*-----------------------------------------------------------------------------
 ---------------------------- INTERRUPCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
/*void __interrupt() isr(void) //funcion de interrupciones
{
    //------INTERRUPCION DEL TIMER1
    
}*/

/*-----------------------------------------------------------------------------
 ----------------------------- MAIN LOOP --------------------------------------
 -----------------------------------------------------------------------------*/
void main(void)
{
    setup();            //se llama funcion de configuracion
    //---------CONFIGURACION DE LCD
    lcd_clear();        //invoco la funcion de limpieza de lcd
    lcd_init();         //invoco la funcion de inicializacion de la lcd
	cmd(0x90);          //invocao la funcion de configurcion de comandos lc
    __delay_ms(1);
    //---------CONFIGURACION DE 12C
    
    while(1)
    {
        //---------COMUNICACION I2C
        I2C_Master_Start();
        I2C_Master_Write(0x50);
        I2C_Master_Write(PORTA);
        I2C_Master_Stop();
        __delay_ms(200);
       
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        PORTE = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        PORTA++;   
        
        //---------MANDAR DATOS A LCD
        lcd_linea(1,1);             //selecciono la linea 1 para escribir
        show(PORTA);     //mensaje a enviar linea 1
        lcd_linea(2,1);             //selecciono la linea 2 para escibrir
        show(PORTD);          //mensaje a enviar linea 2
        
    }

}
/*-----------------------------------------------------------------------------
 ---------------------------------- SET UP -----------------------------------
 -----------------------------------------------------------------------------*/
void setup(void)
{
    //---------CONFIGURACION DE ENTRADAS ANALOGICAS
    ANSEL=0;                //solo se limpian entradas analogicas
    ANSELH=0;               //solo se limpian entradas analogicas
    ANSELbits.ANS0=1;       //entrada para el potenciometro 1
    ANSELbits.ANS1=1;       //entrada para el potenciometro 2
    
    //---------CONFIGURACION DE IN/OUT
    TRISA=0;                //todo el portA como salida
    TRISB=0;                //todo el portB como salida
    TRISD=0;                //todo el portB como salida
    TRISDbits.TRISD5=0;     //salida para pines lcd
    TRISDbits.TRISD6=0;     //salida para pines lcd
    TRISDbits.TRISD7=0;     //salida para pines lcd
    
    
    //---------LIMPIEZA DE PUERTOS
    PORTA=0;                //se limpia puerto
    PORTB=0;                //se limpia puerto
    PORTD=0;                //se limpia puerto
    
    //---------IMPORTACION DE FUNCIONES DE LIBRERIAS
    osc_config(8);                  //se llama funcion de oscilador a 4MHz
    I2C_Master_Init(100000);        // Inicializar Comuncaci�n I2C
    
    //---------CONFIGURACIOND DE INTERRUPCIONES
    INTCONbits.GIE=0;           //se habilita interrupciones globales
    INTCONbits.PEIE=0;          //interrupcion por perifericos
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/

