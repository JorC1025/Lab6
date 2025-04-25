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

volatile char modo = '0';
uint8_t valor_adc = 0;

int main(void)
{
	setup();
	mostarMenu();



	while (1)
	{
		if (modo == '1')
		{
			ADCSRA |= (1<<ADSC);
		}
	}
}

void setup(){
	cli();

	//UBRR en 103
	DDRC &= ~(1<<DDC4);
	DDRC |= (1 << DDC1);
	DDRC |= (1 << DDC0);
	initUAT();
	initADC();
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
	char dato_recibido = UDR0;

	if (modo == '0')
	{
		if (dato_recibido == '1')
		{
			modo = '1';
			writeString("Potenciometro Seleccionado \n");
		}
		else if (dato_recibido == '2')
		{
			modo = '2';
			writeString("ASCII seleccionado \n");
		}
		
		else { //Programacion defensiva, y reinicio de pregunta despues de mostrar el error
		writeString("Opcion no valida \n");
		mostarMenu();
		}		
				
	}
	
		else if (modo == '2') //Mostar ASCII y reiniciar pregunta
		{
			PORTB = dato_recibido;
			PORTC = (dato_recibido >> 6);
			
			modo = '0';
			mostarMenu();
		}
		


	
	
}

void writeString(char* texto){ //Funcion de escribir string
	
	for (uint8_t i =0; *(texto+i)!='\0'; i++)
	{
		writeChar(*(texto+i));
	}
	
}

void initADC(){ //Setup de canales adc
	
	
	ADMUX = 0;
	ADMUX = (1 << REFS0) | (1 << ADLAR) | (1 << MUX2);

	
	ADCSRA = 0;
	ADCSRA |= (1 << ADPS1) |(1<<ADPS0) | (1<< ADEN) | (1<<ADIE);
	
	
}
ISR (ADC_vect){
	
	if (modo =='1') //Comparar si se esta en el modo del adc
	{
		valor_adc = ADCH;
		char buffer[10];
		
		itoa(valor_adc, buffer, 10);
		writeString("Valor ADC: ");
		writeString(buffer);
		writeString("\n");
		
		modo ='0';
		mostarMenu();
	}
	
	
}

void mostarMenu(){ //Funcion que permite mostrar el menu al usuario al reninciar la pregunta
	
				writeString("Que desea hacer?\n");
				
				writeString("Leer Potenciometro: \n");
				writeString("Enviar ASCII \n");
				
	
	
}