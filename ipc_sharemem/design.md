## 环形缓冲区
### 数组index取mod实现
###  write read 的逻辑
* 缓冲区只能有一个进程写，进程写时不能读，读时不能写
* write_index == read_index 时，write进程先启动，read等待