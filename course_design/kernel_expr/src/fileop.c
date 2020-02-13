#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

int init_module(void)
{
    // Create variables
    struct file *f;
    struct file *dest;
    char buf[128];
    // mm_segment_t fs;
    int i;
    int ret;
    char * src ;
    char * dst ;
    int read_size=0;
    int write_size=0;
    int times= 0;
    // ssize_t (*read)(struct file *, char *, size_t, loff_t *);
    // Init the buffer with 0
    ret = 0;
    src =  "/root/OS/course_design/kernel_expr/src/input";
    dst =  "/root/OS/course_design/kernel_expr/src/output";
    for(i=0;i<128;i++)
        buf[i] = 0;
    f = filp_open(src, O_RDONLY, 0);
    dest = filp_open(dst,O_WRONLY|O_CREAT|O_TRUNC,0600);
    if(f == NULL || dest == NULL ){  
        printk(KERN_ALERT "filp_open error!!.\n");
        return -1;
    }else if(IS_ERR(f) || IS_ERR(dest)){
        printk("error occured while opening file %s, exiting...\n", src);
        return 0;
    }

    // Get current segment descriptor
    printk(KERN_INFO "filp_open success!!.\n");
    read_size = kernel_read(f,buf,128*sizeof(char),&f->f_pos);
    times++;
    while (read_size>0 && times <5 )
    {

        printk(KERN_INFO "read_size:%d,pos:%d\n",read_size,f->f_pos);
        write_size = kernel_write(dest,buf,read_size,&dest->f_pos);
        printk(KERN_INFO "write_size:%d,pos:%d\n",write_size,dest->f_pos);
        memset(buf,0,128);
        times++;
        read_size = kernel_read(f,buf,128*sizeof(char),&f->f_pos);
    }
    filp_close(f,NULL);
    filp_close(dest,NULL);
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_INFO "My module is unloaded\n");
}


MODULE_AUTHOR("Benji");
MODULE_DESCRIPTION("file module");
MODULE_LICENSE("GPL");
