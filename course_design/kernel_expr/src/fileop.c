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
    int times = 0;
    int total = 0;
    // ssize_t (*read)(struct file *, char *, size_t, loff_t *);
    // Init the buffer with 0
    ret = 0;
    src = "./input";
    dst = "./output";
   	int  src_len = strlen(src);
	int  dst_len = strlen(dst);
	char src_file[src_len];
	char dst_file[dst_len];

	if(copy_from_user(src_file,src,src_len)){
    	printk(KERN_INFO "copy from src  failed");
	}
	if(copy_from_user(dst_file,dst,dst_len)){
    	printk(KERN_INFO "copy from dst failed");
	}
	memset(buf, 0, 512);
    printk(KERN_INFO "src file: %s,dst file: %s",src_file,dst_file);
	f = filp_open(src_file, O_RDONLY, 0);
	if (f == NULL) {
		printk(KERN_ALERT "open src file  error!!.\n");
		return -1;
	} else if (IS_ERR(f)) {
		printk(KERN_ALERT
		       "error occured while opening src file: %s, exiting...\n",
		       src_file);
		return -2;
	}
	dest = filp_open(dst_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (dest == NULL) {
		printk(KERN_ALERT "open dst file error!!.\n");
		return -1;
	} else if (IS_ERR(dest)) {
		printk(KERN_ALERT
		       "error occured while opening dst file: %s, exiting...\n",
		       dst_file);
		return -2;
	}

	// Get current segment descriptor
	printk(KERN_INFO "filp_open success!!.\n");
	read_size = kernel_read(f, buf, 512 * sizeof(char), &f->f_pos);
	while (read_size > 0) {
		printk(KERN_INFO "read_size:%d,pos:%lld\n", read_size,
		       f->f_pos);
		write_size = kernel_write(dest, buf, read_size, &dest->f_pos);
		printk(KERN_INFO "write_size:%d,pos:%lld\n", write_size,
		       dest->f_pos);
		total += write_size;
		memset(buf, 0, 512);
		read_size = kernel_read(f, buf, 512 * sizeof(char), &f->f_pos);
	}
	filp_close(f, NULL);
	filp_close(dest, NULL);
	return total;
}

void cleanup_module(void)
{
    printk(KERN_INFO "My module is unloaded\n");
}

MODULE_AUTHOR("Benji");
MODULE_DESCRIPTION("file module");
MODULE_LICENSE("GPL");
