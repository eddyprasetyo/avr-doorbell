#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <stdint.h>
#include <stdlib.h>
#include "uart-routines.c"

uint8_t lagubaf[900];
uint8_t EEMEM tempo = 12;
uint8_t EEMEM lagu[]=
{
	16,117,	16,116,	16,117,	16,116,	16,117,	16,112,	16,113,	16,110,
	16,100, 16,101, 16,105, 16,110,	8,112,	16,100, 16,105, 16,109,
	16,112, 8,113,	16,100, 16,105, 16,117, 16,116, 16,117, 16,116,
	16,117, 16,112, 16,115, 16,113, 16,100, 16,101, 16,105, 16,110,
	8,112,	16,100, 16,105, 16,113, 16,112, 4,110,	8,110,	16,100,
	16,112, 16,113, 16,115, 8,117,	16,108, 16,118, 16,117, 8,115,
	16,106, 16,117, 16,115, 8,113,	16,105, 16,115, 16,113, 8,112,
	16,100, 16,105, 16,117, 8,100,	16,117, 16,129, 8,100,	16,116,
	16,117, 8,100,	16,116, 16,117, 16,115, 16,117, 16,116, 255
};

void timer_init (void)
{
	DDRB |= (1<<PB1) | (1<<PB5); //set output pin
	TCCR1A |= (1<<COM1A0); //toogle OC1A
	TCCR1B |= (1<<WGM12); //set ctc mode
	//OCR1A = 10568;
}

void tombol_init (void)
{
	EICRA |= (1<<ISC01);
	EIMSK |= (1<<INT0);
}

uint16_t nadatoocr (uint8_t nada)
{
	uint16_t ocr;
	if (nada==101) {ocr=21136;return ocr;}
	else if (nada==102) {ocr=19950;return ocr;}
	else if (nada==103) {ocr=18830;return ocr;}
	else if (nada==104) {ocr=17773;return ocr;}
	else if (nada==105) {ocr=16675;return ocr;}
	else if (nada==106) {ocr=15834;return ocr;}
	else if (nada==107) {ocr=14945;return ocr;}
	else if (nada==108) {ocr=14106;return ocr;}
	else if (nada==109) {ocr=13315;return ocr;}
	else if (nada==110) {ocr=12567;return ocr;}
	else if (nada==111) {ocr=11862;return ocr;}
	else if (nada==112) {ocr=11196;return ocr;}
	else if (nada==113) {ocr=10568;return ocr;}
	else if (nada==114) {ocr=9975;return ocr;}
	else if (nada==115) {ocr=9415;return ocr;}
	else if (nada==116) {ocr=8887;return ocr;}
	else if (nada==117) {ocr=8388;return ocr;}
	else if (nada==118) {ocr=7917;return ocr;}
	else if (nada==119) {ocr=7473;return ocr;}
	else if (nada==120) {ocr=7053;return ocr;}
	else if (nada==121) {ocr=6657;return ocr;}
	else if (nada==122) {ocr=6284;return ocr;}
	else if (nada==123) {ocr=5931;return ocr;}
	else if (nada==124) {ocr=5598;return ocr;}
	else if (nada==125) {ocr=5284;return ocr;}
	else if (nada==126) {ocr=4987;return ocr;}
	else if (nada==127) {ocr=4707;return ocr;}
	else if (nada==128) {ocr=4443;return ocr;}
	else if (nada==129) {ocr=4194;return ocr;}
	else if (nada==130) {ocr=3959;return ocr;}
	else if (nada==131) {ocr=3736;return ocr;}
	else if (nada==132) {ocr=3527;return ocr;}
	else if (nada==133) {ocr=3329;return ocr;}
	else if (nada==134) {ocr=3142;return ocr;}
	else if (nada==135) {ocr=2966;return ocr;}
	else if (nada==136) {ocr=2799;return ocr;}
	else ocr=0;
	return ocr;
}

void play (uint8_t *data)
{
	uint8_t baf[2];
	baf[0] = eeprom_read_byte(data++);
	baf[1] = eeprom_read_byte(data++);
	
	while(baf[0] != 255)
	{
		if(baf[1] != 100)
		{
			OCR1A = nadatoocr(baf[1]);
			TCCR1B |= (1<<CS10);
			PORTB |= (1<<PB5);
			_delay_ms(240000/(eeprom_read_byte(&tempo)*5*baf[0]));
			TCCR1B &= ~(1<<CS10);
			PORTB &= ~(1<<PB5);
		}
		else _delay_ms(240000/(eeprom_read_byte(&tempo)*5*baf[0]));
		
		baf[0] = eeprom_read_byte(data++);
		baf[1] = eeprom_read_byte(data++);
	}
	
}

ISR(INT0_vect)
{
	//sei();
	play(lagu);
}

ISR(USART_RX_vect)
{
	cli();
	uint8_t ch;
	uint8_t ch2;
	uint8_t i;
	uint16_t a;
	uint8_t nad=0;
	uint8_t dur=0;
	uint8_t temptempo;
	uint8_t baf[5]={0,0,0,0,0};
	uint8_t nadbaf[3]={0,0,0};
	ch = UDR0;
	
	if (ch == 'h')
	{
		putch(12);
		putstrf(PSTR("***####$$$$   SELAMAT DATANG   $$$$###***"));
		newline();
		putstrf(PSTR("Programable Door Bell berbasis ATMega 328p"));
		newline();
		putstrf(PSTR("Tempo="));
		temptempo=eeprom_read_byte(&tempo);
		putvar(temptempo*5, 4,10);
		putstrf(PSTR(" bpm"));
		newline();newline();newline();newline();newline();
		putstrf(PSTR("daftar perintah :"));
		newline();
		putstrf(PSTR("d - decrement tempo by 5 bpm"));newline();
		putstrf(PSTR("h - this help menu"));newline();
		putstrf(PSTR("i - increment tempo by 5 bpm"));newline();
		putstrf(PSTR("u - upload Tone, Nokia Komposer format"));newline();newline();
	}
	else if (ch == 'i')
	{
		temptempo=eeprom_read_byte(&tempo);
		if (temptempo < 51)
		{
			temptempo++;
			eeprom_update_byte(&tempo, temptempo);
			newline();
			putstrf(PSTR("Tempo="));
			putvar(temptempo*5, 4,10);
			newline();
		}
	}
	else if (ch == 'd')
	{
		temptempo=eeprom_read_byte(&tempo);
		if (temptempo > 2)
		{
			temptempo--;
			eeprom_update_byte(&tempo, temptempo);
			newline();
			putstrf(PSTR("Tempo="));
			putvar(temptempo*5, 4,10);
			newline();
		}
	}
	else if (ch == 'u')
	{
		putch(12);
		putstrf(PSTR("tekan tombol sukses, silahkan kirim"));
		newline();
		a=2;
		ch2 = getch();
		while ( ch2 != 's')
		{
			baf[0]=0;
			baf[1]=0;
			baf[2]=0;
			baf[3]=0;
			baf[4]=0;
			nadbaf[0]=0;
			nadbaf[1]=0;
			nadbaf[2]=0;
			dur=0;
			nad=0;
			i=0;
			while (ch2 != ' ')
			{
				baf[i] = ch2;
				i++;
				ch2 = getch();
			}
			ch2 = getch();
			if(baf[0] == '1')
			{
				if (baf[1] == '6')
				{
					dur = 16;
					nadbaf[0] = baf[2];
					nadbaf[1] = baf[3];
					nadbaf[2] = baf[4];				
				}
				else
				{
					dur = 1;
					nadbaf[0] = baf[1];
					nadbaf[1] = baf[2];
					nadbaf[2] = baf[3];
				}
			}		
			else if(baf[0] == '3')
			{
				dur = 32 ;
				nadbaf[0] = baf[2];
				nadbaf[1] = baf[3];
				nadbaf[2] = baf[4];	
			}		
			else if(baf[0] == '2')
			{
				dur = 2;
				nadbaf[0] = baf[1];
				nadbaf[1] = baf[2];
				nadbaf[2] = baf[3];
			}		
			else if(baf[0] == '4')
			{
				dur = 4;
				nadbaf[0] = baf[1];
				nadbaf[1] = baf[2];
				nadbaf[2] = baf[3];
			}		
			else if(baf[0] == '8')
			{
				dur = 8;
				nadbaf[0] = baf[1];
				nadbaf[1] = baf[2];
				nadbaf[2] = baf[3];
			}
			
			if (nadbaf[0] == 'C')
			{
				if (nadbaf[1] == '#')
				{
					if(nadbaf[2] == '1') nad = 102;
					else if(nadbaf[2] == '2') nad = 114;
					else if (nadbaf[2] == '3') nad = 126;
				}
				else
				{
					if (nadbaf[1] == '1') nad = 101;
					else if (nadbaf[1] == '2' ) nad = 113;
					else if (nadbaf[1] == '3' ) nad = 125;
				}
			}
			else if (nadbaf[0] == 'c')
			{
				if (nadbaf[1] == '#')
				{
					if(nadbaf[2] == '1') nad = 102;
					else if(nadbaf[2] == '2') nad = 114;
					else if (nadbaf[2] == '3') nad = 126;
				}
				else
				{
					if (nadbaf[1] == '1') nad = 101;
					else if (nadbaf[1] == '2' ) nad = 113;
					else if (nadbaf[1] == '3' ) nad = 125;
				}
			}
			else if (nadbaf[0] == 'D')
			{
				if (nadbaf[1] == '#')
				{
					if(nadbaf[2] == '1') nad = 104;
					else if(nadbaf[2] == '2') nad = 116;
					else if (nadbaf[2] == '3') nad = 128;
				}
				else
				{
					if (nadbaf[1] == '1') nad = 103;
					else if (nadbaf[1] == '2' ) nad = 115;
					else if (nadbaf[1] == '3' ) nad = 127;
				}
			}
			else if (nadbaf[0] == 'd')
			{
				if (nadbaf[1] == '#')
				{
					if(nadbaf[2] == '1') nad = 104;
					else if(nadbaf[2] == '2') nad = 116;
					else if (nadbaf[2] == '3') nad = 128;
				}
				else
				{
					if (nadbaf[1] == '1') nad = 103;
					else if (nadbaf[1] == '2' ) nad = 115;
					else if (nadbaf[1] == '3' ) nad = 127;
				}
			}
			else if (nadbaf[0] == 'E')
			{
				if (nadbaf[1] == '1') nad = 105;
				else if (nadbaf[1] == '2' ) nad = 117;
				else if (nadbaf[1] == '3' ) nad = 129;
			}
			else if (nadbaf[0] == 'e')
			{
				if (nadbaf[1] == '1') nad = 105;
				else if (nadbaf[1] == '2' ) nad = 117;
				else if (nadbaf[1] == '3' ) nad = 129;
			}
			else if (nadbaf[0] == 'F')
			{
				if (nadbaf[1] == '#')
				{
					if(nadbaf[2] == '1') nad = 107;
					else if(nadbaf[2] == '2') nad = 119;
					else if (nadbaf[2] == '3') nad = 131;
				}
				else
				{
					if (nadbaf[1] == '1') nad = 106;
					else if (nadbaf[1] == '2' ) nad = 118;
					else if (nadbaf[1] == '3' ) nad = 130;
				}
			}
			else if (nadbaf[0] == 'f')
			{
				if (nadbaf[1] == '#')
				{
					if(nadbaf[2] == '1') nad = 107;
					else if(nadbaf[2] == '2') nad = 119;
					else if (nadbaf[2] == '3') nad = 131;
				}
				else
				{
					if (nadbaf[1] == '1') nad = 106;
					else if (nadbaf[1] == '2' ) nad = 113;
					else if (nadbaf[1] == '3' ) nad = 125;
				}
			}
			else if (nadbaf[0] == 'G')
			{
				if (nadbaf[1] == '#')
				{
					if(nadbaf[2] == '1') nad = 109;
					else if(nadbaf[2] == '2') nad = 121;
					else if (nadbaf[2] == '3') nad = 133;
				}
				else
				{
					if (nadbaf[1] == '1') nad = 108;
					else if (nadbaf[1] == '2' ) nad = 120;
					else if (nadbaf[1] == '3' ) nad = 132;
				}
			}
			else if (nadbaf[0] == 'g')
			{
				if (nadbaf[1] == '#')
				{
					if(nadbaf[2] == '1') nad = 109;
					else if(nadbaf[2] == '2') nad = 121;
					else if (nadbaf[2] == '3') nad = 133;
				}
				else
				{
					if (nadbaf[1] == '1') nad = 108;
					else if (nadbaf[1] == '2' ) nad = 120;
					else if (nadbaf[1] == '3' ) nad = 132;
				}
			}
			else if (nadbaf[0] == 'A')
			{
				if (nadbaf[1] == '#')
				{
					if(nadbaf[2] == '1') nad = 111;
					else if(nadbaf[2] == '2') nad = 123;
					else if (nadbaf[2] == '3') nad = 135;
				}
				else
				{
					if (nadbaf[1] == '1') nad = 110;
					else if (nadbaf[1] == '2' ) nad = 122;
					else if (nadbaf[1] == '3' ) nad = 134;
				}
			}
			else if (nadbaf[0] == 'a')
			{
				if (nadbaf[1] == '#')
				{
					if(nadbaf[2] == '1') nad = 111;
					else if(nadbaf[2] == '2') nad = 123;
					else if (nadbaf[2] == '3') nad = 135;
				}
				else
				{
					if (nadbaf[1] == '1') nad = 110;
					else if (nadbaf[1] == '2' ) nad = 122;
					else if (nadbaf[1] == '3' ) nad = 134;
				}
			}
			else if (nadbaf[0] == 'B')
			{
				if (nadbaf[1] == '1') nad = 112;
				else if (nadbaf[1] == '2' ) nad = 124;
				else if (nadbaf[1] == '3' ) nad = 136;
			}
			else if (nadbaf[0] == 'b')
			{
				if (nadbaf[1] == '1') nad = 112;
				else if (nadbaf[1] == '2' ) nad = 124;
				else if (nadbaf[1] == '3' ) nad = 136;
			}
			else if (nadbaf[0] == '-') nad = 100;
			
			lagubaf[a] = dur;						
			a++;
			lagubaf[a] = nad;
			a++;
		}
		lagubaf[a] = 255;
		a++;
		lagubaf[a] = 255;
		a++;
		lagubaf[a] = 255;
		a++;
		lagubaf[a] = 255;
		eeprom_update_block((const void*)lagubaf, (void*)lagu, 900);
		putstrf(PSTR("upload sukses"));
		newline();
		
		
	}
		
	else
	sei();
}

int main (void)
{
	usart_init();
	timer_init();
	tombol_init();
	putstrf(PSTR("ok"));
	uint8_t temporam;
	temporam = 12;	
	eeprom_update_byte((uint8_t*)(uint16_t)1, temporam);
	sei();
	
	while(1)
	{
		sleep_mode();
	}
}
