// SPDX-License-Identifier: GPL-2.0
/*
 * hello2 - calls print_hello() from hello1 'count' times.
 */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/moduleparam.h>

#include "hello1.h"

MODULE_AUTHOR("Shmihel Makar <IO-44>");
MODULE_DESCRIPTION("Lab 5: hello2 - uses print_hello() from hello1");
MODULE_LICENSE("GPL");

static unsigned int count = 1;
module_param(count, uint, 0444);
MODULE_PARM_DESC(count, "How many times to print \"Hello, world!\" (default 1, max 10)");

static int __init hello2_init(void)
{
	unsigned int i;

	if (count == 0)
		pr_warn("hello2: count=%u, continuing anyway\n", count);
	else if (count >= 5 && count <= 10)
		pr_warn("hello2: count=%u, continuing anyway\n", count);
	else if (count > 10) {
		pr_err("hello2: count=%u is too big (max 10)\n", count);
		return -EINVAL;
	}

	for (i = 0; i < count; i++)
		print_hello();

	return 0;
}

static void __exit hello2_exit(void)
{
	pr_info("hello2: unloaded\n");
}

module_init(hello2_init);
module_exit(hello2_exit);
