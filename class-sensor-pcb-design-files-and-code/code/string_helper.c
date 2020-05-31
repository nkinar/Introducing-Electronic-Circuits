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

#include "string_helper.h"
#include "main.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include "constants.h"


//----------------------------------
#ifdef DEBUG_BOARD
//----------------------------------

struct sh_data
{
    char str[32];
    
} sh_data;


/** 
 * Checks to see if a character is non-printable (ASCII)
 * @param c             as the character
 * @return              true or false
 */
 bool is_printable(char c)
 {
     if((c < 32) || (c > 126))
         return false;
    // else
    return true;
 } // end
 
 
 /**
  * Function to compare string
  * @param s1 as the first string
  * @param s2 as the second string
  * @return true if the strings are equal
  */
 bool compare_string(const char *s1, const char *s2)
 {
      if(strcmp(s1,s2)==0)
          return true; // the strings compare
     // else
      return false;
 } // end


 /**
  * Compare the first n characters of the string
  * @param s1
  * @param s2
  * @return
  */
 bool compare_string_chars(const char *s1, const char *s2)
 {
     int n = strlen(s2); // compare only n chars of second string
     if(strncmp(s1,s2,n)==0)
          return true; // the strings compare
     // else
     return false;
 } // end


 /**
  * Converts an int to a string by the use of sprintf
  * @param num as the number to convert
  * @param str as the string
  */
 void int_to_str(int num, char *str)
 {
     sprintf(str,"%d", num);
 }

/**
 * Print a uint8_t to the debug uart
 * @param num
 */
 void print_uint8_t(uint8_t num)
 {
     sprintf(sh_data.str, "%d", num);
     print_uart(sh_data.str);
 } // end
 
 /**
  * Print larger int
  * @param num
  */
 void print_large_int(int num)
 {
     sprintf(sh_data.str, "%d", num);
     print_uart(sh_data.str); 
 } // end 

 /**
  * Print uint16_t
  * @param num      as the number to be printed 
  */
  void print_uint16_t(uint16_t num)
 {
     sprintf(sh_data.str, "%u", num);
     print_uart(sh_data.str); 
 } // end
 
/**
 * Print uint32_t
 * @param num   as the number
 */
void print_uint32_t(uint32_t num)
{
     sprintf(sh_data.str, "%lu", num); 
     print_uart(sh_data.str);
 } // end



 // REFERENCE:
 // http://stackoverflow.com/questions/122616/how-do-i-trim-leading-trailing-whitespace-in-a-standard-way
 // Note: This function returns a pointer to a substring of the original string.
// If the given string was allocated dynamically, the caller must not overwrite
// that pointer with the returned value, since the original pointer must be
// deallocated using the same allocator with which it was allocated.  The return
// value must NOT be deallocated using free() etc.
char *trimwhitespace(char *str)
{
  char *end;

  // Trim leading space
  while(isspace(*str)) str++;

  if(*str == 0)  // All spaces?
    return str;

  // Trim trailing space
  end = str + strlen(str) - 1;
  while(end > str && isspace(*end)) end--;

  // Write new null terminator
  *(end+1) = 0;

  return str;
}

/**
 * Print a string and then a number
 * @param str       as the string to print 
 * @param num       as the number to print after the string
 */
void print_string_then_number(const char *str, int num)
{
    sprintf(sh_data.str, "%s%d", str, num);
    print_uart(sh_data.str);
} // end


/*
 * Print string then string
 */
void print_string_then_string(const char *str0, const char *str1)
{
    sprintf(sh_data.str, "%s%s", str0, str1);
    print_uart(sh_data.str);
} // end


/*
Print a string then an unsigned number. 
*/
void print_string_then_unsigned_number(const char *str, uint32_t num)
{
	 sprintf(sh_data.str, "%s %lu", str, num);
	 print_uart(sh_data.str); 
} // end 


/*
 * Print string then some numbers
 */
void print_uint16_t_s_numbers(const char *str, uint32_t k, uint32_t c)
{
    sprintf(sh_data.str, "%s%lu%lu", str, k, c);
    print_uart(sh_data.str);
} // end


/**
 * Print a string and then a floating point number
 * @param str       as the string to print
 * @param num       as the double number
 */
void print_string_then_fnumber(char *str, double num)
{
    sprintf(sh_data.str, "%s %lf", str, num);
    print_uart(sh_data.str);
} // end


/*
 * Print string then two floating point numbers
 */
void print_string_then_two_fnumber(const char *str, double n1, double n2)
{
    sprintf(sh_data.str, "%s %lf %lf", str, n1, n2);
    print_uart(sh_data.str);
} // end


void print_string_then_large_fnumber(const char *str, double num)
{
    sprintf(sh_data.str, "%s %0.10lf", str, num);
    print_uart(sh_data.str);
}


/**
 * Replace _ in the string with a space
 * @param str
 */
void replace_with_space(char *str)
{
    int len = strlen(str);
    int i;
    for(i = 0; i < len; i++)
    {
        if(str[i]=='_')
            str[i] = ' ';
    }
} // end



/*
 * Print true/false
 */
void print_true_false(bool val)
{
   if(val) print_uart(TRUE_STRING);
   else print_uart(FALSE_STRING);
} // end


/*
 * Print string then true/false
 */
void print_string_then_true_false(char *str, bool val)
{
    print_uart(str);
    print_uart(val ? TRUE_STRING : FALSE_STRING);
} // end



//----------------------------------
#endif
//----------------------------------



