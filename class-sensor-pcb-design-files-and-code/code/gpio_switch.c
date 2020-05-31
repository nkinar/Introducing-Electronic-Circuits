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
#include <stdint.h>
#include <stdbool.h>
#include "gpio_switch.h"
#include "constants.h"
#include "check_present.h"

void setup_switch()
{
	BIT_CLEAR(DDRB, 2); // port PB2 as input
	BIT_SET(PORTB, 2);  // pull-up on PB2
} // end


/*
This function reads the state of the switch for the operation state.
Returns:

true = if the temperature and RH is to be shown on the display
false = if the pyranometer is to be used

If the T and RH sensor is not present, then it is assumed that the unit is pyranometer only.
Other checks are conducted when the data is sampled.
*/
bool get_operation_state_t_rh()
{
	if (BIT_CHECK(PINB, 2))
	{
		return true;
	}
	return false;
} // end 

