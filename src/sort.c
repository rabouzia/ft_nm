#include "ft_nm.h"

void swap_res(t_res *a, t_res *b)
{
    char *tmp_symbol = a->symbol;
    char tmp_letter = a->letter;
    uint64_t tmp_value = a->addr;

    a->symbol = b->symbol;
    a->letter = b->letter;
    a->addr = b->addr;

    b->symbol = tmp_symbol;
    b->letter = tmp_letter;
    b->addr = tmp_value;
}

int compare_symbols(const char *s1, const char *s2) 
{
	if (!s1 || !s2)
		return 0;
	return strcmp(s1, s2);
}

void ft_nmsort(t_res *head) 
{
    if (!head || !head->next)
		return;
    int swapped;
    t_res *ptr;

    do {
        swapped = 0;
        ptr = head;
        while (ptr->next) {
			// printf("Comparing: %s ---- %s\n", ptr->symbol, ptr->next->symbol);
            if (compare_symbols(ptr->symbol, ptr->next->symbol) > 0) {
   				swap_res(ptr, ptr->next);
   				swapped = 1;
			}			
            ptr = ptr->next;
        }
    } while (swapped);
}

