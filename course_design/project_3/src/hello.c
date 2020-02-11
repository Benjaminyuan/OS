#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/string.h>
#include <linux/uaccess.h>
static char buff[1024] = "Benji's Simple device";
static dev_t  devno;
static struct cdev cdev;
static ssize_t my_read(struct file * fd, char * user_buf, size_t count, loff_t * offset)
{
    size_t avail;
    printk(KERN_INFO "read,buff_len:%d,offset:%d\n",sizeof(buff),*offset);
    avail = sizeof(buff) - *offset;
    if(count <= avail) {
        if(copy_to_user(user_buf,buff+*offset,count) != 0 ){
            return -EFAULT;
        }
        *offset += count;
        return count;
    }else{
        if(copy_to_user(user_buf,buff+*offset,avail) != 0 ){
            return -EFAULT;
        }
        *offset += avail;
        return avail; 
    }
    return 0;
}
static ssize_t my_write(struct file * fd , const char * user_buf, size_t count , loff_t * offset)
{
    size_t avail;
    printk(KERN_INFO "write,buff_len:%d,offset:%d\n",sizeof(buff),*offset);
    avail = sizeof(buff) - *offset;
    memset(buff + *offset, 0, avail);
    if(count <= avail) {
        if(copy_from_user(buff+*offset,user_buf,count) != 0 ){
            return -EFAULT;
        }
        *offset += count;
        return count;
    }else{
        if(copy_from_user(buff+*offset,user_buf,avail) != 0 ){
            return -EFAULT;
        }
        *offset += avail;
        return avail; 
    }
    return 0;
}
static int my_open(struct inode * node, struct file * fd)
{
    printk(KERN_INFO "open\n");

    return 0;
}
static int my_release(struct inode * node , struct file * fd)
{
    printk(KERN_INFO "release\n");
    return 0;
}
static loff_t my_llseek(struct file *fd, loff_t offset, int whence){
    loff_t new_offset;
    printk(KERN_INFO "release\n");
       switch (whence) {
    case 0: /* SEEK_SET */
        new_offset = offset;
        break;
    case 1: /* SEEK_CUR */
        new_offset = fd->f_pos + offset;
        break;
    case 2: /* SEEK_END */
        new_offset = sizeof(buff) + offset;
        break;
    default:
        return -EINVAL;
    }

    if (new_offset < 0) {
        return -EINVAL;
    }

    fd->f_pos = new_offset;
    return new_offset;
}
static const struct file_operations fops = {
    .owner = THIS_MODULE,
    .read = my_read,
    .open = my_open,
    .write = my_write,
    .release = my_release,
    .llseek = my_llseek,
};
static int __init hello_init(void)
{
    int ret;
    printk("hello module load\n");
    devno = MKDEV(111,0);
    ret = register_chrdev_region(devno,1,"hello");
    if(ret < 0 ){
        return ret;
    }
    cdev_init(&cdev,&fops);
    cdev.owner = THIS_MODULE;
    cdev_add(&cdev, devno, 1);
    return 0;

}
static void __exit hello_exit(void)
{
    printk("hello module ot ");
    unregister_chrdev_region(devno, 1);
    cdev_del(&cdev);
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Benji");
MODULE_DESCRIPTION("hello module");
