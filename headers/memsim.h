/*

    :: memsim.h

    author  Robert Woods <hi@robertwoods.me>
    course  Operating Systems
    assign  Assignment 2: Virtual Memory Management

    Virtual memory simulator, covering four page replacement algorithms:

    1. Second chance (SC): Approximates the least-recently used (LRU) algorithm
       as a FIFO queue in which a reference bit is set on a page whenever it is
       referred to. Pages with unset reference bits are favoured as candidates
       for replacement. These bits are unset as pages are visited in the search
       for a candidate, so that the queue degenerates into FIFO when all bits
       are set.

    2. Enhanced SC (ESC): Like SC, but pages are flagged with an
       ordered pair (a, b), where a indicates that a page has been referenced,
       and b indicates that it has been modified (is dirty). Replacing a dirty
       page results in a write to disk, so the algorithm performs multiple
       passes looking for the first available candidate in one of the following
       classes: (0, 0), (0, 1), (1, 0), (1, 1).

    3. Additional reference bits (ARB): Similar to SC, but each page keeps more
       granular information about how recently it was referenced, in the form
       of an 8-bit reference string (unsigned int). When a page is referenced,
       the MSB of the string is set, and the reference bits of all pages are
       shifted to the right on some defined interval. The preferred candidate
       for replacement is the one with the lowest-valued reference string.

    3. Enhanced ARB (EARB): Like ARB, but includes consideration of whether a
       page is clean in prioritising it for replacement. If all pages are dirty
       or clean, the algorithm performs like ARB. Otherwise, if the ARB-style
       best candidate is dirty, it is replaced only if there isn't another clean
       page resident in memory that has been referenced within three intervals
       of it.

    This header file includes libraries, typedefs and #defined constants shared
    by the program's modules.

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