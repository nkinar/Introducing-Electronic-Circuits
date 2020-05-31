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
#include <avr/io.h>
#include <avr/fuse.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <util/delay.h>
#include <avr/sleep.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <alloca.h>
#include <stdbool.h>
#include "main.h"
#include "uart.h"
#include "display.h"
#include "string_helper.h"
#include "i2c.h"
#include "am2320.h"
#include "gpio8.h"
#include "gpio16.h"
#include "check_present.h"
#include "gpio_switch.h"
#include "adc.h"
#include "run_collection.h"
#include "timer.h"


/*
Fuses used to set:
1. Brownout detector ON
2. CLKDIV/8 off
3. External oscillator

These fuses are read using Tools -> Device Programming
*/
FUSES = 
{
.extended = 0xF9,
.high = 0xD4,
.low = 0xF7
};


#ifdef DEBUG_BOARD
static struct main_data
{
	char buffer[SERIAL_BUFFER_SIZE];
	bool process_command;
	int uart_num;
} mdata;
#endif


void setup()
{
	#ifdef DEBUG_BOARD
		setup_uart();
	#endif
	setup_display();
	print_welcome();
	setup_i2c();
	check_present(); // this must be called before setup of the gpio expanders 
	setup_gpio8();  
	setup_gpio16();
	setup_switch();
	setup_timer(); 
	collection_setup();
	
	#ifdef DEBUG_BOARD
		mdata.uart_num = UART_USB_NUM; 
		mdata.process_command = false;
	#endif
	
} // end

//----------------------------------------------------
#ifdef DEBUG_BOARD
//----------------------------------------------------

void send_buffer_to_main(char *buff, int uart_num)
{
	strlcpy(mdata.buffer, buff, SERIAL_BUFFER_SIZE);
	mdata.uart_num = uart_num;
	mdata.process_command = true;
} // end


void print_uart(const char *s)
{
	if(mdata.uart_num==UART_USB_NUM)
	{
		uart_send_string_crlf(s);
	}
} // end


void print_uart_no_crlf(const char *s)
{
	if(mdata.uart_num==UART_USB_NUM)
	{
		uart_send_string(s);
	}
} // end


void print_info()
{
	uart_send_string_crlf("Class Sensor");
	uart_send_string_crlf("SWSL@usask, nj kinar");
	uart_send_string_crlf(__DATE__" "__TIME__);
} // end


void uart_send_prompt()
{
	uart_send_string_crlf("");  // send CRLF to advance to the next line
	uart_send_string(PROMPT);   // on CRLF on end
} // end


void commmand_process(void)
{
	if(!mdata.process_command) return;

	char *s = trimwhitespace(mdata.buffer);
	uart_send_string_crlf("");                                          // send CRLF

	//----------------------------------------------------------------------------
	// INFO
	//----------------------------------------------------------------------------

	if(compare_string(s, INFO_COMMAND))                                 // info
	{
		print_info();
	}
	else if (compare_string(s, HW_COMMAND))								// hw to print Hello World on the display
	{
		print_hello();
		print_uart(DONE_STRING);
	}
	else if (compare_string_chars(s, TRH_COMMAND))						// trh,[temperature],[rh]
	{
		print_true_false(parse_t_rh(s));
	}
	else if (compare_string_chars(s, VFX_COMMAND))						// vfx,[voltage],[flux]
	{
		print_true_false(parse_v_flux(s));
	}
	else if (compare_string(s, RT_COMMAND))								// rt	to read the temperature and RH and print this to the terminal
	{
		print_true_false(print_adm2320()); 
	}
	else if (compare_string(s, P8_COMMAND))								// p8	to print the GPIO8 expander levels
	{
		print_level_gpio8();
	}
	else if (compare_string(s, P32_COMMAND))							// p32	to print the GPIO16 expander levels
	{
		print_level_gpio32();
	}
	else if (compare_string(s, CP_COMMAND))								// cp	to print whether the ICs are present
	{
		check_present_print();
	}	
	else if (compare_string(s, SW_COMMAND))								// sw	to print the state of the switch
	{
		print_true_false(get_operation_state_t_rh());
	}
	else if (compare_string(s, ADC_COMMAND))							// adc	to print the output of the ADC
	{
		print_adc();
	}
	else if (compare_string(s, CD_COMMAND))								// cd to display the data
	{
		collect_data();
	}
	else if (compare_string(s, ON_TIMER_COMMAND))						// on-timer
	{
		on_timer();
	}
	else if (compare_string(s, OFF_TIMER_COMMAND))						// off-timer
	{
		off_timer();
	}
	else if (compare_string(s, ITO_COMMAND))							// ito
	{
		print_true_false(is_timer_on());
	}
	else if (compare_string_chars(s, SET_FLOAT_COMMAND))				// sf,[value] to set the coefficient used to scale the pyranometer
	{
		print_true_false(set_coeff(s));
	}
	else if (compare_string(s, WELCOME_COMMAND))						// welcome
	{
		print_welcome(); 
	}
	
	uart_send_prompt();             // send prompt back to UART in preparation for the next command
	mdata.process_command = false;  // important to reset flag
	
} // end


//----------------------------------------------------
#endif  // DEBUG_BOARD
//----------------------------------------------------



void checks()
{
	#ifdef DEBUG_BOARD
		check_uart(); 
		commmand_process();
	#endif
	
	check_timer(); 
} // end


int main()
{
	wdt_enable(WDTO_8S);	// 8 second watchdog
	setup();				// setup everything
	
	#ifndef DEBUG_BOARD
		on_timer();
	#endif
	
	#ifndef DEBUG_BOARD	
		set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	#endif
	
	sei();					// turn on all interrupts
	
    while (1) 
    {
		checks();
		wdt_reset();
		#ifndef DEBUG_BOARD	
			sleep_enable();
		#endif
    }
	return 0;
} // end


