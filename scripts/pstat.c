#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    struct stat b;
    //获取文件的元数据
    int s = stat(argv[1], &b);
    if(s == -1) {
        perror("stat");
        return -1;
    }
    //输出文件的元数据
    printf("st_ino:%lu\n", b.st_ino);
    printf("length:%ld\n", b.st_size);
    printf("blocks: %ld\n", b.st_blocks);
    return 0;
}