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

#pragma once
#include <stdint.h>
#include <stdbool.h>

#define GPIO16_BYTES			2		// two bytes to read

void write_gpio16_reg(const uint8_t addr, const uint8_t reg_start, const uint16_t data);
void read_gpio16_reg(uint8_t data[GPIO16_BYTES], const uint8_t addr, const uint8_t reg);
void setup_gpio16();
uint16_t read_level_gpio16(uint8_t addr);
uint8_t reverse(uint8_t b);
uint32_t read_level_gpio32();
void print_level_gpio32();

#define GPIO16_IODIRA_REG		0x00
#define GPIO16_IODIRB_REG		0x01

#define GPIO16_IPOLA_REG		0x02
#define GPIO16_IPOLB_REG		0x03

#define GPIO16_GPPUA_REG		0x0C
#define GPIO16_GPPUB_REG		0x0D

#define GPIO16_GPIOA_REG		0x12
#define GPIO16_GPIOB_REG		0x13