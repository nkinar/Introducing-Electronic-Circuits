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
#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdint.h>
#include "i2c.h"

/*
I2C master code
REFERENCE:
https://www.electronicwings.com/avr-atmega/atmega1632-i2c 
*/

void setup_i2c()
{
	TWBR = BITRATE(TWSR=0x00);
} // end


uint8_t I2C_Start(uint8_t address)
{
	uint8_t status;							/* Declare variable */
	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT);	/* Enable TWI, generate START */
	while(!(TWCR&(1<<TWINT)));				/* Wait until TWI finish its current job */
	status=TWSR&0xF8;						/* Read TWI status register */
	if(status!=0x08)						/* Check START transmitted or not? */
	return 0;								/* Return 0 to indicate start condition fail */
	TWDR=address;							/* Write SLA+W in TWI data register */
	TWCR=(1<<TWEN)|(1<<TWINT);				/* Enable TWI & clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));				/* Wait until TWI finish its current job */
	status=TWSR&0xF8;						/* Read TWI status register */
	if(status==0x18)						/* Check for SLA+W transmitted &ack received */
	return 1;								/* Return 1 to indicate ack received */
	if(status==0x20)						/* Check for SLA+W transmitted &nack received */
	return 2;								/* Return 2 to indicate nack received */
	else
	return 3;								/* Else return 3 to indicate SLA+W failed */
} // end


uint8_t I2C_Repeated_Start(uint8_t address)
{
	uint8_t status;								/* Declare variable */
	TWCR=(1<<TWSTA)|(1<<TWEN)|(1<<TWINT);		/* Enable TWI, generate start */
	while(!(TWCR&(1<<TWINT)));					/* Wait until TWI finish its current job */
	status=TWSR&0xF8;							/* Read TWI status register */
	if(status!=0x10)							/* Check for repeated start transmitted */
	return 0;									/* Return 0 for repeated start condition fail */
	TWDR=address;								/* Write SLA+R in TWI data register */
	TWCR=(1<<TWEN)|(1<<TWINT);					/* Enable TWI and clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));					/* Wait until TWI finish its current job */
	status=TWSR&0xF8;							/* Read TWI status register */
	if(status==0x40)							/* Check for SLA+R transmitted &ack received */
	return 1;									/* Return 1 to indicate ack received */
	if(status==0x48)							/* Check for SLA+R transmitted &nack received */
	return 2;									/* Return 2 to indicate nack received */
	else
	return 3;									/* Else return 3 to indicate SLA+W failed */
}


uint8_t I2C_Start_write(uint8_t address)
{
	uint8_t a = (address << 1);
	return I2C_Start(a);
} // end


uint8_t I2C_Start_read(uint8_t address)
{
	uint8_t a = (address << 1) | 0x01;
	return I2C_Start(a);
} // end


uint8_t I2C_Repeated_Start_read(uint8_t address)
{
	uint8_t a = (address << 1) | 0x01;
	return I2C_Repeated_Start(a);
} // end


uint8_t I2C_Repeated_Start_write(uint8_t address)
{
	uint8_t a = (address << 1);
	return I2C_Repeated_Start(a);
} // end


uint8_t I2C_Write(uint8_t data)	
{
	uint8_t status;					/* Declare variable */
	TWDR=data;						/* Copy data in TWI data register */
	TWCR=(1<<TWEN)|(1<<TWINT);		/* Enable TWI and clear interrupt flag */
	while(!(TWCR&(1<<TWINT)));		/* Wait until TWI finish its current job */
	status=TWSR&0xF8;				/* Read TWI status register */
	if(status==0x28)				/* Check for data transmitted &ack received */
	return 0;						/* Return 0 to indicate ack received */
	if(status==0x30)				/* Check for data transmitted &nack received */
	return 1;						/* Return 1 to indicate nack received */
	else
	return 2;						/* Else return 2 for data transmission failure */
} // end 


uint8_t I2C_Read_Ack()
{
	TWCR=(1<<TWEN)|(1<<TWINT)|(1<<TWEA);				/* Enable TWI, generation of ack */
	while(!(TWCR&(1<<TWINT)));							/* Wait until TWI finish its current job */
	return TWDR;										/* Return received data */
} // end


uint8_t I2C_Read_Nack()	
{
	TWCR=(1<<TWEN)|(1<<TWINT);	
	while(!(TWCR&(1<<TWINT)));	
	return TWDR;		
} // end


void I2C_Stop()		
{
	TWCR=(1<<TWSTO)|(1<<TWINT)|(1<<TWEN);			/* Enable TWI, generate stop */
	while(TWCR&(1<<TWSTO));							/* Wait until stop condition execution */
	_delay_ms(10);  // delay required after each stop
} // end


bool check_i2c_present(uint8_t addr)
{
	uint8_t rv = I2C_Start_write(addr);
	I2C_Stop();
	if(rv == I2C_ACK) return true;
	return false;
} // end











