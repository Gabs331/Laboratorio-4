/*
 * Prelab 4.c
 *
 * Created: 31/03/2025 20:29:28
 * Author : pyocm
 */ 
//***************************************************************************
// Universidad del Valle de Guatemala
// IE2023: Programación de Microcontroladores
// Autor: Paola Gabriela Yoc Moreira
// Hardware: ATMEGA328P
// Created: 01/04/2025
//***************************************************************************
// PreLab 4
//***************************************************************************
#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char Boton1X = 1;
unsigned char Boton11= 1;
unsigned char Boton2X = 2;
unsigned char Boton2 = 1;

int cont = 0;
int main(void)
{
	// salidas
	DDRD = 0xFF;
	PORTD = 0;
	// PINES RX TX
	UCSR0B = 0;
	// entradas
	DDRB &= ~(1<<0);
	PORTB |= (1<<PB0);
	DDRB &= ~(1<<1);
	PORTB |= (1<<PB1);
	// INTERRUPCION PARA PUERTO B
	PCICR |= (1<<PCIE0);
	// HABILITAR PARA PCINT0
	PCMSK0 |= ((1<<PCINT0)|(1<<PCINT1));
	sei();
	while (1)
	{
		if (cont > 255){
			cont = 0;
		}
		if (cont < 0){
			cont = 255;
		}
		PORTD = cont;
	}
}

ISR(PCINT0_vect){
	_delay_ms(50);
	Boton11 = PINB&(1<<PINB0);
	Boton2 = PINB&(1<<PINB1);
	if ((Boton11 == 0)&&(Boton1X == 1)){
		Boton1X = 0;
		cont ++;
	}
	if (Boton11 == 1){
		Boton1X = 1;
	}
	if ((Boton2 == 0)&&(Boton2X == 2)){
		Boton2X = 0;
		cont --;
	}
	if (Boton2 == 2){
		Boton2X = 2;
	}
}

