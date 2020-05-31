/* 
 * This file is part of the class sensor code for the
 * "Introducing Electronic Circuits and Hydrological
 * Models to Postsecondary Physical Geography Students: 
 * Systems Science, Circuit Theory, Construction and Calibration" paper
 * 
 * Copyright (c) 2019-2020 Nicholas J. Kinar
 * 
 * This program is free software: you can redistribute it and/or modify  
 * it under the terms of the GNU General Public License as published by  
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU 
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "uart.h"
#include "main.h"
#include "constants.h"

//----------------------------------
#ifdef DEBUG_BOARD
//----------------------------------

static struct uart_data
{
	volatile char c;
	volatile bool flag;
	char buffer[UART_BUFFER_SIZE];
	uint8_t pos;

}ud;

void setup_uart()
{
	UBRR0H = (BAUDRATE_UART>>8);        // shift the register right by 8 bits
	UBRR0L = BAUDRATE_UART;             // set baud rate
	UCSR0B|= (1<<TXEN0)|(1<<RXEN0)|(1<<RXCIE0);     // enable receiver and transmitter and receive interrupt
	UCSR0C|= (1<<UCSZ00)|(1<<UCSZ01);   // 8bit data format
	ud.flag = false;
	uart_clear_buffer();
} // end


void send_uart_char(char c)
{
	 while (!( UCSR0A & (1<<UDRE0)));
	 UDR0 = (uint8_t)c;
} // end


void uart_send_string(const char *s)
{
	while(*s != 0)
	{
		send_uart_char(*s);
		s++;
	}
} // end


void uart_send_string_crlf(const char *s)
{
	 uart_send_string(s);
	 uart_send_string(CRLF);
} // end


void uart_send_and_clear()
{
	ud.buffer[ud.pos+1] = STR_TERMINATOR;
	send_buffer_to_main(ud.buffer, UART_USB_NUM);
	uart_clear_buffer();
} // end


void uart_delete_char()
{
	send_uart_char(BACKSPACE);
	send_uart_char(SPACE);
	send_uart_char(BACKSPACE);
} // end


void check_uart()
{
	    if(ud.flag)
	    {
		    if(ud.c==LT)  // line terminator
		    {
			    uart_send_and_clear();
		    }
		    else  // not line terminator
		    {
			    if(ud.c==BACKSPACE)
			    {
				    if(ud.pos != 0)
				    {
					    uart_delete_char();
					    ud.buffer[--ud.pos] = STR_TERMINATOR;
				    }
			    }
			    else
			    {
				    send_uart_char(ud.c);                   // echo character
				    if(ud.pos < (UART_BUFFER_SIZE-1))       // ensure that there is space at the end for the string terminator
				    {
					    ud.buffer[ud.pos++] = ud.c;  // add the character
				    }
				    else
				    {
					    uart_send_and_clear();  // exceeds the buffer, so send the buffer
				    }
			    }
		    }
		    ud.flag = false; // data has been handled
	    }	
} // end


void uart_clear_buffer()
{
	memset(ud.buffer,0,sizeof(ud.buffer));
	ud.pos = 0;
} // end


//--------------------------------------------------

ISR(USART_RX_vect)
{
	unsigned char data = UDR0;
	ud.c = data;
	ud.flag = true;
} // end


//----------------------------------
#endif
//----------------------------------


