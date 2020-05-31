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

// DEFINES
#define F_CPU   8000000UL			// 8 MHz clock

#define USE_GPIO						// turn on this define to use GPIO to obtain coefficients (for student boards)

// #define DEBUG_BOARD					// turn on this define to debug the board and allow for serial port access
// #define PRINT_DEBUG_STARTUP_I2C		// turn on this define to print the startup I2C information
// #define FORCE_PYRO					// turn on this define to force pyranometer mode on a fully-populated board

// Addresses
#define T_RH_ADDR	0x5C			// T-RH sensor
#define GPIO8_ADDR	0x20			// 8-bit GPIO expander address
#define GPIO16_ADDR_LOW		0x21	// low word 16-bit expander
#define GPIO16_ADDR_HIGH	0x22	// high word 16-bit expander
	
#define ADC_AVERAGE_TIMES	25
#define VREF				5.0f

#define HIGH	1
#define LOW		0

#define BAUD_UART  9600UL   // 9600 baud uart
#define BAUDRATE_UART ((F_CPU)/(BAUD_UART*16UL)-1)
#define UART_USB_NUM	0

#define UART_BUFFER_SIZE   16
#define SERIAL_BUFFER_SIZE   16

#define MAX_DISPLAY_BYTES	9
#define FIRST_LINE_DISPLAY  0
#define SECOND_LINE_DISPLAY  1

#define CRLF "\r\n"
#define CR '\r'
#define LF '\n'
#define BACKSPACE '\b'
#define SPACE ' '
#define STR_TERMINATOR '\0'
#define PROMPT ">"
#define LT  CR

#define ERROR_STRING   "ERROR"
#define DONE_STRING    "DONE"
#define TRUE_STRING		"TRUE"
#define FALSE_STRING	"FALSE"

#define SCL_CLK		100000			// 100 kHz clock for I2C

#define T_RH_SCALE		10			// scaling coefficient for temperature
#define PYR_SCALE		156.3f		// scaling coefficient for pyranometer

// COMMANDS
#define INFO_COMMAND	"info"
#define HW_COMMAND		"hw"
#define TRH_COMMAND		"trh"
#define VFX_COMMAND		"vfx"
#define RT_COMMAND		"rt"
#define P8_COMMAND		"p8"
#define P32_COMMAND		"p32"
#define CP_COMMAND		"cp"
#define SW_COMMAND		"sw"
#define ADC_COMMAND		"adc"
#define CD_COMMAND		"cd"
#define ON_TIMER_COMMAND	"on-timer"
#define OFF_TIMER_COMMAND	"off-timer"
#define ITO_COMMAND			"ito"
#define SET_FLOAT_COMMAND	"sf"
#define WELCOME_COMMAND		"welcome"

// MACROS
#define BIT_SET(a,b) ((a) |= (1ULL<<(b)))
#define BIT_CLEAR(a,b) ((a) &= ~(1ULL<<(b)))
#define BIT_FLIP(a,b) ((a) ^= (1ULL<<(b)))
#define BIT_CHECK(a,b) (!!((a) & (1ULL<<(b))))

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
(byte & 0x80 ? '1' : '0'), \
(byte & 0x40 ? '1' : '0'), \
(byte & 0x20 ? '1' : '0'), \
(byte & 0x10 ? '1' : '0'), \
(byte & 0x08 ? '1' : '0'), \
(byte & 0x04 ? '1' : '0'), \
(byte & 0x02 ? '1' : '0'), \
(byte & 0x01 ? '1' : '0')


