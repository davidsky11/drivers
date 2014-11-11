/*
 * =====================================================================================
 *
 *       Filename:  add_sub.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/11/2014 10:36:01 AM
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
#include "include/add_sub.h"

long add_integer(long a, long b)
{
	return a+b;
}

long sub_integer(long a, long b)
{
	return a-b;
}

EXPORT_SYMBOL(add_integer);
EXPORT_SYMBOL(sub_integer);
MODULE_LICENSE("Dual BSD/GPL");

