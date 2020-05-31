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


#include "constants.h"
#include <util/delay.h>
#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "display.h"
#include "string_helper.h"
#include "main.h"

static struct display_data
{
	char buff[MAX_DISPLAY_BYTES];
}dd;

/*
Bits are ordered the same as in the datasheet
RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
*/
void control_display(uint16_t word)
{
	for(int k = 0; k <= 9; k++)
	{
		bool set = false;
		if(BIT_CHECK(word, k)) set = true;
		if(k==0)		set_d0(set);
		else if (k==1)	set_d1(set);
		else if (k==2)	set_d2(set);
		else if (k==3)	set_d3(set);
		else if (k==4)	set_d4(set);
		else if (k==5)	set_d5(set);
		else if (k==6)	set_d6(set);
		else if (k==7)	set_d7(set);
		else if (k==8)	set_rw(set);
		else if (k==9)	set_rs(set);
	}
	set_en(HIGH);
	_delay_us(1);
	set_en(LOW);
	_delay_us(40);
} // end


void set_d0(bool val)
{
	if(val) BIT_SET(PORTD, 2);
	else BIT_CLEAR(PORTD, 2); 
} // end


void set_d1(bool val)
{
	if(val) BIT_SET(PORTD, 3);
	else BIT_CLEAR(PORTD, 3); 
} // end

void set_d2(bool val)
{
	if(val) BIT_SET(PORTD, 4);
	else BIT_CLEAR(PORTD, 4);
} // end

void set_d3(bool val)
{
	if(val) BIT_SET(PORTD, 5);
	else BIT_CLEAR(PORTD, 5);
} // end

void set_d4(bool val)
{
	if(val) BIT_SET(PORTD, 6);
	else BIT_CLEAR(PORTD, 6);
} // end

void set_d5(bool val)
{
	if(val) BIT_SET(PORTD, 7);
	else BIT_CLEAR(PORTD, 7);
} // end

void set_d6(bool val)
{
	if(val) BIT_SET(PORTB, 0);
	else BIT_CLEAR(PORTB, 0);
} // end

void set_d7(bool val)
{
	if(val) BIT_SET(PORTB, 1);
	else BIT_CLEAR(PORTB, 1);
} // end


void set_rs(bool val)
{
	if(val) BIT_SET(PORTC, 1);
	else BIT_CLEAR(PORTC, 1);
} // end


void set_rw(bool val)
{
	if(val) BIT_SET(PORTC, 2);
	else BIT_CLEAR(PORTC, 2);
} // end

void set_en(bool val)
{
	if(val) BIT_SET(PORTC, 3);
	else BIT_CLEAR(PORTC, 3);
} // end


void setup_display()
{	
	// ensure all low
	set_d0(0);
	set_d1(0);
	set_d2(0);
	set_d3(0);
	set_d4(0);
	set_d5(0);
	set_d6(0);
	set_d7(0);
	set_rs(0);
	set_rw(0);
	set_en(0);
	
	// OUTPUTS
	// D0 to D5
	DDRD |= (1<<2);
	DDRD |= (1<<3);  	
	DDRD |= (1<<4);	
	DDRD |= (1<<5);	
	DDRD |= (1<<6);	
	DDRD |= (1<<7);	
	
	// D6 to D7  
	DDRB |= (1<<0);
	DDRB |= (1<<1);
	
	// RS
	DDRC |= (1<<1);
	
	// RW
	DDRC |= (1<<2);
	
	// EN
	DDRC |= (1<<3);
	
	_delay_ms(1);
	control_display(0b0000111100);	// set display to two row operation
	control_display(0b1100);		// turn display on
	control_display(0b110);			// shift cursor to the right when writing
	clear_display();				// clear the display
	upload_custom_chars();			// upload the custom characters
	
} // end


void clear_display()
{
	control_display(0x01);
	_delay_ms(3);
	
} // end


void home_display()
{
	control_display(0x02);
	_delay_ms(3);
	
} // end 

void cursor_second_line()
{
	control_display(0b0011000000);
} // end


void display_putc(uint8_t c)
{
	uint16_t base = 0b1000000000;
	uint16_t add = (uint16_t)c;
	uint16_t out = base | add;
	control_display(out);
} // end



bool display_writestr(const char *s, uint8_t line)
{
	if(line==FIRST_LINE_DISPLAY)
	{
		home_display();
	}
	else if (line==SECOND_LINE_DISPLAY)
	{
		cursor_second_line();
	}
	else
	{
		return false;
	}
	while(*s != 0)
	{
		display_putc(*s);
		s++;
	}
	return true;
} // end


void display_deg_C(float temperature)
{
	snprintf(dd.buff, MAX_DISPLAY_BYTES, "%.5g", temperature);
	display_writestr(dd.buff, FIRST_LINE_DISPLAY);
	size_t siz = strlen(dd.buff);
	int pad = 8-siz-2;
	for(int k = 0; k < pad; k++) display_putc(' ');
	display_putc(0xDF);  // degree sign
	display_putc('C'); 
} // end

void display_rh(float rh)
{
	if (rh > 99.9f) rh = 99.9f;
	snprintf(dd.buff, MAX_DISPLAY_BYTES, "%.4g", rh);
	display_writestr(dd.buff, SECOND_LINE_DISPLAY);
	size_t siz = strlen(dd.buff);
	int pad = 8-siz-3;
	for(int k = 0; k < pad; k++) display_putc(' ');
	display_putc('%');
	display_putc('R');
	display_putc('H');
} // end


void display_t_rh(float temperature, float rh)
{
	clear_display();
	display_deg_C(temperature);
	display_rh(rh);
} // end


void display_v(float v)
{
	if(v < 0.001f) v = 0.001f;
	snprintf(dd.buff, MAX_DISPLAY_BYTES, "%.3f", v);
	display_writestr(dd.buff, FIRST_LINE_DISPLAY);
	size_t siz = strlen(dd.buff);
	int pad = 8-siz-1;
	for(int k = 0; k < pad; k++) display_putc(' ');
	display_putc('V');	
} // end


void display_flux(float flux)
{
	if (flux < 1.0f) flux = 0.0f;
	else if (flux > 999.9) flux = 999.0f;
	snprintf(dd.buff, MAX_DISPLAY_BYTES, "%.3g", flux);
	display_writestr(dd.buff, SECOND_LINE_DISPLAY);
	size_t siz = strlen(dd.buff);
	int pad = 8-siz-4;
	for(int k = 0; k < pad; k++) display_putc(' ');
	display_putc('W');
	display_putc('/');
	display_putc('m');
	display_putc('2');	
} // end


void display_v_flux(float v, float flux)
{
	clear_display();
	display_v(v);
	display_flux(flux);
} // end


#ifdef DEBUG_BOARD
void print_hello()
{	
	clear_display();
	display_writestr("Hello",  FIRST_LINE_DISPLAY);
	display_writestr("World!", SECOND_LINE_DISPLAY);
} // end
#endif

/*
Upload a custom character to the display.  The code is the location of the
character in the RAM.  The map is a line by line array of pixels for a 5x8 character.
Each line is 5 bits of a byte.  See the upload_a_macron() function below for details.
*/
void upload_custom_char(uint8_t map[MAX_BYTES_CUSTOM], uint8_t code)
{
	uint8_t cmd = 0x40 | (code << 3);
	control_display(cmd);
	for (uint8_t k = 0; k < MAX_BYTES_CUSTOM; k++)
	{
		display_putc(map[k]);
	}
	home_display();  // stop the upload of custom character
} // and


/*
Upload the a with a macron
*/
void upload_a_macron()
{
	uint8_t map[MAX_BYTES_CUSTOM] = {0b11111, 0x00, 0b01110, 0b00001, 0b01111, 0b10001, 0b01111, 0x00};
	upload_custom_char(map, MACRON_A_LOC);	
} // end


void upload_custom_chars()
{
	upload_a_macron(); 
} // end 


void print_welcome()
{
	clear_display();
	display_writestr("Welcome!",  FIRST_LINE_DISPLAY);
	display_writestr(" taw", SECOND_LINE_DISPLAY);
	display_putc(MACRON_A_LOC); // a with macron stored in first location of RAM
	display_putc('w');
} // end 


#ifdef DEBUG_BOARD

void parse_num(char *s, float *v1, float *v2)
{
	char *token = strtok(s, ",");
	int cnt = 0;
	while( token != NULL )
	{
		if(cnt == 1)
		{
			*v1 = atof(token);
		}
		else if (cnt == 2)
		{
			*v2 = atof(token);
		}
		cnt += 1;
		token = strtok(NULL, ",");
	}
} // end


bool parse_t_rh(char *s)
{
	float temperature = 0.0f; 
	float rh = 0.0f;
	parse_num(s, &temperature, &rh);
	display_t_rh(temperature, rh);
	return true;
} // end


bool parse_v_flux(char *s)
{
	float v = 0.0f;
	float flux = 0.0f;
	parse_num(s, &v, &flux);
	display_v_flux(v, flux); 
	return true;	
} // end 

#endif

