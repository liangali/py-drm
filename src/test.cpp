#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "drm.h"
#include "i915_drm.h"

int err = 0;
int fd = 0;

int main()
{
    fd = open("/dev/dri/renderD128", O_RDWR);
    printf("fd = %d\n", fd);



    close(fd);
    printf("done\n");
    return 0;
}
