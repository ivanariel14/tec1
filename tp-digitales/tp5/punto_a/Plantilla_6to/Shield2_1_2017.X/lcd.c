#include "lcd.h"


/* esta librer�a solo puede utilizar las cuatro l�neas menos significativas de 
 * un puerto en caso de querer utilizar otras l�neas se debe de reprogramar
 * es necesar�o este habilitada la interrupci�n de timer en la cual se debe de 
 * llamar a la funci�n LCD_tic() la misma establece las demoras que de otra
 * manera habr�a que implementarlas como una funci�n aparte 
 */
#define ENTRADA	0xF0	// Esto es para invertir el bus de datos y poder
#define SALIDA	0x0F    //leer cuando necesito ver si est� busy..
#define BUSY	PORTDbits.RD7	// Con estos defines me abstraigo del hardware
#define BUS_DIR TRISD

#define TRUE    0x1
#define FALSE   0x0

unsigned short LCD_tout;								// Contador para decrementar cada 1mS

void write_DATA(unsigned char dato);	// Pone se�ales para escribir dato
void write_CMD(unsigned char dato);	// Pone se�ales para escribir comando
void write_LCD (unsigned char dato);	// Pone byte en el bus 
void read_BUSY(void);		// Verifica si termin� la operaci�n en proceso

// Esta rutina recibe un puntero a char e imprime en el LCD ese string

void msg2LCD(const char *datos){

	while (*datos != 0){
		write_DATA(*datos);
		read_BUSY();
		datos++;
	}
}

// Esta rutina recibe un byte e imprime en el LCD
void char2LCD(unsigned char dato){

	write_DATA(dato);
	read_BUSY();
}

// Env�a el comando de borrar display (cmd 0x01)
void clear_LCD(void){
	write_CMD(0x01);
	read_BUSY();
}

// Lleva el cursor a la primera posici�n del primer rengl�n
void ret_HOME(void){
	write_CMD(0x02);
	read_BUSY();
}

// Pone el cursor donde quiero (0x00 a 0x0F el 1er rengl�n, 0x40 a 0x4F el 2do)
void set_CURSOR(unsigned char posi){
	posi |= 0x80;
	write_CMD(posi);
	read_BUSY();
}
void DesplazarIzquierda(void){
	write_CMD(24);
	read_BUSY();
}

// Inicializaci�n de display
void LCD_init(void){
    BUS_DIR &= SALIDA;	// Hace el bus salida para escribir en el LCD
    RS_BUS_DIR = 0;
    RW_BUS_DIR = 0;
    E_BUS_DIR = 0;
        

	LCD_tout = 30;          // Espera 30 interrup de timer
	while (LCD_tout);

	write_CMD(0x02);        // trabajar en 4bits 0010****
	LCD_tout = 1;		// Al menos 50uS 
	while (LCD_tout);	// Espera ac� hasta que LCD_tout se hace 0

	write_CMD(0x28);	// 4 bits 2 lineas caracter 5*8
	LCD_tout = 1;
	while (LCD_tout);

	write_CMD(0x0C);        //disp ON Cursor OFF Blink OFF
	LCD_tout = 1;
	while (LCD_tout);

	write_CMD(0x01);        // borra el display
	LCD_tout = 1;
	while (LCD_tout);

	write_CMD(0x06);        //Incrementar la pos auto SCROLL OFF
	LCD_tout = 1;
	while (LCD_tout);

	read_BUSY();												// Esta rutina lee el bit de busy a ver si el LCD se liber�..

}
// Pone las se�ales correspondientes para enviar un dato a mostrar
void write_DATA(unsigned char dato){

	RS = TRUE;
	RW = FALSE;
	write_LCD(dato);	// Env�a efectivamente el dato

}

// Pone las se�ales correspondientes para enviar un comando
void write_CMD(unsigned char dato){

	RS = FALSE;
	RW = FALSE;
	write_LCD(dato);	// Env�a efectivamente el comando

}

// Pone un byte en el bus de a un nibble por vez
void write_LCD (unsigned char dato){
		    
    DISPLAY &= 0x0F;
    E = TRUE;
    
    
    DISPLAY |= (dato & 0xF0);		// Pone el nibble alto en el bus
    
    E = FALSE;
    
    DISPLAY &= 0x0F;
    E = TRUE;
    
    
    DISPLAY |= (dato<<4);		// Pone el nibble alto en el bus
    
    E = FALSE;
        
}

// Lee el bit de busy (LCD termin� de procesar byte recibido..)
void read_BUSY(void){
	unsigned char aux;

	BUS_DIR |= ENTRADA;	// Pone el bus como entrada (hay que leer) D7 a D4
	RS = FALSE;         // Pone las se�ales para indicar que va a leer
	RW = TRUE;
	do{             // Hace
		E = TRUE;	// E=1
		aux = BUSY;	// Lee bit de busy
		E = FALSE;	// E=0
		E = TRUE;	// E=1
		(void)BUSY;	// Lectura dummy para completar el byte
		E = FALSE;	// E=0, completo byte
	}while (aux);		// Mientras busy = 1, cuando busy = 0 (LCD termin�), sale de este do-while
	RW = FALSE;		// Normaliza las se�ales
	BUS_DIR &= SALIDA;	// Hace el bus salida para seguir escribiendo al LCD
}

// S�lo decrementa una variable si esta est� cargada con alg�n valor
void tic_LCD(void){
	 if (LCD_tout) LCD_tout--;
     
}