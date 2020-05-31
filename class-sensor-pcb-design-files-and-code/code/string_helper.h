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

bool is_printable(char c);
bool compare_string(const char *s1, const char *s2);
void int_to_str(int num, char *str);
char *trimwhitespace(char *str);
bool compare_string_chars(const char *s1, const char *s2);
void print_uint8_t(uint8_t num);
void print_string_then_number(const char *str, int num);
void print_large_int(int num);
void print_uint16_t(uint16_t num);
void print_uint32_t(uint32_t num);
void print_string_then_fnumber(char *str, double num);
void print_string_then_unsigned_number(const char *str, uint32_t num);
void replace_with_space(char *str);
void print_uint16_t_s_numbers(const char *str, uint32_t k, uint32_t c);
void print_string_then_large_fnumber(const char *str, double num);
void print_true_false(bool val);
void print_string_then_true_false(char *str, bool val);
void print_string_then_string(const char *str0, const char *str1);
char *set_name_tokenize(char *s);
char *get_filename_tokenize(char *s);
void print_string_then_two_fnumber(const char *str, double n1, double n2);