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
#include <stdint.h>
#include <stdbool.h>
#include "check_present.h"
#include "i2c.h"
#include "main.h"
#include "string_helper.h"
#include "am2320.h"

static struct check_present_data
{
	bool temp_rh;
	bool gpio8;
	bool gpio32;
	bool is_t_rh_ready;
} pd;



#ifdef DEBUG_BOARD
void check_present_print()
{
	print_string_then_unsigned_number("trh = ", check_is_t_rh_ready());
	print_string_then_unsigned_number("pyro = ", check_pyro_ready());	
} // end
#endif


void check_present_update(uint8_t addr, bool *var)
{
	*var = check_i2c_present(addr);
} // end


void check_present()
{
	_delay_ms(1000); // we need to wait until voltage levels are stable before checking the AM2320 IC
	wake_am2320();
	check_present_update(T_RH_ADDR, &pd.temp_rh);
	check_present_update(GPIO8_ADDR, &pd.gpio8);
	pd.is_t_rh_ready = pd.temp_rh && pd.gpio8;
	
	bool gpio16_low, gpio16_high;
	check_present_update(GPIO16_ADDR_LOW, &gpio16_low);
	check_present_update(GPIO16_ADDR_HIGH, &gpio16_high);
	pd.gpio32 = gpio16_low && gpio16_high;
	
	#ifdef PRINT_DEBUG_STARTUP_I2C
		print_string_then_unsigned_number("pd.temp_rh = ", pd.temp_rh);
		print_string_then_unsigned_number("pd.gpio8 = ", pd.gpio8);
		print_string_then_unsigned_number("pd.gpio16_low = ", gpio16_low);
		print_string_then_unsigned_number("pd.gpio16_low = ", gpio16_high);
	#endif 
} // end


bool check_temp_rh()
{
	return pd.temp_rh; 
} // end


bool check_gpio8()
{
	#ifdef FORCE_PYRO // force GPIO8 as missing
		return false;
	#else
		return pd.gpio8; 
	#endif
} // end


bool check_gpio32()
{
	return pd.gpio32;
} // end


//------------------------------------------------------------------------------

/*
Use this function to check and see if the T and RH sensors can be used 
*/ 
bool check_is_t_rh_ready()
{
	#ifdef FORCE_PYRO
		return false;
	#else
		return pd.is_t_rh_ready;
	#endif
} // end


/*
Use this function to check and see if the Pyranometer sensor can be used
(if the GPIO expander has been fitted)
*/
bool check_pyro_ready()
{
	return pd.gpio32;
} // end

//------------------------------------------------------------------------------

