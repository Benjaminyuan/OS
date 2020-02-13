## 参考
[大概率是某个学长的代码](https://www.lhaihai.wang/post/linux%E5%AD%97%E7%AC%A6%E8%AE%BE%E5%A4%87%E9%A9%B1%E5%8A%A8%E7%A8%8B%E5%BA%8F%E7%BC%96%E5%86%99/)
## 编写file_operations
* 结构
```java
struct file_operations { 
 
    struct module *owner;//拥有该结构的模块的指针，一般为THIS_MODULES 
 
 
    loff_t (*llseek) (struct file *, loff_t, int);//用来修改文件当前的读写位置 
 
    ssize_t (*read) (struct file *, char __user *, size_t, loff_t *);//从设备中同步读取数据 
 
    ssize_t (*write) (struct file *, const char __user *, size_t, loff_t *);//向设备发送数据
 
    ssize_t (*aio_read) (struct kiocb *, const struct iovec *, unsigned long, loff_t);//初始化一个异步的读取操作 
 
    ssize_t (*aio_write) (struct kiocb *, const struct iovec *, unsigned long, loff_t);//初始化一个异步的写入操作 
 
    int (*readdir) (struct file *, void *, filldir_t);//仅用于读取目录，对于设备文件，该字段为NULL 
 
    unsigned int (*poll) (struct file *, struct poll_table_struct *); //轮询函数，判断目前是否可以进行非阻塞的读写或写入 
 
    int (*ioctl) (struct inode *, struct file *, unsigned int, unsigned long); //执行设备I/O控制命令 
 
    long (*unlocked_ioctl) (struct file *, unsigned int, unsigned long); //不使用BLK文件系统，将使用此种函数指针代替ioctl 
 
    long (*compat_ioctl) (struct file *, unsigned int, unsigned long); //在64位系统上，32位的ioctl调用将使用此函数指针代替 
 
 
    int (*mmap) (struct file *, struct vm_area_struct *); //用于请求将设备内存映射到进程地址空间
 
    int (*open) (struct inode *, struct file *); //打开 
 
    int (*flush) (struct file *, fl_owner_t id); 
 
    int (*release) (struct inode *, struct file *); //关闭 
 
    int (*fsync) (struct file *, struct dentry *, int datasync); //刷新待处理的数据 
 
    int (*aio_fsync) (struct kiocb *, int datasync); //异步刷新待处理的数据 
 
    int (*fasync) (int, struct file *, int); //通知设备FASYNC标志发生变化 
 
    int (*lock) (struct file *, int, struct file_lock *); 
 
    ssize_t (*sendpage) (struct file *, struct page *, int, size_t, loff_t *, int); 
 
    unsigned long (*get_unmapped_area)(struct file *, unsigned long, unsigned long, unsigned long, unsigned long); 
 
    int (*check_flags)(int); 
 
    int (*flock) (struct file *, int, struct file_lock *);
 
    ssize_t (*splice_write)(struct pipe_inode_info *, struct file *, loff_t *, size_t, unsigned int);
 
    ssize_t (*splice_read)(struct file *, loff_t *, struct pipe_inode_info *, size_t, unsigned int); 
 
    int (*setlease)(struct file *, long, struct file_lock **); 
 
};
```
* 第一个参数是系统分配的 file指针

## Linux代码树外编译模块
### Makefile
    * 在自定义模块源代码的树目录下建立一个Makefile文件同时添加
    ```makefile
    obj-m += hello.o
    all:
        make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) modules
    clean:
        make -C /lib/modules/$(shell uname -r)/build M=$(shell pwd) clean
    ```
### load模块
> insmod hello.ko
> rmmod hello 卸载设备
> modinfo hello 查看设备信息
### 创建设备文件
> mknod /dev/hello c 111 0命令创建设备文件 另外可以自动创建设备节点
### 调试
> dmesg 查看printk输出