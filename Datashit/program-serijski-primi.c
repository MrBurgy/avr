//#define USE_LCD
#define WITH_SENSORS

#define UART_BUFFER_SIZE	80

#define F_CPU 8000000UL
#define __AVR_ATmega328P__
#define UART_BAUD_RATE	9600      
#include <util/delay.h>
#if defined (USE_LCD)
	#include "glcd.c"
#endif
#include <stdlib.h> 	// itoa()
//#include "uart.h"
#include "uart.c"
#include <string.h> 	// strlen()	strncmp()
//#include <ctype.h>	// isprint
//#include <stdbool.h> 	// bool

#if defined (WITH_SENSORS)
#include "stepper.c"
#include "adc.c"
#include "ambient_light.c"
#include "motion_sensor.c"
#endif
//#include <avr/interrupt.h>

// jebote presveto srce Marijino, sa -Os zadane ispise prvo kao 0 pa kasnije kao smece
// sa -O0 zadane ispise kao prazne

char* serijski_primi();
void parsiraj(char *arg);
void obrisi_polje(char *arg);

// XXX zlo i naopako, globalne varijable:
int   nzad_temperatura=0;
int   nzad_foto1=0;
int   nzad_foto2=0;
int   nzad_svjetlost=0;
int   nzad_grijac=0;
int   nzad_sati=0;
int   nzad_minute=0;
	int temp;
	int foto1;
	int foto2;
	int mousn_senzor;
	int ambient_light_var;
	int grijac=2;	// nedozvoljeno stanje
	int sati=0;
	int minute=0;

/*
void postavi_zadanu(char arg, char arg2)
{
	if(arg == "temp")
	{
		*szad_temperatura = *arg;
	}

}
*/

// "obrisi" ekran
void serijski_clr()
{
	uart_putc(27);
	uart_puts("[2J");
	uart_putc(27);
	uart_puts("[2H");
}

// ispisi integer na serijski
void uart_putn(int broj)
{
	char buffer[7];
	itoa(broj,buffer,10); 
	uart_puts(buffer);
	uart_puts("\n\r");
}

void uart_rn()
{
	uart_puts("\n\r");
}

void serijski_ispisi_zadano()
{
	uart_puts("Zadana temperatura: ");
	uart_putn(nzad_temperatura);
	uart_puts("Zadana foto1: ");
	uart_putn(nzad_foto1);
	uart_puts("Zadana foto2: ");
	uart_putn(nzad_foto2);
	uart_puts("Zadana svjetlost: ");
	uart_putn(nzad_svjetlost);
	uart_puts("Zadana grijac: ");
	uart_putn(nzad_grijac);
	uart_puts("Zadana sati: ");
	uart_putn(nzad_sati);
	uart_puts("Zadana minute: ");
	uart_putn(nzad_minute);
}

void postavi_sat()
{
	if(sati != nzad_sati)
	{
		sati = nzad_sati;
	}
	if(minute != nzad_minute)
	{
		minute = nzad_minute;

	}
}

char* serijski_primi()
{
	int temp = uart_getc();	// procitaj char sa serijskog

	char polje[UART_BUFFER_SIZE];
	volatile char *pPolje = polje;		// jebem ti AVR GCC i -Os	// mora bit volatile da uspije maknit 'y' sa kraja
	//char *pPolje = (char *)pP;		// pretvori ga nazad u char* da se izjegne dalje casting	// bit ce da ne radi

	//obrisi_polje((char *)pPolje);			// za svaki slucaj

	// format koji prima serijski xnekikuracy
	if (temp == 'x')				// pocetak niza	
	{
		do 
		{
			temp = uart_getc();		// procitaj novi char sa serijskog
			*pPolje++ = temp;		// spremi ga u polje
		}
		while (temp != 'y');			// sve dok ne dodjes do kraja niza

		*(--pPolje) = '\0';			// makni 'y' iz zadnjeg mjesta u polju

	}
	pPolje = polje;			// pokazi na pocetak polja
	return (char *)pPolje;
}

void parsiraj(char *arg)
{
	// sto nam je to serijski poslao, hmm

	// korisnik moze zadat paljenje grijaca, temperaturu na kojoj se grijac pali, alarm, razinu svjetlosti ispod/iznad koje se pali nesta
	// XXX prepravit u originalnom programu na donje stringove
	// stringovi sa kojima se usporedjuje argument dobiven preko serijskog:
	char *ztemp="zad_temp";
	char *zfot1="zad_fot1";
	char *zfot2="zad_fot2";
	char *zsvje="zad_svje";
	char *zgrij="zad_grij";
	char *zsati="zad_sati";
	char *zminu="zad_minu";
	// XXX ovo gore bi se moglo i ljepse rijesit, struct?

	if ((strncmp(arg, ztemp, 8)) == 0)
	{
		//uart_puts("parsiraj(): ");
		//uart_puts("String je zadana temperatura\n\r");
		char *narg = arg += 9;	// pomakni adresu na dio iza ':'
		nzad_temperatura = atoi(narg);	// pretvori u integer

		//uart_puts("parsiraj(): nzad_temperatura je: ");
		//uart_putn(nzad_temperatura);
	}
	else if ((strncmp(arg, zfot1, 8)) == 0)
	{
		//uart_puts("parsiraj(): ");
		//uart_puts("String je zadana foto1\n\r");
		char *narg = arg += 9;	// pomakni adresu na dio iza ':'
		nzad_foto1 = atoi(narg);	// pretvori u integer
	}
	else if ((strncmp(arg, zfot2, 8)) == 0)
	{
		//uart_puts("parsiraj(): ");
		//uart_puts("String je zadana foto1\n\r");
		char *narg = arg += 9;	// pomakni adresu na dio iza ':'
		nzad_foto2 = atoi(narg);	// pretvori u integer
	}
	else if ((strncmp(arg, zsvje, 8)) == 0)
	{
		//uart_puts("parsiraj(): ");
		//uart_puts("String je zadana svjetlost\n\r");
		char *narg = arg += 9;	// pomakni adresu na dio iza ':'
		nzad_svjetlost = atoi(narg);	// pretvori u integer
	}
	else if ((strncmp(arg, zgrij, 8)) == 0)
	{
		//uart_puts("parsiraj(): ");
		//uart_puts("String je zadani grijac\n\r");
		char *narg = arg += 9;	// pomakni adresu na dio iza ':'
		nzad_grijac = atoi(narg);	// pretvori u integer
	}
	else if ((strncmp(arg, zsati, 8)) == 0)
	{
		//uart_puts("parsiraj(): ");
		//uart_puts("String je zadani sati\n\r");
		char *narg = arg += 9;	// pomakni adresu na dio iza ':'
		nzad_sati = atoi(narg);	// pretvori u integer
	}
	else if ((strncmp(arg, zminu, 8)) == 0)
	{
		//uart_puts("parsiraj(): ");
		//uart_puts("String je zadane minute\n\r");
		char *narg = arg += 9;	// pomakni adresu na dio iza ':'
		nzad_minute = atoi(narg);	// pretvori u integer
	}
	else
	{
		uart_puts("parsiraj(): ");
		uart_puts("Neprepoznati string.\n\r");
		uart_puts("arg je: ");
		uart_puts(arg);
		uart_puts("\n\r");
	}
}

void obrisi_polje(char *arg)
{
	while (*arg)
		*arg++ = '\0';
}

void ocitaj_senzore()
{
	temp = adc_read(0);
	foto1 = adc_read(1);
	foto2 = adc_read(2);
	mousn_senzor = motion_get();
	ambient_light_var = ambient_light_status();
}

void serijski_ispisi_trenutno()
{
	uart_puts("___________________________________________");
	uart_rn();
	uart_puts("Trenutna temperatura: ");
	uart_putn(temp);
	uart_puts("Trenutna foto1: ");
	uart_putn(foto1);
	uart_puts("Trenutna foto2: ");
	uart_putn(foto2);
	uart_puts("Trenutni mousn senzor: ");
	uart_putn(mousn_senzor);
	uart_puts("Trenutno stanje grijaca: ");
	uart_putn(grijac);				// XXX grijac bi trebalo i ocitat negdje
	uart_puts("Trenutno ambientno osvjetljenje: ");
	uart_putn(ambient_light_var);
}

int main()
{
#if defined (USE_LCD)
	glcd_init();
	glcd_led_on();
#endif
	uart_init(UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU));  
	sei();		// treba

	uart_puts("___________________________________________\n\r");

	while(1)
	{
		//serijski_ispisi_zadano();

		/*
		*/
		char* var_ser_rx = serijski_primi();	// ovo sjebe ispis zadanih XXX	// bilo bi lijepo napravit preko interruptova
		if(*var_ser_rx)
		{
			uart_puts("main(): serijski je dobio: ");
			uart_puts(var_ser_rx);
			uart_puts("\n\r");
			parsiraj((char *)var_ser_rx);

		//	obrisi_polje((char *)var_ser_rx);	// ne radi bez ovoga	// ha, sad radi
		}

		serijski_clr();
		ocitaj_senzore();
		serijski_ispisi_zadano();
		serijski_ispisi_trenutno();
		_delay_ms(1000);
	}
	return 0;
}
