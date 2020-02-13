#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/string.h>
#include <linux/sched.h>
#include <linux/dcache.h>
#include <asm/fcntl.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
static int __init copy_init(void){
    int ret;
    int len ;
    int read_size ;
    char buff[512];
    char* src ="/root/OS/course_design/project_2/src/input";
    char* dst ="/root/OS/course_design/project_2/src/output";
    struct file *fp;
    struct file *dst_fp;
    mm_segment_t old_fs;
    printk(KERN_INFO "=====copy_init======");
    len = 2014;
    fp = filp_open(src,O_RDONLY,0644);
    dst_fp = filp_open(dst,O_RDWR|O_CREAT,0644);
    if(IS_ERR(fp)){
        ret = PTR_ERR(fp);
        printk(KERN_INFO "open %s failed,err: %d",src,ret);
        return ret;
    }

    // 为了能够使用内核空间的内存
    old_fs = get_fs();
    set_fs(KERNEL_DS);

    memset(buff, 0, sizeof(buff));
    while((read_size = vfs_read(fp,buff,len,&fp->f_pos)) > 0){
        vfs_write(dst_fp,buff,read_size,&dst_fp->f_pos);
        memset(buff, 0, sizeof(buff));
    }
    if(read_size < 0 ){
       printk(KERN_INFO "vfs read failed\n");
    }
    set_fs(old_fs);
    filp_close(fp,NULL);
    filp_close(dst_fp,NULL);
    return 0;
}

static void __exit copy_exit(void){
    printk(KERN_INFO "=========copy_exit======");
}
module_init(copy_init);
module_exit(copy_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benji");
MODULE_DESCRIPTION("hello module");


