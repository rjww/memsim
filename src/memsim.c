/*

    :: memsim.c

    author  Robert Woods <hi@robertwoods.me>
            Trek Hopton <trek.hopton@gmail.com>
    course  Operating Systems
    assign  Assignment 2: Virtual Memory Management

*/

#include "../headers/memsim.h"
#include "../headers/ring_buffer.h"
#include <stdio.h>

global_variable uint PageSize;
global_variable uint PageCount;
global_variable char *Algorithm;
global_variable uint Interval;

global_variable uint EventCount;
global_variable uint ReadCount;
global_variable uint WriteCount;

int main(int argc, char *argv[])
{
    PageSize = strtoul(argv[2], NULL, 10);
    PageCount = strtoul(argv[3], NULL, 10);
    Algorithm = argv[4];
    if (argc > 5) Interval = strtoul(argv[5], NULL, 10);

    char *Filename = argv[1];
    FILE *File = fopen(Filename, "r");
    char Line[LINE_BUFFER_SIZE];

    if (File == NULL)
    {
        fprintf(stderr, "Couldn't open file %s\n", Filename);
        exit(EXIT_FAILURE);
    }

    ring_buffer *Memory = NewBuffer(PageCount);

    // The main loop of the program. Iterate over the input trace, each line of
    // which represents read or write trace event (if not a blank or comment).
    while (fgets(Line, LINE_BUFFER_SIZE, File) != NULL)
    {
        // Ignore comments.
        if (Line[0] == '#') continue;

        // Calculate the page number corresponding to the referenced memory
        // address as a function of the specified page size. 
        uint Address = strtoul(&Line[2], NULL, 16);
        uint PageNumber = Address / PageSize;

        // Note whether the trace event was a write event.
        bool WriteEvent = (Line[0] == 'W');

        // Look for the referenced page in memory.
        node *Result = Find(Memory, PageNumber);

        // If the result is not null, the page was resident in memory. Note that
        // it was referenced and whether it was modified, then continue.
        if (Result != NULL)
        {
            if (DEBUG_MODE) printf("HIT:     page %d\n", PageNumber);

            Result->Data->Referenced = true;
            Result->Data->ReferenceBits |= (1 << 7);
            Result->Data->Modified = (WriteEvent) ? true : Result->Data->Modified;
        }

        // If the result is null, the page was not resident in memory, and we
        // have a pagefault.
        else
        {
            if (DEBUG_MODE) printf("MISS:    page %d\n", PageNumber);

            // Create a new page struct to represent the referenced page being
            // read from disk, and record the read.
            page *InboundPage = NewPage(PageNumber);
            InboundPage->Modified = WriteEvent;
            ++ReadCount;

            // Insert the page into memory, maybe replacing an existing victim
            // page, which will be returned if applicable. How the victim is
            // selected depends upon the page replacement algorithm specified as
            // the third argument to the Insert() function.
            node *Victim = Insert(Memory, InboundPage, Algorithm);

            // If the victim is not null, then the memory was full, and a victim
            // page was evicted. Note the replacement, and if the victim was
            // modified at some point, record the fact that it was written back
            // to disk. (Also free the structs representing the page and its
            // enclosing node).
            if (Victim != NULL)
            {
                if (DEBUG_MODE) printf("REPLACE: page %d", Victim->Data->PageNumber);

                if (Victim->Data->Modified)
                {
                    if (DEBUG_MODE) printf(" (DIRTY)");
                    ++WriteCount;
                }

                if (DEBUG_MODE) printf("\n");
                free(Victim->Data);
                free(Victim);
            }
        }

        // Record a valid event.
        ++EventCount;

        // If Interval is nonzero, then we are using the ARB or EARB algorithm.
        // If the number of events is a multiple of the specified shift interval,
        // perform a rightward shift on the reference bits of all pages
        // resident in memory.
        if (Interval != 0 && EventCount % Interval == 0) ShiftARB(Memory);
    }

    // Output a summary of the events in the trace.
    printf("events in trace:    %d\n", EventCount);
    printf("total disk reads:   %d\n", ReadCount);
    printf("total disk writes:  %d\n", WriteCount);

    // Free the memory struct and its children, and exit.
    DeleteBuffer(Memory);
    exit(EXIT_SUCCESS);
}
