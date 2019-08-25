**1 一键式编译、链接**  
./onekeybuild.sh  

**2 测试**  
cd build/bin
```
masterserver:  
./masterserver  

client:  
./fs_client mkdir /file1
./fs_client ls /file1
```
启动masterserver进程，client端输入mkdir命令可以创建文件夹存储在文件系统里，通过ls命令可以查询已创建文件夹的元数据信息
