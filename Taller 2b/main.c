/******************************************************************************
* Taller 2 Sistemas Embebidos
* Departamento Ingenieria Biomedica, Universidad de Valparaiso
*
* Autor: Sergio Jeria
* Licencia: New BSD
* Descripcion:
* Comunicacion Serial para Parpadeo de un LED en Arduino Duemilanove
*
******************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <inttypes.h>
#include <util/delay.h>
#include "serial.h"
#include "main.h"

#define led_on() LED_PORT |= (1 << LED_PIN)
#define led_off() LED_PORT &= ~(1 << LED_PIN)

/* Prototipo de funciones */
void timer0_init(void);
void write_buffer(char *buffer, uint8_t n);

/* Variables globales */
volatile uint8_t Timer0_Flag;

int main(void)
{
  int16_t data_in;
  int16_t datap;
  int j=0;
  int dat[3];
  int datint=0;
  int a=100;
  int t[3]={30,30,30};
  
  serial_init();
  while(1) {
	data_in = serial_read();
	/*Se registran los valores ingresados por el usuario mientras estos
		sean numeros (entre 0 =0x30 y 9=0x39) y se registran en el arreglo
		"data", si en usuario utiliza caracteres estos son descartados*/
	if(data_in >= 0x30 && data_in <= 0x39){
		dat[j]=data_in - 0x30;
		dat[j]=(int)dat[j];
		serial_write(data_in);
		serial_write('-');
		serial_write(dat[j]);
		serial_write('-');
		datint=datint+(dat[j]*a);
		serial_write(datint);
		serial_write('-');
		j++;
		a=a/10;
		if(j==3){
			char texto2[14]={'P','r','e','c','i','o','n','e',' ','E','n','t','e','r'};
			for(int a=0;a<=14;a++){
				serial_write(texto2[a]);
			}
			serial_write('\r');
			serial_write('\n');
		}
	}
	/*Responde a la pulsacion del boton Enter y registra los valores
		ingresados en la variable "dataint" para usarla como tiempo
		de parpadeo del LED*/
	if(data_in == 0x0d) {
		a=100;
		j=0;
		datap=serial_read(); 
		uint16_t i;
		int time =0;
	  /* Mientras que "dataint" posea algun valor, este se utiliza para
		regular la duracion del parapadeo del LED*/
	  while(datint != 0){
		led_on();
			for(i=0; i<datint; i++) {
				_delay_ms(1);
			}

		led_off();
			for(i=0; i<datint; i++) {
				_delay_ms(1);
			}
			
/* Si se detecta un ingreso de datos en la terminal se detiene
	el parpadeo del LED*/
		data_in=serial_read();
		if(data_in != datap){
			datint=0;
		}
	}
}
}
  return 0;
}
