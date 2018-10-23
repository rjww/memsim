/*

    :: ring_buffer.c

    author  Robert Woods <hi@robertwoods.me>
            Trek Hopton <trek.hopton@gmail.com>
    course  Operating Systems
    assign  Assignment 2: Virtual Memory Management

*/

#include "../headers/ring_buffer.h"
#include <stdio.h>
#include <string.h>

node *NewNode(page *Page)
{
    node *Node = malloc(sizeof(node));

    Node->Data = Page;
    Node->Next = NULL;
    Node->Previous = NULL;

    return Node;
}

ring_buffer *NewBuffer(uint MaxLength)
{
    ring_buffer *Buffer = malloc(sizeof(ring_buffer));

    Buffer->Head = NULL;
    Buffer->Tail = NULL;
    Buffer->Length = 0;
    Buffer->MaxLength = MaxLength;
    
    return Buffer;
}

void DeleteBuffer(ring_buffer *Buffer)
{
    node *Current = Buffer->Head;
    uint Count = 0;

    while (Count < Buffer->Length)
    {
        node *ToDelete = Current;
        Current = Current->Next;
        free(ToDelete->Data);
        free(ToDelete);
        ++Count;
    }

    free(Buffer);
}

node *Find(ring_buffer *Buffer, uint PageNumber)
{
    if (Buffer->Length == 0) return NULL;

    node *Current = Buffer->Head;
    uint Count = 0;

    while (Count < Buffer->Length)
    {
        if (Current->Data->PageNumber == PageNumber) return Current;
        Current = Current->Next;
        ++Count;
    }

    return NULL;
}

node *Insert(ring_buffer *Buffer, page *Page, char *Algorithm)
{
    node *Victim = NULL;

    if (Buffer->Length < Buffer->MaxLength)
    {
        Append(Buffer, Page);
        return NULL;
    }
    else
    {
        if      (strcmp("SC", Algorithm) == 0)   Victim = ReplaceSC(Buffer, Page);
        else if (strcmp("ESC", Algorithm) == 0)  Victim = ReplaceESC(Buffer, Page);
        else if (strcmp("ARB", Algorithm) == 0)  Victim = ReplaceARB(Buffer, Page);
        else if (strcmp("EARB", Algorithm) == 0) Victim = ReplaceEARB(Buffer, Page);
        else
        {
            fprintf(stderr, "Unrecognised algorithm: %s\n", Algorithm);
            exit(EXIT_FAILURE);
        }
    }

    return Victim;
}

void Append(ring_buffer *Buffer, page *Page)
{
    node *Appendee = NewNode(Page);

    if (Buffer->Length == 0)
    {
        Appendee->Next = Appendee;
        Appendee->Previous = Appendee;
        Buffer->Head = Appendee;
        Buffer->Tail = Appendee;
        ++Buffer->Length;
    }
    else
    {
        Buffer->Tail->Next = Appendee;
        Appendee->Previous = Buffer->Tail;
        Appendee->Next = Buffer->Head;
        Buffer->Head->Previous = Appendee;
        Buffer->Tail = Appendee;
        ++Buffer->Length;
    }
}

node *ReplaceSC(ring_buffer *Buffer, page *Page)
{
    node *Current = Buffer->Head;

    // Step through the buffer, looking for a page that hasn't been referenced.
    // As we go, we unset the reference value for each page we visit, so that in
    // the case that there isn't an unreferenced page, a victim is selected on a
    // FIFO basis.
    while (Current->Data->Referenced)
    {
        Current->Data->Referenced = false;
        Current = Current->Next;
    }

    node *Victim = Current;
    node *Replacement = NewNode(Page);

    // Replace the victim in the buffer, and move the pointer into position
    // after it (by setting the buffer's head and tail around it).
    Victim->Previous->Next = Replacement;
    Replacement->Previous = Victim->Previous;
    Replacement->Next = Victim->Next;
    Replacement->Next->Previous = Replacement;
    Buffer->Head = Replacement->Next;
    Buffer->Tail = Replacement;

    // Unlink and return the victim.
    Victim->Next = NULL;
    Victim->Previous = NULL;
    return Victim;
}

node *ReplaceESC(ring_buffer *Buffer, page *Page)
{
    node *Current = NULL;
    node *Victim = NULL;
    uint Count;

    // First pass. Check for (0, 0) -- a page that has been neither recently
    // used nor modified.

    Current = Buffer->Head;
    Count = 0;

    while (Victim == NULL && Count < Buffer->Length)
    {
        if (!Current->Data->Referenced && !Current->Data->Modified) Victim = Current;

        Current = Current->Next;
        ++Count;
    }

    // Second pass. Check for (0, 1) -- a page that has not been recently used,
    // but has been modified (i.e. is dirty). As we go, we unset the reference
    // value for each page we visit, as in the standard second-chance algorithm.

    Current = Buffer->Head;
    Count = 0;

    while (Victim == NULL && Count < Buffer->Length)
    {
        if (!Current->Data->Referenced) Victim = Current;

        Current->Data->Referenced = false;
        Current = Current->Next;
        ++Count;
    }

    // Third pass. Check for (1, 0) -- a page that has been recently used but
    // is clean. Since all reference values have been unset by this point, we
    // actually just look for the first clean page.

    Current = Buffer->Head;
    Count = 0;

    while (Victim == NULL && Count < Buffer->Length)
    {
        if (!Current->Data->Modified) Victim = Current;

        Current = Current->Next;
        ++Count;
    }

    // Fourth pass. Check for (1, 1) -- a page that has been recently used and
    // is dirty, the last resort option. By this stage, all reference values
    // have been unset, and no clean pages have been found, so we just select
    // the head node directly.

    if (Victim == NULL) Victim = Buffer->Head;

    node *Replacement = NewNode(Page);

    // Replace the victim in the buffer, and move the pointer into position
    // after it (by setting the buffer's head and tail around it).
    Victim->Previous->Next = Replacement;
    Replacement->Previous = Victim->Previous;
    Replacement->Next = Victim->Next;
    Victim->Next->Previous = Replacement;
    Buffer->Head = Replacement->Next;
    Buffer->Tail = Replacement;

    // Unlink and return the victim.
    Victim->Next = NULL;
    Victim->Previous = NULL;
    return Victim;
}

node *ReplaceARB(ring_buffer *Buffer, page *Page)
{
    node *Current = Buffer->Head;
    node *BestCandidate = Buffer->Head;
    uint Count = 0;

    // Look for the node in the buffer whose corresponding page has the lowest-
    // valued reference bits. These are right-shfited as time passes since the
    // page was last referenced, so the lowest value indicates the page that
    // has been referenced least recently (within the margin of error dictated
    // by the shift interval).
    while (Count < Buffer->Length)
    {
        uint8 LowestPriority = BestCandidate->Data->ReferenceBits;
        uint8 CurrentPriority = Current->Data->ReferenceBits;
        if (CurrentPriority < LowestPriority) BestCandidate = Current;

        Current = Current->Next;
        ++Count;
    }

    node *Victim = BestCandidate;

    // Remove victim from the buffer.
    Victim->Previous->Next = Victim->Next;
    Victim->Next->Previous = Victim->Previous;
    if (Victim == Buffer->Head) Buffer->Head = Victim->Next;
    if (Victim == Buffer->Tail) Buffer->Tail = Victim->Previous;
    --Buffer->Length;

    // Add replacemnt to the tail of the buffer.
    Append(Buffer, Page);

    // Unlink and return the victim.
    Victim->Next = NULL;
    Victim->Previous = NULL;
    return Victim;
}

node *ReplaceEARB(ring_buffer *Buffer, page *Page)
{
    node *Current, *BestCandidate;
    uint Count;

    // First pass. Check if page dirtiness is consistent across the buffer (that
    // is, whether all pages are either modified or unmodified). Also find the
    // best candidate for replacement as per the standard ARB algorithm.

    Current = Buffer->Head;
    BestCandidate = Current;
    bool SameDirtiness = true;
    bool Dirtiness = Current->Data->Modified;
    Count = 0;

    while (Count < Buffer->Length)
    {
        uint8 LowestPriority = BestCandidate->Data->ReferenceBits;
        uint8 CurrentPriority = Current->Data->ReferenceBits;
        if (CurrentPriority < LowestPriority) BestCandidate = Current;
        if (Current->Data->Modified != Dirtiness) SameDirtiness = false;

        Current = Current->Next;
        ++Count;
    }

    // If the dirtiness of pages isn't the same across the buffer, and the best
    // candidate for replacement is dirty, we need to make a second pass to look
    // for an appropriate clean replacement candidate.
    if (!SameDirtiness && BestCandidate->Data->Modified)
    {
        Current = Buffer->Head;
        Count = 0;

        // Second pass. Look for clean pages, and test their biased reference
        // bits (right-shifted by 3) against the current (dirty) candidate. If
        // the biased value of a clean page's reference bits is less than or
        // equal to that of the dirty candidate's, the clean page becomes the
        // new best candidate. After that, we continue to look for another clean
        // candidate with a lower reference value than the current one, in the
        // usual ARB fashion.
        while (Count < Buffer->Length)
        {
            if (!Current->Data->Modified && BestCandidate->Data->Modified)
            {
                uint8 LowestPriority = BestCandidate->Data->ReferenceBits;
                uint8 WeightedPriority = (Current->Data->ReferenceBits >> 3);
                if (WeightedPriority <= LowestPriority)
                {
                    BestCandidate = Current;
                }
            }

            else if (!Current->Data->Modified && !BestCandidate->Data->Modified)
            {
                uint8 LowestPriority = BestCandidate->Data->ReferenceBits;
                uint8 CurrentPriority = Current->Data->ReferenceBits;
                if (CurrentPriority < LowestPriority) BestCandidate = Current;
            }

            Current = Current->Next;
            ++Count;
        }
    }

    node *Victim = BestCandidate;

    // Remove victim from the buffer.
    Victim->Previous->Next = Victim->Next;
    Victim->Next->Previous = Victim->Previous;
    if (Victim == Buffer->Head) Buffer->Head = Victim->Next;
    if (Victim == Buffer->Tail) Buffer->Tail = Victim->Previous;
    --Buffer->Length;

    // Add replacemnt to the tail of the buffer.
    Append(Buffer, Page);

    // Unlink and return the victim.
    Victim->Next = NULL;
    Victim->Previous = NULL;
    return Victim;
}

void ShiftARB(ring_buffer *Buffer)
{
    node *Current = Buffer->Head;
    uint Count = 0;

    // Step through each node in the buffer, and right-shfit the reference bits
    // of its corresponding page.
    while (Count < Buffer->Length)
    {
        Current->Data->ReferenceBits >>= 1;
        Current = Current->Next;
        ++Count;
    }
}
