/*------------------------------------------------------------------------------
Archivo: mainsproject.s
Microcontrolador: PIC16F887
Autor: Andy Bonilla
Compilador: pic-as (v2.30), MPLABX v5.45
    
Programa: laboratorio 1
Hardware: PIC16F887
    
Creado: 16 de julio de 2021    
Descripcion: 
------------------------------------------------------------------------------*/

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT   //configuracion de oscilador interno
//#pragma config FOSC = EXTRC_NOCLKOUT// Oscillator Selection bits (RCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, RC on RA7/OSC1/CLKIN)
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
#include "LCD.h"
#include <xc.h>

/*-----------------------------------------------------------------------------
 ----------------------- VARIABLES A IMPLEMTENTAR------------------------------
 -----------------------------------------------------------------------------*/

//-------DIRECTIVAS DEL COMPILADOR
#define _XTAL_FREQ 8000000
//-------VARIABLES DE PROGRAMA
unsigned char desde_contador;       //variable que recibe contador
unsigned char desde_pot;            //variable que recibe potenciometro
unsigned char desde_sensor;         //variable que recibe sensor
uint8_t NUM;                        //variable arbitraria
char Cen3 = 0;                      //centenas de temperatura
char Dec3 = 0;                      //decenas de tempertarua
char Un3 = 0;                       //unidades de temperatura
char AC3 = 0;                       
char AD3 = 0;
char AU3 = 0;
/*-----------------------------------------------------------------------------
 ------------------------ PROTOTIPOS DE FUNCIONES ------------------------------
 -----------------------------------------------------------------------------*/
void setup(void);
unsigned char datos_ascii(uint8_t numero);
void temperatura(void);
uint8_t lcd_ascii();
/*-----------------------------------------------------------------------------
 ----------------------------- MAIN LOOP --------------------------------------
 -----------------------------------------------------------------------------*/
void main(void) {
    setup();
    lcd_clear();
    lcd_init();         //invoco la funcion de inicializacion de la lcd
	cmd(0x90);          //invocao la funcion de configurcion de comandos lc
    __delay_ms(1);
    while(1)
    {
        temperatura();
        //-----SENCUENCIA INICIAL PARA MANDAR
        I2C_Master_Start();
        I2C_Master_Write(0x50);
        I2C_Master_Write(0);
        I2C_Master_Stop();
        __delay_ms(200);
        //-----RECEPCION DE CONTADOR
        I2C_Master_Start();
        I2C_Master_Write(0x51);
        desde_contador = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        //-----RECEPCION DE POTENCIOMETRO
        I2C_Master_Start();
        I2C_Master_Write(0x61);
        desde_pot =I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        //------COMUNICACION INICIAL CON SENSOR
        I2C_Master_Start();     //Escribir
        I2C_Master_Write(0x80);
        I2C_Master_Write(0xF3);
        I2C_Master_Stop();
        __delay_ms(100);
        //-----RECEPCION DE SENSOR
        I2C_Master_Start();
        I2C_Master_Write(0x81);
        desde_sensor = I2C_Master_Read(0);
        I2C_Master_Stop();
        __delay_ms(200);
        
        //------DESPLEGAR VALORES EN LCD
        lcd_linea(1,1);             //selecciono la linea 1 para escribir
        show("Count Pot  Temp");     //mensaje a enviar linea 1
        lcd_linea(2,1);             //selecciono la linea 2 para escibrir
        show(lcd_ascii());          //mensaje a enviar linea 2
    }
    return;
}
/*-----------------------------------------------------------------------------
 ---------------------------------- SET UP -----------------------------------
 -----------------------------------------------------------------------------*/
void setup(void){
    //-------CONFIGURACION ENTRADAS ANALOGICAS
    ANSEL = 0;
    ANSELH = 0;
    //-------CONFIGURACION IN/OUT
    TRISA = 0;
    TRISB = 0;
    TRISD = 0;
    TRISE = 0;
    //-------LIMPIEZA DE PUERTOS
    PORTA = 0;
    PORTB = 0;
    PORTD = 0;
    PORTE = 0;
    //-------CONFIGURACION DE RELOJ A 8MHz
    OSCCONbits.IRCF=0b111;      //Freq a 8MHz
    OSCCONbits.SCS=1;           //Oscilador interno
    //-------LIMPIEZA DE PUERTOS
    I2C_Master_Init(100000);        // Inicializar Comuncaci�n I2C
    
}
/*-----------------------------------------------------------------------------
 --------------------------------- FUNCIONES ----------------------------------
 -----------------------------------------------------------------------------*/
//------FUNCION PARA CONVERTIR A ASCII
unsigned char datos_ascii(uint8_t numero) 
{
    switch(numero)
    {
        default:
            return 0x30;        //retorna 0 en ascii
            break;
        case(0):
            return 0x30;        //retorna 0 en ascii
            break;
            
        case(1):
            return 0x31;        //retorna 1 en ascii
            break;
            
        case(2):
            return 0x32;        //retorna 2 en ascii
            break;
            
        case(3):
            return 0x33;        //retorna 3 en ascii
            break;
            
        case(4):
            return 0x34;        //retorna 4 en ascii
            break;
            
        case(5):
            return 0x35;        //retorna 5 en ascii
            break;
            
        case(6):
            return 0x36;        //retorna 6 en ascii
            break;
            
        case(7):
            return 0x37;        //retorna 7 en ascii
            break;
            
        case(8):
            return 0x38;        //retorna 8 en ascii
            break;
            
        case(9):
            return 0x39;        //retorna 9 en ascii
            break;       
    }   
}
//------FUNCION PARA DESPLEGAR EN LCD LOS VALORES ASCII
uint8_t lcd_ascii()
{
    uint8_t random[16];                    
    random[0]=32;                                   //espacio
    random[1]=32;                                   //espacio
    random[2]=datos_ascii((desde_contador/10)%10);  //decenas contador
    random[3]=datos_ascii(desde_contador%10);       //unidades contador
    random[4]=32;                                   //espacio 
    random[5]=datos_ascii(((2*desde_pot)/100)%10);  //unidades pot    
    random[6]=0x2E;                                 //punto decimal   
    random[7]=datos_ascii(((2*desde_pot)/10)%10);   //decemas pot
    random[8]=datos_ascii(desde_pot%10);            //centecimas pot
    random[9]=86;
    random[10]=32;                                 //voltaje
    random[11]=AC3;      
    random[12]=AD3;    
    random[13]=AU3;      
    random[14]=0xDF;                      //se deja espacio
    random[15]=67;                      //se deja espacio
    return random;                      //se retorna el valor para el lcd
}
//datos_ascii((desde_sensor/10)%10);
void temperatura(void){
    if (desde_sensor>=68){      //Conversión de números positivos para temperatura
        NUM = 24*desde_sensor/35-1632/35;
        Cen3 = NUM/100;
        Dec3 = (NUM-Cen3*100)/10;
        Un3 = NUM-Cen3*100-Dec3*10;
        AC3 = datos_ascii(Cen3);
        AD3 = datos_ascii(Dec3);
        AU3 = datos_ascii(Un3);
    }
    else{
        NUM = (877/19)-13*desde_sensor/19;      //Conversión de números negativos para temperatura
        AC3 = 0x2D;
        Dec3 = NUM/10;
        Un3 = NUM - Dec3*10;
        AD3 = datos_ascii(Dec3);
        AU3 = datos_ascii(Un3);
    }
}