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
#include <stdbool.h>
#include <stdint.h>
#include <math.h>
#include "constants.h"
void setup_i2c();
uint8_t I2C_Start(uint8_t address);
uint8_t I2C_Repeated_Start(uint8_t address);
uint8_t I2C_Start_write(uint8_t address);
uint8_t I2C_Start_read(uint8_t address);
uint8_t I2C_Repeated_Start_read(uint8_t address);
uint8_t I2C_Repeated_Start_write(uint8_t address);
uint8_t I2C_Write(uint8_t data);
uint8_t I2C_Read_Ack();
uint8_t I2C_Read_Nack();
void I2C_Stop();
bool check_i2c_present(uint8_t addr);

// macros
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))

// return codes
#define I2C_ACK			1
#define I2C_NAK			2
#define I2C_FAIL		3