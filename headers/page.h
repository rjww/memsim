/*

    :: page.h

    author  Robert Woods <hi@robertwoods.me>
            Trek Hopton <trek.hopton@gmail.com>
    course  Operating Systems
    assign  Assignment 2: Virtual Memory Management

*/

#ifndef __PAGE_H__

#include "./memsim.h"

typedef struct
{
    uint PageNumber;
    bool Referenced, Modified;
    uint8 ReferenceBits;
} page;

page *NewPage(uint);

#define __PAGE_H__
#endif