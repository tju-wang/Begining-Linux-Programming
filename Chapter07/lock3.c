#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

//使用fcntl锁定文件

const char *test_file = "/tmp/test_lock";

int main() {
    int file_desc;
    int byte_count;
    char *byte_to_write = "A";
    struct flock region_1;//区域操作  锁结构体
    struct flock region_2;
    int res;

        /* open a file descriptor */
    file_desc = open(test_file, O_RDWR | O_CREAT, 0666);
    //open()函数 第二个参数 必选项为打开文件类型（可读、可写、可读可写） 
    //可选项有 O_CREAT 文件不存在则创建它 此时需要提供第三个参数  表明文件的权限
    if (!file_desc) {
        fprintf(stderr, "Unable to open %s for read/write\n", test_file);
        exit(EXIT_FAILURE);
    }

        /* put some data in the file */
    for(byte_count = 0; byte_count < 100; byte_count++) {
        (void)write(file_desc, byte_to_write, 1);
    }

        /* setup region 1, a shared lock, from bytes 10 -> 30 */
    region_1.l_type = F_RDLCK;  //共享锁  可读
    region_1.l_whence = SEEK_SET;//文件头  SEEK_CUR当前指向位置  SEEK_END文件尾 
    region_1.l_start = 10;  //针对关注区域的偏移
    region_1.l_len = 20;//关注区域长度
    
        /* setup region 2, an exclusive lock, from bytes 40 -> 50 */
    region_2.l_type = F_WRLCK;  //独占锁
    region_2.l_whence = SEEK_SET;
    region_2.l_start = 40;
    region_2.l_len = 10;

        /* now lock the file */
    printf("Process %d locking file\n", getpid());
    res = fcntl(file_desc, F_SETLK, &region_1);
    if (res == -1) fprintf(stderr, "Failed to lock region 1\n");
    res = fcntl(file_desc, F_SETLK, &region_2);
    if (res == -1) fprintf(stderr, "Failed to lock region 2\n");

        /* and wait for a while */
    sleep(60);

    printf("Process %d closing file\n", getpid());    
    close(file_desc);
    exit(EXIT_SUCCESS);
}
