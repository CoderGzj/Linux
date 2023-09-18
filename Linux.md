# Linux 学习
第一阶段：linux 基本操作命令（文件操作命令、编辑工具使用，linux 用户管理）  
第二阶段：linux 各种配置（环境变量配置、网络配置、服务配置）  
第三阶段：linux 下搭建对应语言开发环境  
第四阶段：编写 shell 脚本，对 Linux 服务器进行维护  
第五阶段：进行安全设置，防止攻击，保障服务器正常进行，能对系统调优  
第六阶段：深入理解 Linux 系统（内核），熟练掌握大型网站应用架构组成、熟悉各个环节的部署和维护方法

---
# Linux入门

## 1 Linux入门 
### 1.1 概述
Linux由林纳斯.托瓦兹发明。是一套免费试用和自由传播的类**Unix**操作系统。   
知名发行版：Ubantu、RedHat、CenOS、Debian、SuSE
### 1.2 Linux、Windows区别
|比较|Windows|Linux|
|:---:|:---:|:---:|
|收费|收费且贵|免费或少许收费|
|软件与支持|数量和质量优势|免费质量欠缺|
|安全性|补丁，一般|相对更安全|
|使用习惯|图形交互|图形和命令交互|
|可定制性|封闭|开源|
|应用场景|桌面系统|服务器|

---

## 2 VM、Linux
### 2.1 VMware
在物理机上创建虚拟机的软件
### 2.2 CentOS
Linux一个发行版本

---

## 3 Linux文件与目录结构
### 3.1 Linux文件
Linux系统中“一切皆文件”
### 3.2 Linux目录结构
* /bin
Binary缩写，存放着最经常使用的指令
* /home
存放普通用户的主目录，在Linux中每个用户都有一个自己的目录，一般以用户账号命名
* /root
系统管理员，超级权限者用户主目录
* /etc
所有系统管理所需的配置文件和子目录
* /usr
重要，用户许多程序文件在此
* /boot
存放启动Linux时使用的核心文件
* /media
设备
* /mnt
临时挂载
* /opt
额外安装软件所摆放的目录
* /var
不断扩充的东西，经常修改的目录

---

## 4 VI / VIM 编辑器
### 4.1 VI / VIM
Unix操作系统和类Unix操作系统中最通用的文本编辑器
### 4.2 一般模式
以vi打开就直接进入一般模式（默认模式）。主要是复制粘贴和删除
|语法|功能|
|:---:|:---:|
|yy|复制光标当前行|
|y数字y|复制一段|
|p|粘贴到箭头目的行|
|u|撤消上一步|
|dd|删除光标当前行|
|d数字d|删除光标及后n行|
|x|剪切一个字母，相当于delete|
|X|剪切一个字母，相当于Backspace|
|yw|复制一个词|
|dw|删除一个词|
|^|移动到行头|
|$|移动到行尾|
|1+G|移动到页头，数字|
|G|移动到页尾|
|数字+G|移动到目标行|
### 4.3 编辑模式
一般模式 -> 编辑模式，按以下按键进入。
|按键|功能|
|:---:|:---:|
|i|当前光标前|
|a|当前光标后|
|o|当前光标下一行|
|I|光标所在行最前|
|A|光标所在行最后|
|O|当前光标上一行|
编辑模式 -> 一般模式，按ESC退出到一般模式。
### 4.4 指令模式
一般模式 -> 指令模式，按以下按键进入。
|命令|功能|
|:---:|:---:|
|:w|保存|
|:q|退出|
|:!|强制执行|
|/要查找的词|n往下找，N往上找|
|:noh|取消高亮|
|:set nu|显示行号|
|:set nonu|关闭行号|
|:%s/old/new/g|替换内容 /g替换匹配到的所有内容|

---

## 5 网络配置
- ifconfig 
显示所有网络接口的配置信息
- ping 目的主机
测试当前服务器是否可以连接目的主机
- hostname
查看当前服务器的主机名称

---

## 6 系统管理
### 6.1 Linux中进程和服务
计算机中，一个正在执行的程序或命令，被叫做“进程”（process）。  
启动之后一只存在、常驻内存的进程，一般被称作“服务”（service）。

CentOS 7 版本  
systemctl  
基本语法  
systemctl  start | stop | restart | status  服务名  
查看服务  
/user/lib/systemd/system
### 6.2 系统运行级别
CentOS 7 运行级别简化：  
multi-user.target 等价于原运行级别3（多用户有网，无图形界面）  
graphical.target 等价于原运行级别5（多用户有网，有图形界面）  

查看当前运行级别:  
systemctl get-default
### 关机重启命令
1 sync       （功能描述：将数据由内存同步到硬盘中）  
2 halt         （功能描述：停机，关闭系统，但不断电）  
3 poweroff （功能描述：关机，断电）  
4 reboot     （功能描述：就是重启，等同于shutdown -r now）  
5 shutdown [选项] 时间
> -H    相当于 halt 停机
> -r	相当于 reboot 重启
> now   立刻关机
> 时间  等待多久后关机（单位分钟）

---

## 7 常用基本命令
### 7.1 帮助命令
- man 获得帮助信息  
man [命令或配置文件]
```
查看ls命令的帮助信息
[root@curry ~]# man ls
```

- help 获得 shell 内置命令的帮助信息  
help 命令
```
查看cd命令的帮助信息
[root@curry ~]# help cd
```

- 常用快捷键  
ctr + c 停止进程

### 7.2 文件目录类
- pwd 显示当前工作目录的绝对路径
pwd: print working directory 打印工作目录
```
显示当前工作目录的绝对路径
[root@curry ~]# pwd
/root
```

- ls 列出目录的内容
ls:list 列出目录内容
ls [选项] [目录或是文件]
> 选项说明：
> -a 全部文件，包括隐藏档（.开头的文件）一起列出来
> -l 长数据列出，包括文件属性、权限等数据

```
查看当前目录的所有内容信息
[root@curry ~]# ls -al(简写 ll)
```

- cd 切换目录
cd:Change Directory 切换路径
cd [参数]
> 参数说明：
> cd 绝对路径	 切换路径
> cd 相对路径    切换路径
> cd ~或者cd 	回到自己的家目录
> cd - 		    回到上一次所在目录
> cd .. 		回到当前目录的上一级目录
> cd -P 		跳转到实际物理路径，而非快捷方式路径

- mkdir 创建一个新的目录
mkdir:Make directory 建立目录
mkdir [选项] 要创建的目录
> 选项说明 ：
-p 创建多层目录

```
创建一个多级目录
[root@curry ~]# mkdir -p xiyou/dssz/meihouwang
```

- rmdir 删除一个空的目录
rmdir:Remove directory 移除目录
rmdir 要删除的**空目录**
```
删除一个空的文件夹
[root@curry ~]# rmdir xiyou/dssz/meihouwang
```

- touch 创建空文件
touch 文件名称
```
[root@curry ~]# touch xiyou/dssz/sunwukong.txt
```

- cp 复制文件或目录
cp [选项] source dest
> 选项说明
> -r 递归复制整个文件夹

强制覆盖不提示的方法：\cp
```
递归复制整个文件夹
[root@curry ~]# cp -r xiyou/dssz/ ./
```

- rm 删除文件或目录
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
- mv 移动文件与目录或重命名
（1）mv oldNameFile newNameFile （功能描述：重命名）
（2）mv /temp/movefile /targetFolder （功能描述：移动文件）
- cat 查看文件内容
cat [选项] 要查看的文件
> 选项说明
> -n 显示所有行的行号，包括空行。

一般查看比较小的文件，一屏幕能显示全的。
```
查看文件内容并显示行号
[root@curry ~]# cat -n houge.txt
```

- more 文件内容分屏查看器
more 要查看的文件
> 选项说明
> space空格键	向下翻一页
> enter 回车键    向下翻一行
> q			  退出
> Ctrl+F	    向下滚动一屏
> Ctrl+B 	    返回上一屏

- less 分屏显示文件内容
less 要查看的文件
> 选项说明
> space空格键	向下翻一页
> pagedown	   向下翻一页
> pageup    	向上翻一页
> / 字串		   向下搜寻字串		
> q			  退出

- echo 输出内容到控制台
echo [选项] [输出内容]
> 选项说明
> -e	支持反斜线控制的字符转换

- head 显示文件头部内容
head 文件（功能描述：默认情况查看文件头10行内容）
head -n <数字> 文件（功能描述：查看文件头<数字>行内容）
```
查看文件的头2行
[root@curry ~]# head -n 2 smartd.conf
```

- tail 输出文件尾部内容
默认情况下tail 指令显示文件的后10 行内容。
> 选项说明
> -n <行数>	输出末尾n行内容
> -f	显示最新追加内容，监视文件变化

-  输出重定向>和 追加>>
\> 	覆盖
\>>	追加

- ln 软链接
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

- history 查看已经执行过历史命令
history （功能描述：查看已经执行过历史命令）

### 7.3 时间日期类
date [OPTION]... [+FORMAT]
> 选项说明
> -d<时间字符串>	显示指定的时间字符串表示的时间，而非当前时间
> -s<日期时间>		设置系统日期时间

- date 显示当前时间
（1）date 	（功能描述：显示当前时间）
（2）date +%Y （功能描述：显示当前年份）
（3）date +%m （功能描述：显示当前月份）
（4）date +%d （功能描述：显示当前是哪一天）
（5）date "+%Y-%m-%d %H:%M:%S" （功能描述：显示年月日时分秒）

- date 显示非当前时间
（1）date -d '1 days ago' （功能描述：显示前一天时间）
（2）date -d '-1 days ago' （功能描述：显示明天时间）

- date 设置系统时间
date -s 字符串时间
```
设置系统当前时间
[root@hadoop101 ~]# date -s "2023-08-19 20:52:18"
```

- cal 查看日历
cal [选项] （功能描述：不加选项，显示本月日历）
> 选项： 具体某一年		显示这一年的日历

### 7.4 用户管理命令
- useradd ：添加新用户
useradd 用户名		（功能描述：添加新用户）
useradd -g 组名用户名（功能描述：添加新用户到某个组）

- passwd ：设置用户密码
passwd 用户名（功能描述：设置用户密码）
```
设置用户的密码
[root@curry ~]# passwd tangseng
```

- id ：查看用户是否存在
id 用户名
```
查看用户是否存在
[root@curry ~]#id tangseng
```

- cat /etc/passwd ：查看创建了哪些用户
- cat /etc/shadow ：查看口令信息

- su ：切换用户
su: swith user 切换用户
su 用户名称（功能描述：切换用户，只能获得用户的执行权限，不能获得环境变量）
su - 用户名称（功能描述：切换到用户并获得该用户的环境变量及执行权限）

- userdel ：删除用户
userdel 用户名	（功能描述：删除用户但保存用户主目录）
userdel -r 用户名（功能描述：用户和用户主目录，都删除）

- who ：查看登录用户信息
whoami （功能描述：显示自身用户名称）
who am i （功能描述：显示登录用户的用户名以及登陆时间）

- sudo ：设置普通用户具有root 权限
修改配置文件	/etc/sudoers
```
## Allow root to run any commands anywhere
root ALL=(ALL) ALL
curry ALL=(ALL) ALL
```

- usermod ：修改用户
usermod -g 用户组  用户名

### 7.5 用户组管理命令
每个用户都有一个用户组，系统可以对一个用户组中的所有用户进行集中管理。

- groupadd ：新增组
groupadd 组名
```
添加一个xitianqujing组
[root@curry opt]#groupadd xitianqujing
```

- groupdel ：删除组
groupdel 组名
```
删除xitianqujing组
[root@curry opt]# groupdel xitianqujing
```

- groupmod ：修改组
groupmod -n 新组名  老组名

- cat /etc/group ：查看创建了哪些组

### 7.6 文件权限类
- 文件属性
1）0 首位表示类型
在Linux中第一个字符代表这个文件是目录、文件或链接文件等等
\-  代表文件
d   代表目录
l   链接文档(link file)；
2）第1-3位确定属主（该文件的所有者）拥有该文件的权限。---User
3）第4-6位确定属组（所有者的同组用户）拥有该文件的权限，---Group
4）第7-9位确定其他用户拥有该文件的权限---Other

- chmod :改变权限
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

- chown :改变所有者
chown [选项] [最终用户] [文件或目录] 
> 选项说明
> -R	递归

- chgrp :改变所属组
chgrp [最终用户组] [文件或目录]

### 7.7 搜索查找类
- find :查找文件或者目录
find [搜索范围] [选项]
> 选项说明
> -name<查询方式>	按照指定的文件名查找模式查找文件
> -user<用户名>	查找属于指定用户名所有文件
> -size<文件大小> 	按照指定的文件大小查找文件

- locate :快速定位文件路径
locate 搜索文件
由于locate 指令基于数据库进行查询，所以第一次运行前，必须使用updatedb 指令创建locate 数据库。

- grep :过滤查找及“|”管道符
管道符，“|”，表示将前一个命令的处理结果输出传递给后面的命令处理
grep 选项 	查找内容源文件

### 7.8 压缩和解压类
- gzip/gunzip :压缩
gzip 文件		（功能描述：压缩文件，只能将文件压缩为\*.gz 文件）
gunzip 文件.gz （功能描述：解压缩文件命令）

经验技巧
1）只能压缩文件不能压缩目录
2）不保留原来的文件
3）同时多个文件会产生多个压缩包

- zip/unzip :压缩
zip [选项] XXX.zip 将要压缩的内容	压缩文件和目录的命令
unzip [选项] XXX.zip				解压缩文件
> 选项说明
> -r	压缩目录
> -d	制定解压后文件的存放目录



# 编译
