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
#include <stdio.h>
#include "gpio16.h"
#include "i2c.h"
#include "main.h"
#include "uart.h"
#include "constants.h"
#include "check_present.h"


void write_gpio16_reg(const uint8_t addr, const uint8_t reg_start, const uint16_t data)
{
	I2C_Start_write(addr);
	I2C_Write(reg_start);
	I2C_Write((uint8_t)(data & 0xFF));
	I2C_Write((uint8_t)(data >> 8));
	I2C_Stop();
} // end


void read_gpio16_reg(uint8_t data[GPIO16_BYTES], const uint8_t addr, const uint8_t reg)
{
	I2C_Start_write(addr);
	I2C_Write(reg);
	I2C_Repeated_Start_read(addr);
	data[0] = I2C_Read_Ack();
	data[1] = I2C_Read_Nack();
	I2C_Stop();
} // end


void setup_gpio16()
{
	if (check_pyro_ready()==false) return;
	
	// ensure pins are inputs
	write_gpio16_reg(GPIO16_ADDR_LOW,  GPIO16_IODIRA_REG, 0xFFFF);
	write_gpio16_reg(GPIO16_ADDR_HIGH,  GPIO16_IODIRA_REG, 0xFFFF);
	
	// input polarity inverted
	write_gpio16_reg(GPIO16_ADDR_LOW,  GPIO16_IPOLA_REG, 0xFFFF);
	write_gpio16_reg(GPIO16_ADDR_HIGH, GPIO16_IPOLA_REG, 0xFFFF);
	
	// pullups
	write_gpio16_reg(GPIO16_ADDR_LOW,  GPIO16_GPPUA_REG, 0xFFFF);
	write_gpio16_reg(GPIO16_ADDR_HIGH, GPIO16_GPPUA_REG, 0xFFFF);
} // end


/*
Function to reverse the bits in a byte
REFERENCE: https://stackoverflow.com/questions/2602823/in-c-c-whats-the-simplest-way-to-reverse-the-order-of-bits-in-a-byte
*/
uint8_t reverse(uint8_t b) 
{
	b = (b & 0xF0) >> 4 | (b & 0x0F) << 4;
	b = (b & 0xCC) >> 2 | (b & 0x33) << 2;
	b = (b & 0xAA) >> 1 | (b & 0x55) << 1;
	return b;
} // end 


uint16_t read_level_gpio16(uint8_t addr)
{
	if (check_pyro_ready()==false) return 0;
	uint8_t data[GPIO16_BYTES];
	read_gpio16_reg(data, addr, GPIO16_GPIOA_REG);
	uint8_t low = data[0];
	uint8_t high = reverse(data[1]);  // the bits are reversed by wiring on the PCB
	uint16_t out = ((uint16_t)high<<8) | (uint16_t)low;
	return out;
} // end


uint32_t read_level_gpio32()
{
	if (check_pyro_ready()==false) return 0; 
	uint16_t low = read_level_gpio16(GPIO16_ADDR_LOW);
	uint16_t high = read_level_gpio16(GPIO16_ADDR_HIGH);
	uint32_t out = ((uint32_t)high<<16) | (uint32_t)low;
	return out;
} // end


#ifdef DEBUG_BOARD
void print_level_gpio32()
{	
	if (check_pyro_ready()==false) return;
	char s[9];
	uint32_t levels = read_level_gpio32();
	for(int k = 3; k >= 0; k--)
	{
		uint8_t b = (uint8_t)((levels >> (k*8)) & 0xFF);
		snprintf(s, 9, BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(b));
		print_uart_no_crlf(s);
	}
	uart_send_string(CRLF);
} // end
#endif


