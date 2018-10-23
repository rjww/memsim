/*

    :: main.c

    author  Robert Woods <hi@robertwoods.me>
            Trek Hopton <trek.hopton@gmail.com>
    course  Operating Systems
    assign  Assignment 2: Virtual Memory Management

    The source files for this program are located under the src/ directory, and
    the makefile included builds against the files located there. However, the
    web submission system specifically looks for .c files located in the root
    directory, so this file includes all source files in a unity build, simply
    to satisfy that requirement.

*/

#include "./src/memsim.c"
#include "./src/page.c"
#include "./src/ring_buffer.c"