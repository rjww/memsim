/*

    :: memsim.h

    author  Robert Woods <hi@robertwoods.me>
            Trek Hopton <trek.hopton@gmail.com>
    course  Operating Systems
    assign  Assignment 2: Virtual Memory Management

    Libraries, typedefs and #defined constants shared by the program's modules.

*/

#ifndef __MEMSIM_H__

#include <stdint.h>
#include <stdlib.h>

#define global_variable static

typedef uint8_t uint8;
typedef uint32_t uint;

typedef uint8_t bool;
#define true 1
#define false 0

#define LINE_BUFFER_SIZE 32
#define DEBUG_MODE 0

#define __MEMSIM_H__
#endif