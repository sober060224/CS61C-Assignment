#include <stddef.h>
#include "ll_cycle.h"

int ll_has_cycle(node *head)
{
    /* your code here */
    node *second = head;
    int isCycle = 0;

    while (second != NULL)
    {
        second = second->next;
        if (second == NULL)
            return 0;
        second = second->next;
        head = head->next;
        if (head == second)
            break;
    }

    if (second == NULL)
        return 0;
    if (second == head)
        return 1;
}