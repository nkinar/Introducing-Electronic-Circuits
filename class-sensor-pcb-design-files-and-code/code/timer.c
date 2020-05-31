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
#include <stdbool.h>
#include "timer.h"
#include "main.h"
#include "run_collection.h"

/* 
OCRn =  [ (clock_speed / Prescaler_value) * Desired_time_in_Seconds ] - 1

References:
https://sites.google.com/site/qeewiki/books/avr-guide/timers-on-the-atmega328
*/


struct timer_data
{
	volatile bool flag;
	bool timer_on;
}td;


void setup_timer()
{
	td.timer_on = false;
	td.flag = false;
} // end


void on_timer()
{
	cli();									// off interrupts
	td.flag = false;						// clear flag
	TCCR1B = 0x00;							// clear timer register
	TCCR1B |= (1 << WGM12);					// mode4, ctc on OCR1A
	TCCR1B |= (1 << CS12) | (1 << CS10);	// prescaler to /1024
	TIMSK1 |= (1 << OCIE1A);				// interrupt on compare
	OCR1A = 15624;							// 2 second period 
	td.timer_on = true;
	sei();									// on interrupts
} // end	


void off_timer()
{
	cli();
	TCCR1B = 0x00;
	td.flag = false;
	td.timer_on = false;
	sei();
} // end


void check_timer()
{
	if (!td.flag) return;
	collect_data();  // run the data collection routine
	td.flag = false;
} // end

bool is_timer_on()
{
	return td.timer_on;
} // end


ISR (TIMER1_COMPA_vect)
{
	td.flag = true;
} // end
