#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include "asgn1_ioctl.h"

int main(void)
{
    int fd;
    int mode;
    struct asgn1_stats stats;
    char version[ASGN1_VERSION_LEN] = {0};

    fd = open("/dev/asgn1", O_RDWR);

    if (fd < 0) {
        perror("open");
        return 1;
    }

    /* Test GET_VERSION */
    if (ioctl(fd, ASGN1_GET_VERSION, version) < 0)
        perror("GET_VERSION");
    else
        printf("Version: %s\n", version);

    /* Test SET_MODE */
    mode = 0;

    if (ioctl(fd, ASGN1_SET_MODE, &mode) < 0)
        perror("SET_MODE");
    else
        printf("Mode set to KERNEL\n");

    /* Test GET_STATS */
    if (ioctl(fd, ASGN1_GET_STATS, &stats) < 0) {
        perror("GET_STATS");
    } else {
        printf("Open count: %d\n", stats.open_count);
        printf("Write count: %d\n", stats.write_count);
        printf("Read count: %d\n", stats.read_count);
        printf("Buffer length: %d\n", stats.buffer_len);
        printf("Last write size: %d\n",
               stats.last_write_size);
    }

    /* Test RESET_BUFFER */
    if (ioctl(fd, ASGN1_RESET_BUFFER) < 0)
        perror("RESET_BUFFER");
    else
        printf("Buffer reset successful\n");

    close(fd);

    return 0;
}
