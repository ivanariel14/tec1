#include <xc.h> 

#ifndef LCD_H
#define LCD_H

				// Si en otro proyecto tengo pines distintos cambio aqu� solamente...
#define RS			PORTEbits.RE2
#define RS_BUS_DIR  TRISEbits.RE2
#define RW			PORTEbits.RE1
#define RW_BUS_DIR  TRISEbits.RE1
#define	E			PORTEbits.RE0
#define E_BUS_DIR   TRISEbits.RE0

#define DISPLAY         LATD	// Es para denominar al puerto donde est� 
                                // Conectado el bus

void LCD_init(void);                // Inicializa el LCD, requiere timer (interrupci�n)
void msg2LCD(const unsigned char*);       // Env�a un mensaje para mostrar en LCD

/**
 * Envia dato como unico caracter ASCII al LCD
 * @param dato caracter a enviar como ASCII HEXA o INT
 */
void char2LCD(unsigned char dato);    
void clear_LCD(void);				// Borra la pantalla del LCD
void ret_HOME(void);				// Ubica el cursor en la primera posici�n
void set_CURSOR(unsigned char);     // Ubica el cursor en una posici�n determinada
void tic_LCD(void);                 // Esta rutina se llama desde la interrupci�n de timer de 1mS
void DesplazarIzquierda(void);

#endif /* LCD_H */
