// SPDX-License-Identifier: GPL-2.0
/*
 * hello1 - exports print_hello(), keeps the list of calls with timestamps.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/ktime.h>

#include "hello1.h"

MODULE_AUTHOR("Shmihel Makar <IO-44>");
MODULE_DESCRIPTION("Lab 5: hello1 - print_hello() with timing list");
MODULE_LICENSE("GPL");

struct hello_item {
	struct list_head list;
	ktime_t time_before;
	ktime_t time_after;
};

static LIST_HEAD(hello_list);

void print_hello(void)
{
	struct hello_item *item;

	item = kmalloc(sizeof(*item), GFP_KERNEL);
	if (!item) {
		pr_err("hello1: no memory for list item\n");
		return;
	}

	item->time_before = ktime_get();
	pr_info("Hello, world!\n");
	item->time_after = ktime_get();

	list_add_tail(&item->list, &hello_list);
}
EXPORT_SYMBOL(print_hello);

static int __init hello1_init(void)
{
	pr_info("hello1: loaded\n");
	return 0;
}

static void __exit hello1_exit(void)
{
	struct hello_item *item, *tmp;
	unsigned int i = 0;

	list_for_each_entry_safe(item, tmp, &hello_list, list) {
		pr_info("hello1: print #%u took %lld ns\n", i++,
			ktime_to_ns(ktime_sub(item->time_after,
					      item->time_before)));
		list_del(&item->list);
		kfree(item);
	}
	pr_info("hello1: unloaded\n");
}

module_init(hello1_init);
module_exit(hello1_exit);
