gdb调试

启动文件：hello.c

命令：gcc  -g -o hello hello.c

gdb hello

(gdb) run       #运行 hello.c里的主函数

(gdb) list        #将源代码显示在终端





逐步调试：

gdb hello 

(gdb)b 25      #在25行打个断点break

(gdb)run

程序会在25行停下，进入断点

(gdb)n           # 逐步执行

