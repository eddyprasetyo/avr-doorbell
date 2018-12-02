void usart_init (void)
{
	DDRD |= (1<<PD1);
	UCSR0B |= (1<<RXEN0) | (1<<TXEN0) | (1<<RXCIE0); //ngaktifke tx n rx buffer, rx complete interrupt enable
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00) ; //milih 8 bit data, sisane default (parity none, stop bit 1 flowcontrol none)
	UBRR0 = 5 ; // baud rate 19200
}

unsigned char getch(void)
{
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}


void putch(unsigned char siji)
{
	while (!(UCSR0A & (1<<UDRE0)));
	{
		UDR0 = siji;
	}
}

void putstr(unsigned char* karakter)
{
	while(*karakter)
	putch(*karakter++);
}

void putstrf(char* karakter)
{
	while(pgm_read_byte(&(*karakter)))
	putch(pgm_read_byte(&(*karakter++)));
}

void putvar(int16_t variable, uint8_t n, int8_t r)
{
	unsigned char s[n];
    itoa( variable, s, r );
	putstr(s);
}

char anych(void)
{
	if (UCSR0A &  (1<<RXC0))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void newline(void)
{
	putch(13);
	putch(10);
}
