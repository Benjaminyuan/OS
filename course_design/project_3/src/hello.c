#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>

static ssize_t my_read(struct file *, char *, size_t, loff_t *)
{
    return 0;
}
static ssize_t my_write(struct file *, const char *, size_t, loff_t *)
{
    return 0;
}
static int my_open(struct inode *, struct file *)
{
    return 0;
}
static int my_release(struct inode *, struct file *)
{
    return 0;
}
static const sturct file_operations fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .open = my_open,
    .write = my_write,
    .release = my_release,
} static int
hello_init(void)
{
    printk("hello module load ");
    return 0;
}
static void hello_exit(void)
{
    printk("hello module ot ");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benji");
MODULE_DESCRIPTION("hello module");
