#include <linux/init.h>
#include <linux/module.h>
#include <linux/interrupt.h>

static irqreturn_t int_12_handler(int irq, void *id)
{
	printk(KERN_WARNING " INTERRUPT_12 \n");
	return 0;
}

static int __init interrupt(void)
{
	int irq;
	ret = request_irq(12 , int_12_handler, IRQF_DISABLED, "INTERRUPT 12", NULL );
	return 0;
}

static void __exit release(void)
{
	free_irq(12, NULL);
}

MODULE_AUTHOR("jerry_liang");
MODULE_DESCRIPTION("catch interrupt 12");
MODULE_LICENSE("GPL");

module_init(interrupt);
module_exit(release);
