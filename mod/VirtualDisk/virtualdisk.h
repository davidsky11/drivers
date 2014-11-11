/*
 * =====================================================================================
 *
 *       Filename:  virtualdisk.h
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/11/2014 02:35:50 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kevin (###), kevin101@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#ifndef _VIRTUAL_DISK_H_
#define _VIRTUAL_DISK_H_

#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/mm.h>
#include <linux/sched.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/compat.h>
#include <asm/io.h>
//#include <asm/system.h>
#include <asm/uaccess.h>

#define VIRTUALDISK_SIZE	0x2000		/* 全局内存最大8K字节 */
#define MEM_CLEAR			0x1			/* 全局内存清零 */
#define PORT1_SET			0x2			/* 将port1端口清零 */
#define PORT2_SET			0x3			/* 将port2端口清零 */
#define VIRTUALDISK_MAJOR	200			/* 预设的VirtualDisk的主设备号为200 */

static int VirtualDisk_major = VIRTUALDISK_MAJOR;

/* VirutalDisk设备结构体 */
struct VirtualDisk
{
	struct cdev cdev;
	unsigned char mem[VIRTUALDISK_SIZE];
	int port1;							/* 两个不同类型的端口 */
	long port2;
	long count;							/* 记录设备目前被多少设备打开 */
};

#endif
