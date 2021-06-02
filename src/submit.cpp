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

#define MI_BATCH_BUFFER_END	(0xA << 23)

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

    // Write Gem object
    const uint32_t batch[2] = { 0, MI_BATCH_BUFFER_END };
    struct drm_i915_gem_pwrite gem_pwrite = {};
    gem_pwrite.handle = handle;
    gem_pwrite.offset = 0;
    gem_pwrite.size = sizeof(batch);
    gem_pwrite.data_ptr = (uint64_t)batch;
    if (ioctl(fd, DRM_IOCTL_I915_GEM_PWRITE, &gem_pwrite) == 0) {
        printf("INFO: Write GEM boject");
    } else {
        printf("ERROR: GEM_PWRITE failed\n");
        return -1;
    }
    
    // Create GEM context
    uint32_t ctx_id = 0;
    struct drm_i915_gem_context_create ctx_create = {};
    if (ioctl(fd, DRM_IOCTL_I915_GEM_CONTEXT_CREATE, &ctx_create) == 0) {
        ctx_id = ctx_create.ctx_id;
        printf("INFO: Context id = %d\n", ctx_id);
    } else {
        printf("ERROR: GEM_CONTEXT_CREATE failed\n");
        return -1;
    }

    // Sumbit batch buffer
    int ring = 0;
    struct drm_i915_gem_exec_object2 obj = { .handle = handle };
    struct drm_i915_gem_execbuffer2 execbuf2 = {
        .buffers_ptr = (uint64_t)(&obj),
        .buffer_count = 1,
        .flags = ring,
        .rsvd1 = ctx_id & I915_EXEC_CONTEXT_ID_MASK,
    };
    
    for (size_t i = 0; i < 10000; i++)
    {
        if (ioctl(fd, DRM_IOCTL_I915_GEM_EXECBUFFER2, &execbuf2) == 0) {
            //printf("INFO: Execute EXECBUFFER2\n");
        } else {
            printf("ERROR: EXECBUFFER2 failed\n");
            return -1;
        }
    }

    // Destroy context
    struct drm_i915_gem_context_destroy destroy = { ctx_id };
    if (ioctl(fd, DRM_IOCTL_I915_GEM_CONTEXT_DESTROY, &destroy) == 0) {
        printf("INFO: Destroy Gem context\n");
    } else {
        printf("ERROR: CONTEXT_DESTROY failed\n");
        return -1;
    }

    close(fd);

    printf("done\n");
    return 0;
}
