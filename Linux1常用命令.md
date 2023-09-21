# 1 帮助命令
## man ：获得帮助信息  
man [命令或配置文件]
```
查看ls命令的帮助信息
[root@curry ~]# man ls
```

## help ：获得 shell 内置命令的帮助信息  
help 命令
```
查看cd命令的帮助信息
[root@curry ~]# help cd
```

## 常用快捷键  
ctr + c 停止进程

# 2 文件目录类
## pwd ：显示当前工作目录的绝对路径
pwd: print working directory 打印工作目录
```
显示当前工作目录的绝对路径
[root@curry ~]# pwd
/root
```

## ls ：列出目录的内容
ls:list 列出目录内容
ls [选项] [目录或是文件]
> 选项说明：
> -a 全部文件，包括隐藏档（.开头的文件）一起列出来
> -l 长数据列出，包括文件属性、权限等数据

```
查看当前目录的所有内容信息
[root@curry ~]# ls -al(简写 ll)
```

## cd ：切换目录
cd:Change Directory 切换路径
cd [参数]
> 参数说明：
> cd 绝对路径	 切换路径
> cd 相对路径    切换路径
> cd ~或者cd 	回到自己的家目录
> cd - 		    回到上一次所在目录
> cd .. 		回到当前目录的上一级目录
> cd -P 		跳转到实际物理路径，而非快捷方式路径

## mkdir ：创建一个新的目录
mkdir:Make directory 建立目录
mkdir [选项] 要创建的目录
> 选项说明 ：
-p 创建多层目录

```
创建一个多级目录
[root@curry ~]# mkdir -p xiyou/dssz/meihouwang
```

## rmdir ：删除一个空的目录
rmdir:Remove directory 移除目录
rmdir 要删除的**空目录**
```
删除一个空的文件夹
[root@curry ~]# rmdir xiyou/dssz/meihouwang
```

## touch ：创建空文件
touch 文件名称
```
[root@curry ~]# touch xiyou/dssz/sunwukong.txt
```

## cp ：复制文件或目录
cp [选项] source dest
> 选项说明
> -r 递归复制整个文件夹

强制覆盖不提示的方法：\cp
```
递归复制整个文件夹
[root@curry ~]# cp -r xiyou/dssz/ ./
```

## rm ：删除文件或目录
rm [选项] deleteFile
> 选项说明
> -r 递归删除目录所有内容
> -f 强制执行删除操作，不提示确认
> -v 显示指令的详细执行过程

**不要随便使用 rm -rf /**
```
递归删除目录中所有内容
[root@curry ~]# rm -rf dssz/
```

## mv ：移动文件与目录或重命名
（1）mv oldNameFile newNameFile （功能描述：重命名）
（2）mv /temp/movefile /targetFolder （功能描述：移动文件）

## cat ：查看文件内容
cat [选项] 要查看的文件
> 选项说明
> -n 显示所有行的行号，包括空行。

一般查看比较小的文件，一屏幕能显示全的。
```
查看文件内容并显示行号
[root@curry ~]# cat -n houge.txt
```

## more ：文件内容分屏查看器
more 要查看的文件
> 选项说明
> space空格键	向下翻一页
> enter 回车键    向下翻一行
> q			  退出
> Ctrl+F	    向下滚动一屏
> Ctrl+B 	    返回上一屏

## less ：分屏显示文件内容
less 要查看的文件
> 选项说明
> space空格键	向下翻一页
> pagedown	   向下翻一页
> pageup    	向上翻一页
> / 字串		   向下搜寻字串		
> q			  退出

## echo ：输出内容到控制台
echo [选项] [输出内容]
> 选项说明
> -e	支持反斜线控制的字符转换

## head ：显示文件头部内容
head 文件（功能描述：默认情况查看文件头10行内容）
head -n <数字> 文件（功能描述：查看文件头<数字>行内容）
```
查看文件的头2行
[root@curry ~]# head -n 2 smartd.conf
```

## tail ：输出文件尾部内容
默认情况下tail 指令显示文件的后10 行内容。
> 选项说明
> -n <行数>	输出末尾n行内容
> -f	显示最新追加内容，监视文件变化

## 输出重定向>和 追加>>
\> 	覆盖
\>>	追加

## ln ：软链接
软链接也称为符号链接，主要存放链接其他文件的路径。
ln -s [原文件或目录] [软链接名] （功能描述：给原文件创建一个软链接）
删除软链接： rm -rf 软链接名，而不是rm -rf 软链接名/
如果使用rm -rf 软链接名/ 删除，会把软链接对应的真实目录下内容删掉
```
创建软连接
[root@curry ~]# ln -s xiyou/dssz/houge.txt ./houzi
删除软连接
[root@curry ~]# rm -rf houzi
```

## history ：查看已经执行过历史命令
history （功能描述：查看已经执行过历史命令）

# 3 时间日期类
date [OPTION]... [+FORMAT]
> 选项说明
> -d<时间字符串>	显示指定的时间字符串表示的时间，而非当前时间
> -s<日期时间>		设置系统日期时间

## date 显示当前时间
（1）date 	（功能描述：显示当前时间）
（2）date +%Y （功能描述：显示当前年份）
（3）date +%m （功能描述：显示当前月份）
（4）date +%d （功能描述：显示当前是哪一天）
（5）date "+%Y-%m-%d %H:%M:%S" （功能描述：显示年月日时分秒）

## date 显示非当前时间
（1）date -d '1 days ago' （功能描述：显示前一天时间）
（2）date -d '-1 days ago' （功能描述：显示明天时间）

## date 设置系统时间
date -s 字符串时间
```
设置系统当前时间
[root@hadoop101 ~]# date -s "2023-08-19 20:52:18"
```

## cal ：查看日历
cal [选项] （功能描述：不加选项，显示本月日历）
> 选项： 具体某一年		显示这一年的日历

# 4 用户管理命令
## useradd ：添加新用户
useradd 用户名		（功能描述：添加新用户）
useradd -g 组名用户名（功能描述：添加新用户到某个组）

## passwd ：设置用户密码
passwd 用户名（功能描述：设置用户密码）
```
设置用户的密码
[root@curry ~]# passwd tangseng
```

## id ：查看用户是否存在
id 用户名
```
查看用户是否存在
[root@curry ~]#id tangseng
```

- cat /etc/passwd ：查看创建了哪些用户
- cat /etc/shadow ：查看口令信息

## su ：切换用户
su: swith user 切换用户
su 用户名称（功能描述：切换用户，只能获得用户的执行权限，不能获得环境变量）
su - 用户名称（功能描述：切换到用户并获得该用户的环境变量及执行权限）

## userdel ：删除用户
userdel 用户名	（功能描述：删除用户但保存用户主目录）
userdel -r 用户名（功能描述：用户和用户主目录，都删除）

## who ：查看登录用户信息
whoami （功能描述：显示自身用户名称）
who am i （功能描述：显示登录用户的用户名以及登陆时间）

## sudo ：设置普通用户具有root 权限
修改配置文件	/etc/sudoers
```
## Allow root to run any commands anywhere
root ALL=(ALL) ALL
curry ALL=(ALL) ALL
```

## usermod ：修改用户
usermod -g 用户组  用户名

# 5 用户组管理命令
每个用户都有一个用户组，系统可以对一个用户组中的所有用户进行集中管理。

## groupadd ：新增组
groupadd 组名
```
添加一个xitianqujing组
[root@curry opt]#groupadd xitianqujing
```

## groupdel ：删除组
groupdel 组名
```
删除xitianqujing组
[root@curry opt]# groupdel xitianqujing
```

## groupmod ：修改组
groupmod -n 新组名  老组名

## cat /etc/group ：查看创建了哪些组

# 6 文件权限类
## 文件属性
1）0 首位表示类型
在Linux中第一个字符代表这个文件是目录、文件或链接文件等等
\-  代表文件
d   代表目录
l   链接文档(link file)；
2）第1-3位确定属主（该文件的所有者）拥有该文件的权限。---User
3）第4-6位确定属组（所有者的同组用户）拥有该文件的权限，---Group
4）第7-9位确定其他用户拥有该文件的权限---Other

## chmod ：改变权限
第一种方式变更权限
chmod [{ugoa}{+-=}{rwx}] 文件或目录
```
[root@curry ~]# chmod u+x houge.txt
```

第二种方式变更权限
chmod [mode=421] [文件或目录]
```
[root@curry ~]# chmod 777 houge.txt
```

## chown ：改变所有者
chown [选项] [最终用户] [文件或目录] 
> 选项说明
> -R	递归

## chgrp ：改变所属组
chgrp [最终用户组] [文件或目录]

# 7 搜索查找类
## find ：查找文件或者目录
find [搜索范围] [选项]
> 选项说明
> -name<查询方式>	按照指定的文件名查找模式查找文件
> -user<用户名>	查找属于指定用户名所有文件
> -size<文件大小> 	按照指定的文件大小查找文件

## locate :快速定位文件路径
locate ：搜索文件
由于locate 指令基于数据库进行查询，所以第一次运行前，必须使用updatedb 指令创建locate 数据库。

## grep ：过滤查找及“|”管道符
管道符，“|”，表示将前一个命令的处理结果输出传递给后面的命令处理
grep 选项 	查找内容源文件

# 8 压缩和解压类
## gzip/gunzip ：压缩
gzip 文件		（功能描述：压缩文件，只能将文件压缩为\*.gz 文件）
gunzip 文件.gz （功能描述：解压缩文件命令）

经验技巧
1）只能压缩文件不能压缩目录
2）不保留原来的文件
3）同时多个文件会产生多个压缩包

## zip/unzip ：压缩
zip [选项] XXX.zip 将要压缩的内容	压缩文件和目录的命令
unzip [选项] XXX.zip				解压缩文件
> 选项说明
> -r	压缩目录
> -d	制定解压后文件的存放目录

## tar ：打包
tar [选项] XXX.tar.gz 将要打包进去的内容
> 选项说明
> -c    产生.tar打包文件
> -v    显示详细信息
> -f    制定压缩后文件名
> -z    打包同时压缩
> -x    解包.tar文件
> -C    解压到指定目录

```
压缩目录
[root@curry ~]# tar -zcvf xiyou.tar.gz xiyou/
解压到指定目录
[root@curry ~]# tar -zxvf xiyou.tar.gz -C /opt
```

# 9 磁盘查看和分区类

## du ：查看文件和目录占用的磁盘空间
du: disk usage 磁盘占用情况
du 目录/文件（功能描述：显示目录下每个子目录的磁盘使用情况）
> 选项说明
-h 以人们较易阅读的格式自行显示；
-a 不仅查看子目录大小，还要包括文件
-c 显示所有的文件和子目录大小后，显示总和
-s 只显示总和
--max-depth=n 指定统计子目录的深度为第n 层

## df ：查看磁盘空间使用情况
df: disk free 空余磁盘
df 选项
-h 以人们较易阅读

## lsblk ：查看设备挂载情况
lsblk （功能描述：查看设备挂载情况）

## mount/umount ：挂载/卸载
1）挂载前准备（必须要有光盘或者已经连接镜像文件）
2）基本语法
mount [-t vfstype] [-o options] device dir （功能描述：挂载设备）
umount 设备文件名或挂载点（功能描述：卸载设备）
> 选项说明
-t vfstype 指定文件系统的类型，通常不必指定。mount自动选择正确的类型。
-o options 主要用来描述设备或档案的挂接方式。常用的参数有：
loop：用来把一个文件当成硬盘分区挂接上系统
ro：采用只读方式挂接设备
rw：采用读写方式挂接设备
iocharset：指定访问文件系统所用字符集
device 要挂接(mount)的设备
dir 设备在系统上的挂接点(mount point)

## fdisk ：分区
fdisk -l （功能描述：查看磁盘分区详情）
fdisk 硬盘设备名（功能描述：对新增硬盘进行分区操作）
经验技巧
该命令必须在root 用户下才能使用

