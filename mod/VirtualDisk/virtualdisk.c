/*
 * =====================================================================================
 *
 *       Filename:  virtualdisk.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/11/2014 02:43:30 PM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kevin (###), kevin101@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include "virtualdisk.h"

static void VirtualDisk_setup_cdev(struct VirtualDisk *dev, int minor);
int VirtualDisk_open(struct inode *inode, struct file *filp);
int VirtualDisk_release(struct inode *inode, struct file *filp);
static ssize_t VirtualDisk_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos);
static ssize_t VirtualDisk_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos);
static loff_t VirtualDisk_llseek(struct file *filp, loff_t offset, int orig);
static long VirtualDisk_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);

static const struct file_operations VirtualDisk_fops;

struct VirtualDisk *vd_devp;

int devno;

/* 初始化并注册cdev */
static void VirtualDisk_setup_cdev(struct VirtualDisk *dev, int minor)
{
	int err;
	devno = MKDEV(VirtualDisk_major, minor);		
	cdev_init(&dev->cdev, &VirtualDisk_fops);
	dev->cdev.owner = THIS_MODULE;
	dev->cdev.ops = &VirtualDisk_fops;
	err = cdev_add(&dev->cdev, devno, 1);		/* 将cdev注册到系统中 */
	if (err)
		printk(KERN_NOTICE "Error in cdev_add()\n");
}

/* 文件操作结构体 */
static const struct file_operations VirtualDisk_fops = 
{
	.owner = THIS_MODULE,
	.llseek = VirtualDisk_llseek,
	.read = VirtualDisk_read,
	.write = VirtualDisk_write,
	.compat_ioctl = VirtualDisk_ioctl,
	.open = VirtualDisk_open,
	.release = VirtualDisk_release,
};

/* 文件打开函数 */
int VirtualDisk_open(struct inode *inode, struct file *filp)
{
	struct VirtualDisk *devp;
	/* 将设备结构体指针赋值给文件私有数据指针 */
	filp->private_data = vd_devp;
	devp = filp->private_data;
	devp->count++;
	return 0;
}

/* 文件释放函数 */
int VirtualDisk_release(struct inode *inode, struct file *filp)
{
	struct VirtualDisk *devp = filp->private_data;
	devp->count--;
	return 0;
}

/* 读函数 */
static ssize_t VirtualDisk_read(struct file *filp, char __user *buf, size_t size, loff_t *ppos)
{
	unsigned long p = *ppos;			/* 记录文件指针偏移位置 */
	unsigned int count = size;			/* 记录需要读取的字节数 */
	int ret = 0;						/* 返回值 */
	struct VirtualDisk *devp = filp->private_data;
										/* 获得设备结构体指针 */
	/* 分析和获取有效的读长度 */
	if (p >= VIRTUALDISK_SIZE)
		return count ? - ENXIO : 0;
	if (count > VIRTUALDISK_SIZE - p)
		count = VIRTUALDISK_SIZE - p;

	/* 内核空间 -> 用户空间 交换数据 */
	if (copy_to_user(buf, (void *)(devp->mem + p), count))
	{
		ret = - EFAULT;
	}
	else
	{
		*ppos += count;
		ret = count;
		printk(KERN_INFO "read %d bytes(s) from %ld\n", count, p);
	}
	return ret;
}

/* 写函数 */
static ssize_t VirtualDisk_write(struct file *filp, const char __user *buf, size_t size, loff_t *ppos)
{
	unsigned long p = *ppos;
	int ret = 0;
	unsigned int count = size;
	struct VirtualDisk *devp = filp->private_data;

	/* 分析和获取有效的写长度 */
	if (p >= VIRTUALDISK_SIZE)
		return count ? - ENXIO : 0;
	if (count > VIRTUALDISK_SIZE - p)
		count = VIRTUALDISK_SIZE - p;

	/* 用户空间 -> 内核空间 */
	if (copy_from_user(devp->mem + p, buf, count))
		ret = - EFAULT;
	else
	{
		*ppos += count;
		ret = count;
		printk(KERN_INFO "writenn %d bytes(s) from %ld\n", count, p);
	}

	return ret;
}

/* seek文件定位函数 */
static loff_t VirtualDisk_llseek(struct file *filp, loff_t offset, int orig)
{
	loff_t ret = 0;
	switch (orig)
	{
		case SEEK_SET:
			if (offset < 0)
			{
				ret = - EINVAL;
				break;
			}
			if ((unsigned int)offset > VIRTUALDISK_SIZE)
			{
				ret = - EINVAL;
				break;
			}
			filp->f_pos = (unsigned int)offset;
			ret = filp->f_pos;
			break;
		case SEEK_CUR:
			if ((filp->f_pos + offset) > VIRTUALDISK_SIZE)
			{
				ret = - EINVAL;
				break;
			}
			if ((filp->f_pos + offset) < 0)
			{
				ret = - EINVAL;
				break;
			}
			filp->f_pos += offset;
			ret = filp->f_pos;
			break;

		default:
			ret = - EINVAL;
			break;
	}

	return ret;
}

/* ioctl设备控制函数 */
static long VirtualDisk_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct VirtualDisk *devp = filp->private_data;

	switch (cmd)
	{
		case MEM_CLEAR:
			memset(devp->mem, 0, VIRTUALDISK_SIZE);
			printk(KERN_INFO "VirtualDisk is set to zero\n");
			break;

		case PORT1_SET:
			devp->port1 = 0;
			break;

		case PORT2_SET:
			devp->port2 = 0;
			break;

		default:
			return - EINVAL;
	}

	return (long)0;
}

/* 设备驱动模块加载函数 */
int VirtualDisk_init(void)
{
	int result;
	dev_t devno = MKDEV(VirtualDisk_major, 0);		/* 构建设备号 */
	/* 申请设备号 */
	if (VirtualDisk_major)
		result = register_chrdev_region(devno, 1, "VirtualDisk");
	else	/* 动态申请设备号 */
	{
		result = alloc_chrdev_region(&devno, 0, 1, "VirtualDisk");
		VirtualDisk_major = MAJOR(devno);
	}

	if (result < 0)
		return result;

	/* 动态申请设备结构体的内存 */
	vd_devp = kmalloc(sizeof(struct VirtualDisk), GFP_KERNEL);
	if (!vd_devp)		/* 申请失败 */
	{
		result = - ENOMEM;
		goto fail_kmalloc;
	}
	memset(vd_devp, 0, sizeof(struct VirtualDisk));

	/* 初始化并且添加cdev结构体 */
	VirtualDisk_setup_cdev(vd_devp, 0);
	return 0;

fail_kmalloc:
	unregister_chrdev_region(devno, 1);
	return result;
}

/* 模块卸载函数 */
void VirtualDisk_exit(void)
{
	cdev_del(&vd_devp->cdev);			/* 注销cdev */
	kfree(vd_devp);						/* 释放设备结构体内存 */
	unregister_chrdev_region(MKDEV(VirtualDisk_major, 0), 1);
										/* 释放设备号 */
}
