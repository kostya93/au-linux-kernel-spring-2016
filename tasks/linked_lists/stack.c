#include <linux/slab.h>
#include <linux/gfp.h>
#include "stack.h"

stack_entry_t* create_stack_entry(void *data)
{
    stack_entry_t* entry_ptr = kmalloc(sizeof(stack_entry_t), GFP_KERNEL);
    if (entry_ptr != NULL)
    {
        entry_ptr->data = data;
        INIT_LIST_HEAD(&(entry_ptr->lh));
    }
    return entry_ptr;
}

void delete_stack_entry(stack_entry_t *entry)
{
    list_del(&entry->lh);
    kfree(entry);
}

void stack_push(struct list_head *stack, stack_entry_t *entry)
{
    list_add(&entry->lh, stack);
}

stack_entry_t* stack_pop(struct list_head *stack)
{
    return list_entry(stack->next, stack_entry_t, lh);
}
