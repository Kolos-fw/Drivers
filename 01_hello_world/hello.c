#include <linux/module.h>
#include <linux/kernel.h>

static int __init hello_init(void)
{
	printk(KERN_INFO "Привет, ядро! Модуль загружен.\n");
	return 0;
}

static void __exit hello_exit(void)
{
	printk(KERN_INFO "Пока, ядро! Модуль выгружен.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Колосков Никита");
MODULE_DESCRIPTION("1-ый модуль ядра");
