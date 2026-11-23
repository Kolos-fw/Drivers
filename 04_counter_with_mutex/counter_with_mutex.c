#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/device.h>
#include <linux/mutex.h>

#define DEVICE_NAME "counter"
#define BUF_SIZE 256


static int counter = 0;
static struct mutex my_lock;

static dev_t dev_num;
static struct cdev my_cdev;
static struct class *my_class;

static ssize_t my_read(struct file *file, char __user *user_buf, size_t count, loff_t *offset)
{
    char kbuf[32]; 
    int len;

	if (mutex_lock_interruptible(&my_lock)){
		return -ERESTARTSYS;
	}

	if (*offset > 0){
		mutex_unlock(&my_lock);
		return 0;
	}

    counter++; 
    len = snprintf(kbuf, sizeof(kbuf), "Current value: %d\n", counter); 
	
	mutex_unlock(&my_lock);

	return simple_read_from_buffer(user_buf, count, offset, kbuf, len);
}

static ssize_t my_write(struct file *file, const char __user *user_buf, size_t count, loff_t *offset)
{
	if (mutex_lock_interruptible(&my_lock)){
		return -ERESTARTSYS;
	}

    counter = 0;
    *offset = 0;
    printk(KERN_INFO "counter: счетчик сброшен в 0\n");

	mutex_unlock(&my_lock);

    return count;
}

static struct file_operations my_fops = {
	.owner = THIS_MODULE,
	.read = my_read,
	.write = my_write,
};

static int __init counter_init(void)
{
	int ret;

	ret = alloc_chrdev_region(&dev_num, 0, 1, DEVICE_NAME);
	if (ret < 0) return ret;

	cdev_init(&my_cdev, &my_fops);
	my_cdev.owner = THIS_MODULE;

	ret = cdev_add(&my_cdev, dev_num, 1);
	if (ret < 0) return ret;

	my_class = class_create("my_class");

	device_create(my_class, NULL, dev_num, NULL, DEVICE_NAME);

	mutex_init(&my_lock);

	printk(KERN_INFO "counter: устройство /dev/counter создано!\n");
	return 0;
}

static void __exit counter_exit(void)
{
	device_destroy(my_class, dev_num);
	class_destroy(my_class);
	cdev_del(&my_cdev);
	unregister_chrdev_region(dev_num, 1);
	printk(KERN_INFO "counter: устройство удалено, модуль выгружен.\n");
}

module_init(counter_init);
module_exit(counter_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Колосков Никита");
MODULE_DESCRIPTION("Счётчик");
