/*

    :: page.c

    author  Robert Woods <hi@robertwoods.me>
            Trek Hopton <trek.hopton@gmail.com>
    course  Operating Systems
    assign  Assignment 2: Virtual Memory Management

*/

#include "../headers/page.h"

page *NewPage(uint PageNumber)
{
    page *Page = malloc(sizeof(page));

    Page->PageNumber = PageNumber;
    Page->Referenced = true;
    Page->ReferenceBits = (1 << 7);
    Page->Modified = false;

    return Page;
}
