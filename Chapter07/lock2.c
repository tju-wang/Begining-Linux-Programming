#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>

//创建锁文件的存储目录 起一个与要保护文件(目标文件)相关联的文件名
const char *lock_file = "/tmp/LCK.test2";

int main() {
    int file_desc;
    int tries = 10;

    while (tries--) {
        file_desc = open(lock_file, O_RDWR | O_CREAT | O_EXCL, 0444);//打开锁文件  若文件存在 则调用失败
        if (file_desc == -1) {  //文件存在 调用失败  此时不能访问目标文件
            printf("%d - Lock already present\n", getpid());
            sleep(3);
        }
        else {
                /* critical region */
            printf("%d - I have exclusive access\n", getpid());
            sleep(1);
            (void)close(file_desc);//执行过后关闭文件
            (void)unlink(lock_file);//删除锁文件
                /* non-critical region */
            sleep(2);
        }
    } /* while */
    exit(EXIT_SUCCESS);
}


//编译运行  同时运行两个该程序  程序之间将协调使用目标文件
/*
gcc -g -o target2 lock2.c
./lock2 & ./lock2

*/

