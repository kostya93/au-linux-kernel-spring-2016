#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/slab.h>

#include "stack.h"
#include "assert.h"

static void __init test_stack(void)
{
    LIST_HEAD(data_stack);
    stack_entry_t *tos = NULL;
    const char *tos_data = NULL;
    const char* test_data[] = { "1", "2", "3", "4" };
    long i = 0;

    pr_alert("Testing basic stack");

    for (i = 0; i != ARRAY_SIZE(test_data); ++i) {
        stack_push(&data_stack,
            create_stack_entry((void*)test_data[i])
        );
    }

    for (i = ARRAY_SIZE(test_data) - 1; i >= 0; --i) {
        tos = stack_pop(&data_stack);
        tos_data = STACK_ENTRY_DATA(tos, const char*);
        delete_stack_entry(tos);
        printk(KERN_ALERT "%s == %s\n", tos_data, test_data[i]);
        assert(!strcmp(tos_data, test_data[i]));
    }

    assert(stack_empty(&data_stack));
}

static int __init print_processes_backwards(void)
{
    LIST_HEAD(stack_of_process);
    stack_entry_t* entry_ps = NULL;
    struct task_struct* task_ps;
    char* str = NULL;
    for_each_process(task_ps)
    {
        str = (char*)kmalloc(TASK_COMM_LEN, GFP_KERNEL);
        if (!str)
        {
            goto error;
        }

        entry_ps = create_stack_entry(str);
        if (!entry_ps)
        {
            goto error;
        }

        get_task_comm(str, task_ps);
        stack_push(&stack_of_process, entry_ps);
    }

    while(!stack_empty(&stack_of_process))
    {
        entry_ps = stack_pop(&stack_of_process);
        str = STACK_ENTRY_DATA(entry_ps, char*);
        printk(KERN_ALERT "%s\n", str);
        delete_stack_entry(entry_ps);
    }
    return -ENOMEM;

error:
    printk(KERN_ALERT "error");
    while(!stack_empty(&stack_of_process))
    {
        entry_ps = stack_pop(&stack_of_process);
        str = STACK_ENTRY_DATA(entry_ps, char*);
        
        delete_stack_entry(entry_ps);
    }
    return -ENOMEM;
}

static int __init ll_init(void)
{
    printk(KERN_ALERT "Hello, linked_lists\n");
    test_stack();
    print_processes_backwards();
    return 0;
}

static void __exit ll_exit(void)
{
    printk(KERN_ALERT "Goodbye, linked_lists!\n");
}

module_init(ll_init);
module_exit(ll_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Linked list exercise module");
MODULE_AUTHOR("Kernel hacker!");
