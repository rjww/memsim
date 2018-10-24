/*

    :: page.h

    author  Robert Woods <hi@robertwoods.me>
    course  Operating Systems
    assign  Assignment 2: Virtual Memory Management

    A simple struct representing a single page in virtual memory. PageNumber is
    calculated before the page is created, as a function of some specified
    page size. The struct includes fields representing reference and modified
    bits treated separately, and an 8-bit unsigned integer representing a flag
    sequence for recording more granular information about how recently a page
    was referenced.

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