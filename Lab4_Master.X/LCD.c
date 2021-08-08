/* 
 * File: LCD.c  
 * Se utiliz� y se adaptaron las librer�as de Ligo George 
 * de la p�gina www.electrosome.com
 * Enlace: https://electrosome.com/lcd-pic-mplab-xc8/
 * Revision history: 
 */

/* 
 * File:   LCD.h
 * Author: Andy Bonilla (hice una Frankenstein)
 *
 * Created on 24 de julio de 2021, 11:32 PM
 */


//LCD Functions Developed by electroSome y modificadas por yo
/*-----------------------------------------------------------------------------
 ----------------------------LIBRERIAS-----------------------------------------
 -----------------------------------------------------------------------------*/
#include <xc.h>
#include "LCD.h"
#define _XTAL_FREQ 4000000
/*-----------------------------------------------------------------------------
-------------------------------- FUNCIONES -----------------------------------
-----------------------------------------------------------------------------*/

//--------funcion para limpiar
void lcd_clear(void)
{
    cmd(0);
    cmd(1);
}

//--------funcion para inicializar el lcd
void lcd_init()
{
    cmd(0x30);		// Configure the LCD in 8-bit mode, 2 line and 5x7 font
	cmd(0x0C);		// Display On and Cursor Off
	cmd(0x01);		// Clear display screen
	cmd(0x06);		// Increment cursor
	cmd(0x80);		// Set cursor position to 1st line, 1st column
	cmd(0x38);
	cmd(0x0c);
	cmd(0x06);
	cmd(0x80);
}

//--------funcion para configuracion de comandos
void cmd(unsigned char a)
{
	PORTB=a;
	rs=0;
	rw=0;
	en=1;
    __delay_ms(1);
	en=0;
}

//--------funcion para envio de datos
void dat(unsigned char b)
{
	PORTB=b;
	rs=1;
	rw=0;
	en=1;
	__delay_ms(1);
	en=0;
}

//--------funcion para envio de informacion con punteros
void show(unsigned char *s)
{
	while(*s) {
		dat(*s++);
        __delay_us(40);
	}
}

//--------funcion para seleccion de linea en lcd
void lcd_linea(char a, char b) {
    char temp, z;
    if (a == 1)
    {
        temp = 0x80 + b - 1;
        z = temp;       //como es de 8bits se usa toda la variable
        cmd(z);     //se manda valor al puerto
        
    } 
    else if (a == 2) 
    {
        temp = 0xC0 + b - 1;
        z = temp;     //como es de 8bits se usa toda la variables
        cmd(z);   //se manda valor al puerto
        
    }
}

//--------funcion para mover a la derecha
void lcd_mov_derecha(void) {
    cmd(0x1c);      //se agrupan los dos nibbles en un byte
}

//--------funcion para mover a la izquierda
void lcd_mov_izquierda(void) {
    cmd(0x18);      //se agrupan los dos nibbles en un byte
}



//CUELLAR

/*
void Lcd_Port(char a)
{
	PORTD = a;
}

void Lcd_Init()
{
    Lcd_Cmd(0x38);
    Lcd_Cmd(0x0c);
    Lcd_Cmd(0x06);
    Lcd_Cmd(0x80);

}

void Lcd_Cmd(char a)
{
	             // => RS = 0
	Lcd_Port(a);
    RS = 0;
	EN  = 1;             // => E = 1
    __delay_ms(5);
    EN  = 0;             // => E = 0
}

void Lcd_Clear(void)
{
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}

void Lcd_Set_Cursor(char a, char b)
{
	char temp,z,y;
	if(a == 1)
	{
	  temp = 0x80 + b - 1;
		z = temp;
		Lcd_Cmd(z);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		z = temp;
		Lcd_Cmd(z);
	}
}


void Lcd_Write_Char(char a)
{
   RS = 1;             // => RS = 1
   Lcd_Port(a);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   RS = 0;
}

void Lcd_Write_String(char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}

void Lcd_Shift_Right()
{
	Lcd_Cmd(0x1C);
}

void Lcd_Shift_Left()
{
	Lcd_Cmd(0x018);
}
*/

//STACK OVERFLOW
/*void Lcd_CmdWrite(char cmd)
{
    LcdDataBus = cmd;               //Send the Command nibble
    LcdControlBus &= ~(1<<LCD_RS);  // Send LOW pulse on RS pin for selecting Command register
    LcdControlBus &= ~(1<<LCD_RW);  // Send LOW pulse on RW pin for Write operation
    LcdControlBus |= (1<<LCD_EN);   // Generate a High-to-low pulse on EN pin
    __delay_us(100);
    LcdControlBus &= ~(1<<LCD_EN); 

    __delay_us(10000);
}

void Lcd_DataWrite(char dat)
{
    LcdDataBus = dat;               //Send the data on DataBus nibble
    LcdControlBus |= (1<<LCD_RS);   // Send HIGH pulse on RS pin for selecting data register
    LcdControlBus &= ~(1<<LCD_RW);  // Send LOW pulse on RW pin for Write operation
    LcdControlBus |= (1<<LCD_EN);   // Generate a High-to-low pulse on EN pin
    __delay_us(100);
    LcdControlBus &= ~(1<<LCD_EN);

    __delay_us(10000);
}*/