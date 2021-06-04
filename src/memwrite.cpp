#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/time.h>

#include "drm.h"
#include "i915_drm.h"

#define MI_BATCH_BUFFER_END    (0xA << 23)

int main()
{
    int err = 0;
    int fd = 0;

    fd = open("/dev/dri/renderD128", O_RDWR);
    printf("fd = %d\n", fd);

    // Create Gem object
    uint32_t handle;
    struct drm_i915_gem_create gem_create = {
        .size = 4096,
    };
    if (ioctl(fd, DRM_IOCTL_I915_GEM_CREATE, &gem_create) == 0) {
        handle = gem_create.handle;
        printf("INFO: GEM handle = %d\n", handle);
    } else {
        printf("ERROR: GEM_CREATE failed\n");
        return -1;
    }

#if 0
    struct drm_i915_gem_object_param obj = {};
    uint32_t id = 131073;
    obj.handle = handle;
    obj.size = 1;
    obj.param = I915_PARAM_MEMORY_REGION | I915_OBJECT_PARAM;
    obj.data = (uintptr_t)&id;
    if (ioctl(fd, DRM_IOCTL_I915_GEM_OBJECT_SETPARAM, &obj)) {
        err = -errno;
        errno = 0;
    }

    struct drm_i915_gem_mmap_offset mmap_arg = {};
    mmap_arg.handle = handle;
    mmap_arg.flags = I915_MMAP_OFFSET_WC;
    int ret = ioctl(fd, DRM_IOCTL_I915_GEM_MMAP_OFFSET, &mmap_arg);

    uint64_t offset = mmap_arg.offset;
    uint32_t *mappedData = (uint32_t *) mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, (size_t) offset );
    printf("done, mapped address is %p, offset is %x\n", mappedData, offset);

    uint8_t *src = (uint8_t *)malloc(4096);

    while (1) {
        struct timeval start, end;
        gettimeofday(&start, NULL);
        //memcpy(mappedData, src, 4096);
        for (int i = 0 ; i < 1024; i++) {
            mappedData[i] ++;
            //mappedData[i] = src[i];
        }
        gettimeofday(&end, NULL);

        double dur = (end.tv_sec - start.tv_sec)*1000000.0 + (end.tv_usec - start.tv_usec);
        printf("%f\n", dur);
        usleep(100000);
    }
#endif

    close(fd);
    printf("done\n");
    return 0;
}
