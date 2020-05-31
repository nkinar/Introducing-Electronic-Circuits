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
#include <avr/sfr_defs.h>
#include <stdint.h>
#include <stdbool.h>
#include "adc.h"
#include "check_present.h"
#include "constants.h"
#include "string_helper.h"

/*
REFERENCE:
https://protostack.com.au/2011/02/analogue-to-digital-conversion-on-an-atmega168/
*/


static struct adc_data
{
	bool adc_on;
}adata;


void adc_setup()
{
	ADCSRA = _BV(ADEN) | _BV(ADPS2) | _BV(ADPS1) | _BV(ADPS0);
	const uint8_t channel = 0;
	ADMUX = (ADMUX & 0xf0) | channel;
	DIDR0 |= _BV(ADC0D);
	adata.adc_on = true;
} // end


uint16_t read_adc()
{
	if (adata.adc_on==false) adc_setup();
	ADCSRA |= _BV(ADSC);
	while(!bit_is_set(ADCSRA,ADIF));
	ADCSRA |= _BV(ADIF); 
	return ADC;
} // end


float read_adc_voltage()
{
	float av = 0.0f;
	for(int k = 0; k < ADC_AVERAGE_TIMES; k++)
	{
		float read = (float)read_adc();
		float out = (read*VREF)/1024.0f;
		av += out;
	}
	av /= (float)ADC_AVERAGE_TIMES;
	return av;
} // end


#ifdef DEBUG_BOARD
void print_adc()
{
	print_string_then_fnumber("adc = ", read_adc_voltage());
} // end
#endif
