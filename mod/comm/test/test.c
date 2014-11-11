/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/11/2014 10:39:06 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kevin (###), kevin101@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <linux/init.h>
#include <linux/module.h>
#include "../include/add_sub.h"

static long a = 1;
static long b = 2;
static int AddOrSub = 1;

static int test_init(void)
{
	long result = 0;
	printk(KERN_ALERT "test init\n");
	if (1 == AddOrSub)
	{
		result = add_integer(a, b);
	}
	else
	{
		result = sub_integer(a, b);
	}

	printk(KERN_ALERT "The %s result is %ld", AddOrSub==1 ? "Add" : "Sub", result);
	return 0;
}

static void test_exit(void)
{
	printk(KERN_ALERT "test exit\n");
}

module_init(test_init);
module_exit(test_exit);
module_param(a, long, S_IRUGO);
module_param(b, long, S_IRUGO);
module_param(AddOrSub, int ,S_IRUGO);

/* 描述信息 */
MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("kevin");
MODULE_DESCRIPTION("A module for testing module params and EXPORT_SYMBOL");
MODULE_VERSION("V1.0");
