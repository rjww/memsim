/*

    :: ring_buffer.h

    author  Robert Woods <hi@robertwoods.me>
    course  Operating Systems
    assign  Assignment 2: Virtual Memory Management

    Typedefs and utility functions for a circular buffer type, used in the
    program to represent the memory space of a computer system. Also included
    here are page replacement algorithms used when inserting a new node into a
    full buffer.

*/

#ifndef __RING_BUFFER_H__

#include "./memsim.h"
#include "./page.h"

typedef struct node
{
    page *Data;
    struct node *Next, *Previous;
} node;

typedef struct
{
    node *Head, *Tail;
    uint Length;
    uint MaxLength;
} ring_buffer;

node *NewNode(page *);
ring_buffer *NewBuffer(uint);
void DeleteBuffer(ring_buffer *);
node *Find(ring_buffer *, uint);
node *Insert(ring_buffer *, page *, char *);
void Append(ring_buffer *, page *);
node *ReplaceSC(ring_buffer *, page *);
node *ReplaceESC(ring_buffer *, page *);
node *ReplaceARB(ring_buffer *, page *);
node *ReplaceEARB(ring_buffer *, page *);
void ShiftARB(ring_buffer *);

#define __RING_BUFFER_H__
#endif