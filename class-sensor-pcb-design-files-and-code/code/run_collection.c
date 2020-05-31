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

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "constants.h"
#include "run_collection.h"
#include "check_present.h"
#include "am2320.h"
#include "gpio8.h"
#include "display.h"
#include "adc.h"
#include "gpio16.h"
#include "gpio_switch.h"
#include "string_helper.h"
#include "main.h"

static struct collection_data
{
	float coeff;	
}cd;


void collection_setup()
{
	cd.coeff = 0.0f;
} // end


void collect_t_rh()
{
	uint16_t temp, rh;
	obtain_am2320(&temp, &rh);
	float temp_f = get_temperature_pos_neg(temp);  // required to scale the temperature positive or negative
	float rh_f = (float)rh;
	uint8_t level;
	
	#ifdef USE_GPIO
		level = read_level_gpio8();
		if(level==0) level = 1;
	#else
		level = T_RH_SCALE;
	#endif
	
	temp_f /= (float)level;
	rh_f /= (float)level;
	display_t_rh(temp_f, rh_f);
	
} // end



void binary_to_float(float *output, const uint32_t binary)
{
	memcpy(output, &binary, sizeof(float));
} // end



void collect_pyranometer()
{
	float v = read_adc_voltage();
	float scale;
	uint32_t scale_uint = 0;
	#ifdef USE_GPIO
		if(cd.coeff == 0.0f)
		{
			scale_uint = read_level_gpio32();
			binary_to_float(&scale, scale_uint);
		}
		else // cd.coeff is not zero, so it is valid
		{	
			scale = cd.coeff;
		}
	#else
		scale = (cd.coeff == 0.0f) ? PYR_SCALE : cd.coeff;
	#endif
	float flux = scale * v;
	display_v_flux(v, flux);
} // end



/*
Entry point function to call to collect data.
This is also called from the timer as well.
CLI: cd
*/
void collect_data()
{
	if(get_operation_state_t_rh())		// switch is high or pullup active
	{
		#ifdef USE_GPIO
			if(check_is_t_rh_ready())
			{
				collect_t_rh();
			}
			else if (!check_gpio8() && check_gpio32())
			{
				collect_pyranometer();
			}	
		#else
			if(check_temp_rh())
			{
				collect_t_rh();
			} 
			else
			{
				collect_pyranometer();
			}
		#endif	
	}
	else								// switch is low
	{
		collect_pyranometer();
	}
} // end


#ifdef DEBUG_BOARD
bool set_coeff(char *s)
{
	float f;
	int ret = sscanf(s, SET_FLOAT_COMMAND",%f", &f);
	if(ret != 1) return false;
	print_string_then_fnumber("coeff = ", f);
	cd.coeff = f;
	return true;
} // end
#endif


