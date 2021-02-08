#include "linkedlist.h"

NODE* create_node(CPS* data, double sim)
{
	NODE* created = (NODE*)calloc(1, sizeof(NODE));
	created->data = data;
	created->sim = sim;
	created->next = NULL;
	return created;
}

void terminate_linked_list(NODE* head)
{
	if (head->next != NULL) { terminate_linked_list(head->next); }
	free(head);
}

NODE* add(NODE* head, NODE* addon)
{
	if (head == NULL)
	{
		return addon;
	}

	NODE* curr = head;
	while (curr->next != NULL)
	{
		curr = curr->next;
	}
	curr->next = addon;
	return head;
}

NODE* replace(NODE* head, NODE* addon)
{
	if (head == NULL)
	{
		return addon;
	}
	if (addon == NULL)
	{
		return head;
	}

	int control = 1;
	NODE* curr = head;

	//	Check with the head.
	if (addon->sim > head->sim)
	{
		addon->next = head->next;
		head->next = NULL;
		terminate_linked_list(head);
		return addon;
	}

	//	Find a less similar current and replace.
	while (curr->next != NULL && control)
	{
		if (addon->sim > curr->next->sim)
		{
			addon->next = curr->next->next;
			curr->next->next = NULL;
			terminate_linked_list(curr->next);
			curr->next = addon;
			control = 0;
		}
		else
		{
			curr = curr->next;
		}
	}

	//	This is the control for not being able to insert addon.
	if (control == 1)
	{
		terminate_linked_list(addon);
	}

	return head;
}

void output_linked_list(NODE* head)
{
	NODE* curr = head;
	while (curr != NULL)
	{
		printf("%s : %f\n", curr->data->name, curr->sim);
		curr = curr->next;
	}
	printf("\n\n");
}
