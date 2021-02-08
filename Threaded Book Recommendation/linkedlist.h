#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

#include "datacontainer.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct LinkedList
{
	double sim;
	CPS* data;
	struct LinkedList* next;
}NODE;

NODE* create_node(CPS*, double);
void terminate_linked_list(NODE*);
NODE* add(NODE*, NODE*);
NODE* replace(NODE*, NODE*);
void output_linked_list(NODE*);

#endif /*__LINKEDLIST_H__*/
