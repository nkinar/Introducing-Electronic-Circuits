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

#define MAX_BYTES_CUSTOM	8
#define MACRON_A_LOC		0x00

void control_display(uint16_t word); 
void set_d0(bool val);
void set_d1(bool val);
void set_d2(bool val);
void set_d3(bool val);
void set_d4(bool val);
void set_d5(bool val);
void set_d6(bool val);
void set_d7(bool val);
void set_rs(bool val);
void set_rw(bool val);
void set_en(bool val);
void setup_display();
void print_hello(); 
void clear_display();
void home_display();
void cursor_second_line();
void print_hello();
bool display_writestr(const char *s, uint8_t line);
void display_putc(uint8_t c);
void display_v_flux(float v, float flux);
void display_t_rh(float temperature, float rh);
bool parse_t_rh(char *s);
void parse_num(char *s, float *v1, float *v2);
bool parse_v_flux(char *s); 
void print_welcome();
void set_char_rom(uint8_t r);
void upload_custom_char(uint8_t map[MAX_BYTES_CUSTOM], uint8_t code);
void upload_a_macron(); 
void upload_custom_chars();
