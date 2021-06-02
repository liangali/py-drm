
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "drm.h"
#include "i915_drm.h"

namespace py = pybind11;

int ret = 0;
int fd = 0;

void drmOpen()
{
    fd = open("/dev/dri/renderD128", O_RDWR);
    printf("fd = %d\n", fd);
}

void drmClose()
{
    close(fd);
    printf("fd closed\n");
}

int drmCreateGemObject(uint64_t size)
{
    uint32_t handle;
    drm_i915_gem_create gem_create = {};
    gem_create.size = size;
    if (ioctl(fd, DRM_IOCTL_I915_GEM_CREATE, &gem_create) == 0) {
        handle = gem_create.handle;
        return handle;
    } else {
        printf("ERROR: GEM_CREATE failed\n");
        return -1;
    }
}

PYBIND11_MODULE(pydrm, m) {
    m.doc() = "drm python bindings"; 
    m.def("open", &drmOpen, "open display device");
    m.def("close", &drmClose, "close display device");

    m.def("create_gem", &drmCreateGemObject, "create gem object");
}