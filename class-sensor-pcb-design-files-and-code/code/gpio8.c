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
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>
#include "gpio8.h"
#include "i2c.h"
#include "main.h"
#include "check_present.h"


void write_gpio8_reg(const uint8_t addr, const uint8_t reg, const uint8_t data)
{
	I2C_Start_write(addr);	
	I2C_Write(reg);
	I2C_Write(data);
	I2C_Stop();
} // end


void read_gpio8_reg(uint8_t *data, const uint8_t addr, const uint8_t reg)
{
	I2C_Start_write(addr);
	I2C_Write(reg);
	I2C_Repeated_Start_read(addr);
	*data = I2C_Read_Nack();
	I2C_Stop();
} // end


void setup_gpio8()
{
	if(check_is_t_rh_ready()==false) return;
	write_gpio8_reg(GPIO8_ADDR, GPIO8_IODIR_REG, 0xFF);		// all ports as inputs
	write_gpio8_reg(GPIO8_ADDR, GPIO8_IOPOL_REG, 0xFF);		// invert input levels
	write_gpio8_reg(GPIO8_ADDR, GPIO8_GPPU_REG, 0xFF);		// all ports have weak pullups	
} // end


uint8_t read_level_gpio8()
{
	if(check_is_t_rh_ready()==false) return 0;
	uint8_t data;
	read_gpio8_reg(&data, GPIO8_ADDR, GPIO8_GPIO_REG);
	return data;
} // end


#ifdef DEBUG_BOARD
void print_level_gpio8()
{
	if(check_is_t_rh_ready()==false) return;
	char s[9];
	uint8_t levels = read_level_gpio8();
	snprintf(s, 9, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(levels));
	print_uart(s);
} // end
#endif

