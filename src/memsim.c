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

    while (fgets(Line, LINE_BUFFER_SIZE, File) != NULL)
    {
        // Ignore comments.
        if (Line[0] == '#') continue;

        uint Address = strtoul(&Line[2], NULL, 16);
        uint PageNumber = Address / PageSize;
        bool Written = (Line[0] == 'W');

        node *Result = Find(Memory, PageNumber);

        if (Result != NULL)
        {
            if (DEBUG_MODE) printf("HIT:     page %d\n", PageNumber);

            Result->Data->Referenced = true;
            Result->Data->ReferenceBits |= (1 << 7);
            Result->Data->Modified = (Written) ? true : Result->Data->Modified;
        }

        else
        {
            if (DEBUG_MODE) printf("MISS:    page %d\n", PageNumber);

            page *InboundPage = NewPage(PageNumber);
            InboundPage->Modified = Written;
            ++ReadCount;

            node *Victim = Insert(Memory, InboundPage, Algorithm);

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

        ++EventCount;
        if (EventCount % Interval == 0) ShiftARB(Memory);
    }

    printf("events in trace:    %d\n", EventCount);
    printf("total disk reads:   %d\n", ReadCount);
    printf("total disk writes:  %d\n", WriteCount);

    DeleteBuffer(Memory);
    exit(EXIT_SUCCESS);
}
