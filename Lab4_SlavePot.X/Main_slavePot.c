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
#include <xc.h>                 //se incluye libreria del compilador
#include <stdint.h>             //se incluye libreria
#include <pic16f887.h>          //se incluye libreria del pic
#include "Osc_config.h"

/*-----------------------------------------------------------------------------
 ------------------------DIRECTIVAS DE COMPILADOR------------------------------
 -----------------------------------------------------------------------------*/
#define _XTAL_FREQ 4000000

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
void __interrupt() isr(void) //funcion de interrupciones
{
    //------INTERRUPCION DEL TIMER1
    
}

/*-----------------------------------------------------------------------------
 ----------------------------- MAIN LOOP --------------------------------------
 -----------------------------------------------------------------------------*/
void main(void)
{
    setup();        //se llama funcion de configuracion
    while(1)
    {
        
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
    
    //---------IMPORTACION DE FUNCIONES DE LIBRERIAS
    osc_config(4);          //se llama funcion de oscilador a 4MHz
    
    //---------CONFIGURACIOND DE INTERRUPCIONES
    INTCONbits.GIE=0;           //se habilita interrupciones globales
    INTCONbits.PEIE=0;          //interrupcion por perifericos
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/


