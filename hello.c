// SPDX-License-Identifier: GPL-2.0-or-later
#include <linux/init.h>
#include <linux/module.h>
#include <linux/printk.h>
#include <linux/moduleparam.h>
#include <linux/bug.h>
#include <linux/slab.h>

MODULE_AUTHOR("Koval Kostiantyn <koftatfok@gmail.com>");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint print_count = 1;
module_param(print_count, uint, 0644);
MODULE_PARM_DESC(print_count, "Number of times to print 'Hello, world!' (default=1)");

struct hello_message {
	struct list_head list;
	char *message;
};

LIST_HEAD(message_list);

static int __init hello_init(void)
{
	uint i;
	struct hello_message *msg;

	BUG_ON(print_count > 10);

	if (print_count == 0 || (print_count >= 5 && print_count <= 10))
		pr_warn("Warning: print_count is %u\n", print_count);

	for (i = 0; i < print_count; i++) {
		msg = kmalloc(sizeof(*msg), GFP_KERNEL);
		if (!msg)
			return -ENOMEM;

		if (i == print_count - 1) {
			kfree(msg);
			msg = NULL;
			BUG_ON(!msg);
		}

		msg->message = "Hello, world!";
		list_add(&msg->list, &message_list);
		pr_emerg("Hello, world!\n");
	}

	return 0;
}

static void __exit hello_exit(void)
{
	struct hello_message *msg, *tmp;

	list_for_each_entry_safe(msg, tmp, &message_list, list) {
		list_del(&msg->list);
		kfree(msg);
	}
}

module_init(hello_init);
module_exit(hello_exit);
