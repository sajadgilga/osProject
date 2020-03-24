#include "device_file.h"
#include <linux/fs.h> 	     /* file stuff */
#include <linux/kernel.h>    /* printk() */
#include <linux/errno.h>     /* error codes */
#include <linux/module.h>  /* THIS_MODULE */
#include <linux/cdev.h>      /* char device stuff */
#include <asm/uaccess.h>  /* copy_to_user() */

static ssize_t device_file_read(struct file *file_ptr, char __user *user_buffer, size_t count, loff_t *position) {
	printk( KERN_NOTICE "device file is read at offset = %i, read byters count = %u", (int)*position, (unsigned int) count);
	return count;
}

static struct file_operations driver_ops = {
	.owner = THIS_MODULE,
	.read = device_file_read,
};

static int device_file_major_number = 0;
static const char device_name[] = "OS_phase0_driver";

int register_device(void) {
	int result = 0;
	printk( KERN_NOTICE "register_device() is called." );
	result = register_chrdev(0, device_name, &driver_ops);
	if (result < 0) {
		printk( KERN_WARNING "can\'t register character device with errorcode = %i", result);
		return result;
	}
	device_file_major_number = result;
	printk( KERN_NOTICE "registered device with major number = %i", result);
	return 0;
}

void unregister_device(void) {
	printk( KERN_NOTICE "unregister_device() is called.");
	if (device_file_major_number != 0)
		unregister_chrdev(device_file_major_number, device_name);
}

