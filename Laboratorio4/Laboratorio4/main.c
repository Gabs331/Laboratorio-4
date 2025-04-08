/*
 * Laboratorio4.c
 *
 * Created: 1/04/2025 14:11:51
 * Author : pyocm
 */ 
//***************************************************************************
// Universidad del Valle de Guatemala
// IE2023: Programación de Microcontroladores
// Autor: Paola Gabriela Yoc Moreira
// Hardware: ATMEGA328P
// Created: 01/04/2025
//***************************************************************************
// Laboratorio 4
//***************************************************************************
#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
void initADC(void);
void dispsegG(int);
void dec_hex(uint8_t);
void alarma(void);

unsigned char Boton1X = 1;
unsigned char Boton1= 1;
unsigned char Boton2X = 2;
unsigned char Boton2 = 1;

unsigned char hexaDisplay[16]={0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
const unsigned int tMulx = 10;

int contadorADC = 0;
int LowNibble = 0;
int HighNibble = 0;
int cont = 0;

int main(void)
{
	cli();
	initADC();
	// salidas
	DDRD = 0xFF;
	PORTD = 0;
	DDRC = 0xFF;
	PORTC = 0;
	DDRB |= (1<<PORTB5)|(1<<PORTB4)|(1<<PORTB3)|(1<<PORTB2);
	// PINES RX TX
	UCSR0B = 0;
	// entradas
	DDRB &= ~(1<<0);
	PORTB |= (1<<PORTB0);
	DDRB &= ~(1<<1);
	PORTB |= (1<<PORTB1);
	// INTERRUPCION PARA PUERTO B
	PCICR |= (1<<PCIE0);
	// HABILITAR PARA PCINT0
	PCMSK0 |= (1<<PCINT0)|(1<<PCINT1);
	sei();
	
	while (1)
	{
		alarma() ;
		// RESTRICCIÓN PARA CONTADOR
		if (cont > 255){
			cont = 0;
		}
		if (cont < 0){
			cont = 255;
		}
		PORTD = cont;
		
		// INICIANDO SECUENCIA DE ADC
		ADCSRA |= (1<<ADSC);
		
		// MOSTRAR PRIMER DISPLAY
		dec_hex(contadorADC);
		PORTC = hexaDisplay[LowNibble];
		dispsegG(LowNibble);
		PORTB |= (1<<PORTB3);
		PORTB &= ~(1<<PORTB2);
		_delay_ms(tMulx);
		
		
		// MOSTRAR SEGUNDO DISPLAY
		dec_hex(contadorADC);
		PORTC = hexaDisplay[HighNibble];
		dispsegG(HighNibble);
		PORTB |= (1<<PORTB2);
		PORTB &= ~(1<<PORTB3);
		_delay_ms(tMulx);
		
	}
}



//***************************************************************************
// FUNCIONES
//***************************************************************************
void dec_hex(uint8_t val){
	HighNibble = val/16;	// PRIMER NIBBLE
	LowNibble = val%16;		// RESIDUO
}
void alarma(void){
	if (contadorADC > cont){
		PORTB |= (1<<PORTB4);
	} else {
		PORTB &= ~(1<<PORTB4);
	}
}

void initADC(void){
	ADMUX |= 0x07;
	// Vref = AVCC
	ADMUX |= (1<<REFS0);
	ADMUX &= ~(1<<REFS1);
	// justificado a la izquierda
	ADMUX |= (1<<ADLAR);
	ADCSRA = 0;
	// encender ADC
	ADCSRA |= (1<<ADEN);
	// HABILITAR ISR ADC
	ADCSRA |= (1<<ADIE);
	// PRESCALER 128 > 16M/128 = 125KHz
	ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
	// DESHABILITAR ENTRADA DIGITAL PC0
	DIDR0 |= (1<<ADC0D);
}
void dispsegG(int segmentG){
	// ENCENDER O APAGAR SEGMENTO G
	switch(segmentG){
		case 0:
			PORTB &= ~(1<<PORTB5);
		break;
		case 1:
			PORTB &= ~(1<<PORTB5);
		break;
		case 2:
			PORTB |= (1<<PORTB5);
		break;
		case 3:
			PORTB |= (1<<PORTB5);
		break;
		case 4:
			PORTB |= (1<<PORTB5);
		break;
		case 5:
			PORTB |= (1<<PB5);
		break;
		case 6:
			PORTB |= (1<<PB5);
		break;
		case 7:
			PORTB &= ~(1<<PB5);
		break;
		case 8:
			PORTB |= (1<<PB5);
		break;
		case 9:
			PORTB |= (1<<PB5);
		break;
		case 10:
			PORTB |= (1<<PB5);
		break;
		case 11:
			PORTB |= (1<<PB5);
		break;
		case 12:
			PORTB &= ~(1<<PB5);
		break;
		case 13:
			PORTB |= (1<<PB5);
		break;
		case 14:
			PORTB |= (1<<PB5);
		break;
		case 15:
			PORTB |= (1<<PB5);
		break;
	}
}


ISR(ADC_vect){
	contadorADC = ADCH;
	ADCSRA |= (1<<ADIF);
}
ISR(PCINT0_vect){
	_delay_ms(50);
	Boton1 = PINB&(1<<PINB0);
	Boton2 = PINB&(1<<PINB1);
	if ((Boton1 == 0)&&(Boton1X == 1)){
		Boton1X = 0;
		cont ++;
	}
	if (Boton1 == 1){
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