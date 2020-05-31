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
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>
#include "i2c.h"
#include "am2320.h"
#include "string_helper.h"
#include "main.h"

static struct am23220_data
{
	uint8_t resp[AM2320_BYTES_RESP];
	bool last_good;
	uint16_t humidity_raw;
	uint16_t temperature_raw;
}ad;




void wake_am2320()
{
	 I2C_Start(0xB8);
	 _delay_us(1000);
	 I2C_Stop();
} // end


unsigned short crc16(unsigned char *ptr, unsigned char len)
{
	unsigned short crc =0xFFFF;
	unsigned char i;
	while(len--)
	{
		crc ^=*ptr++;
		for(i=0;i<8;i++)
		{
			if(crc & 0x01)
			{
				crc>>=1;
				crc^=0xA001;
			}else
			{
				crc>>=1;
			}
		}
	}
	return crc;
}


float get_temperature_pos_neg(uint16_t t)
{
	uint16_t temp = t;
	float out = 1.0f;
	if(BIT_CHECK(temp, 15))
	{
		BIT_CLEAR(temp, 15);
		out = -1.0f;
	}
	out *= (float)temp;
	return out;
} // end


void read_am2320()
{
	// wake up the am2320
	wake_am2320();
	
	// send read command
	I2C_Start(0xB8);
	I2C_Write(0x03);
	I2C_Write(0x00);
	I2C_Write(0x04);
	I2C_Stop();
	
	// wait for measurement
	_delay_ms(2);
	I2C_Start(0xB9);  // start for read
	
	// wait after start
	_delay_us(50);
	
	// read data
	for(uint8_t k = 0; k < AM2320_BYTES_RESP-1; k++)
	{
		ad.resp[k] = I2C_Read_Ack();
	}
	ad.resp[AM2320_BYTES_RESP-1] = I2C_Read_Nack();
	I2C_Stop();
	
	if(ad.resp[0] != 0x03) return;
	if(ad.resp[1] != 0x04) return;
	
	ad.humidity_raw = ((uint16_t)ad.resp[2] << 8) | ((uint16_t)ad.resp[3]);
	ad.temperature_raw = ((uint16_t)ad.resp[4] << 8) | ((uint16_t)ad.resp[5]);
	uint16_t crc_raw = ((uint16_t)ad.resp[7] << 8) | ((uint16_t)ad.resp[6]);	
	unsigned short output_crc = crc16(ad.resp, 6);
		
	if (crc_raw == output_crc) ad.last_good = true;
	else ad.last_good = false;
} // end


bool obtain_am2320(uint16_t *temp, uint16_t *humidity)
{
	read_am2320(); 
	if(ad.last_good)
	{	
		*humidity = ad.humidity_raw;
		*temp = ad.temperature_raw;
		return true;
	}
	*humidity = 0;
	*temp = 0;
	return false;
} // end



#ifdef DEBUG_BOARD
bool print_adm2320()
{
	read_am2320();
	if(ad.last_good==false) return false;
	float temperature = get_temperature_pos_neg(ad.temperature_raw) / 10.0f;
	float rh = (float)ad.humidity_raw / 10.0f;
	print_string_then_fnumber("temperature = ", temperature);
	print_string_then_fnumber("rh = ", rh);
	return true;
} // end
#endif

