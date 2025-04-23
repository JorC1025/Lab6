/*
 * Lab 6.c
 *
 * Created: 22/4/2025 21:36:30
 * Author : jorge
 */ 


#include <avr/io.h>

#include <avr/interrupt.h>
//Mandar cómo mandar y recibir caracteres
//Reconoce

int main(void)
{
	setup();
	writeChar('H');
	/* Replace with your application code */
	while (1)
	{
	}
}

void setup(){
	cli();
	//Con la calculadora se decide el Baud Rate, ahorita se puede elegir cualquiera, ahorita elegimos 9600 (ver en hoja de datos)
	//Se asume 9600 por ahora
	//Modo asíncrono
	//En Double Speed sale bien, entonces se configura para double speed, esto es con prescaler de 1
	//Sin prescaler de 1 double speed y el half speed funcionan bien con 9615
	//Estamos usando prescaler de 1 ahorita
	//UBRR en 103
	
	initUAT();
		DDRB = 0xFF;
		PORTB = 0x00;
	
	
	sei();
	
	
}

void initUAT(){
	//Se usa PD0 y PD1
	//Mirar el pinout
	DDRD |= (1<<DDD1);
	DDRD &=~ (1<<DDD0);
	
	//Configurar UCS0A
	UCSR0A = 0;
	
	UCSR0B = (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0);
	
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);
	
	//Se configura baud rate
	UBRR0 = 103; //9615 @ 16MHz
	
	
	
}

void writeChar(char caracter){

	
	while ((UCSR0A & (1<<UDRE0))==0);
	UDR0 = caracter;
	
}

ISR (USART_RX_vect){
	char codigosalida = UDR0;

	PORTB = codigosalida;
	
}

void writeString(char* texto){
	
	for (uint8_t i =0; *(texto+i)!='\0'; i++)
	{
		writeChar(*(texto+i));
	}
	
}

