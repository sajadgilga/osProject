#include "device_file.h"
#include <linux/fs.h> 	     /* file stuff */
#include <linux/kernel.h>    /* printk() */
#include <linux/errno.h>     /* error codes */
#include <linux/module.h>  /* THIS_MODULE */
#include <linux/cdev.h>      /* char device stuff */
#include <asm/uaccess.h>  /* copy_to_user() */
#include <linux/uaccess.h>
#include <linux/slab.h> 
#define wnum write_num_to_string_arr

static char *fib_arr;
static long fib_size;
static char number[8] = "4";

void write_num_to_string_arr(int num, int *offset) {
	int leap = 6;
	char buf[leap];
	snprintf(buf, leap, "%-5d ", num);
	int j;
	for (j = 0; j < leap; j++) 
		fib_arr[*offset + j] = buf[j];
 	*offset += leap;
}

void calculate_fib(int num) {
	fib_arr = (char*) kmalloc(num * 6 + 2, GFP_KERNEL);
	int offset = 0;
    	int fib[num + 2];
    	fib[0] = 0;
   	//write_num_to_string_arr(fib[0], &offset, leap);
    	fib[1] = 1;
	wnum(fib[1], &offset);
	int i;
	for (i = 2; i <= num; i++) {
        	fib[i] = fib[i - 1] + fib[i - 2];
		wnum(fib[i], &offset);
    	}
	fib_arr[num*6] = '\n';
	fib_arr[num*6 + 1] = '\0';
    //	return fib[num];
}


static ssize_t device_file_read(struct file *file_ptr, char __user *user_buffer, size_t count, loff_t *position) {
	kstrtol(number, 10, &fib_size);
	calculate_fib(fib_size);
	fib_size = fib_size * 6 + 2;
	if (*position >= fib_size)
		return 0;
	if (count + *position > fib_size)
		count = fib_size - *position;
	copy_to_user(user_buffer, fib_arr + *position, count);
	*position += count;
	return count;
}

static ssize_t device_file_write(struct file *file_ptr, const char *user_buffer, size_t count, loff_t *position) {
	if (*position >= 8) 
		return 0;
	if (count + *position > 8)
		count = 8 - *position;
	copy_from_user(number + *position, user_buffer, count);
	*position += count;
	return count;
}

static struct file_operations driver_ops = {
	.owner = THIS_MODULE,
	.read = device_file_read,
	.write = device_file_write,
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

