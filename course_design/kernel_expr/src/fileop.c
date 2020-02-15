#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

int init_module(void)
{
    // Create variables
    char *src;
    char *dst;
    struct file *f;
    struct file *dest;
    char buf[512];
    // mm_segment_t fs;
    int ret;
    int read_size = 0;
    int write_size = 0;
    int total = 0;
	
    ret = 0;
    src = "./input";
    dst = "./output";
	memset(buf, 0, 512);
    printk(KERN_INFO "src file: %s,dst file: %s",src,dst);
	f = filp_open(src, O_RDONLY, 0);
	if (f == NULL) {
		printk(KERN_ALERT "open src file  error!!.\n");
		return -1;
	} else if (IS_ERR(f)) {
		printk(KERN_ALERT
		       "error occured while opening src file: %s, exiting...\n",
		       src);
		return -2;
	}
	dest = filp_open(dst, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (dest == NULL) {
		printk(KERN_ALERT "open dst file error!!.\n");
		return -1;
	} else if (IS_ERR(dest)) {
		printk(KERN_ALERT
		       "error occured while opening dst file: %s, exiting...\n",
		       dst);
		return -2;
	}

	// Get current segment descriptor
	printk(KERN_INFO "filp_open success!!.\n");
	while ((read_size = kernel_read(f, buf, 512 * sizeof(char), &f->f_pos)) > 0) {
		printk(KERN_INFO "read_size:%d,pos:%lld\n", read_size,
		       f->f_pos);
		write_size = kernel_write(dest, buf, read_size, &dest->f_pos);
		printk(KERN_INFO "write_size:%d,pos:%lld\n", write_size,
		       dest->f_pos);
		total += write_size;
		memset(buf, 0, 512);
	}
	filp_close(f, NULL);
	filp_close(dest, NULL);
	return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "My module is unloaded\n");
}

MODULE_AUTHOR("Benji");
MODULE_DESCRIPTION("file module");
MODULE_LICENSE("GPL");
