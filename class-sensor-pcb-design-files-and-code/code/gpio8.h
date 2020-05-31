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

#define GPIO8_IODIR_REG		0x00
#define GPIO8_IOPOL_REG		0x01
#define GPIO8_GPPU_REG		0x06
#define GPIO8_GPIO_REG		0x09

void write_gpio8_reg(const uint8_t addr, const uint8_t reg, const uint8_t data);
void read_gpio8_reg(uint8_t *data, const uint8_t addr, const uint8_t reg); 
void setup_gpio8();
void print_level_gpio8();
uint8_t read_level_gpio8();

