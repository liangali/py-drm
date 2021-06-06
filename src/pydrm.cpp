
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

std::vector<std::string>  drmQueryEngineInfo()
{
    std::vector<std::string> ei_list;

    drm_i915_query_item query_item = {};
    query_item.query_id = DRM_I915_QUERY_ENGINE_INFO;
    query_item.length = 0;

    drm_i915_query query = {};
    query.num_items = 1;
    query.items_ptr = (uintptr_t)&query_item;

    if (ret = ioctl(fd, DRM_IOCTL_I915_QUERY, &query)) {
        printf("ERROR: DRM_IOCTL_I915_GEM_GET_APERTURE failed\n");
        return ei_list;
    } 
    int len = query_item.length;

    std::vector<char> engine_info(len, 0);
    query_item = {};
    query_item.query_id = DRM_I915_QUERY_ENGINE_INFO;
    query_item.length = len;
    query_item.data_ptr = (uintptr_t)engine_info.data();
    query.flags = 0;
    query.num_items = 1;
    query.items_ptr = (uintptr_t)&query_item;

    if (ret = ioctl(fd, DRM_IOCTL_I915_QUERY, &query)) {
        printf("ERROR: DRM_IOCTL_I915_GEM_GET_APERTURE failed\n");
        return ei_list;
    } 

    drm_i915_query_engine_info* info = (drm_i915_query_engine_info*)engine_info.data();
    for (size_t i = 0; i < info->num_engines; i++)
    {
        drm_i915_engine_info *ei = (drm_i915_engine_info *)&info->engines[i];
        std::string str_info;
        switch (ei->engine.engine_class)
        {
        case 0:
            str_info = "RENDER";
            break;
        case 1:
            str_info = "BLT";
            break;
        case 2:
            str_info = "VCS";
            break;
        case 3:
            str_info = "VECS";
            break;
        case -1:
            str_info = "INVALID";
            break;       
        default:
            break;
        }
        ei_list.push_back(str_info);
    }

    return ei_list;
}

PYBIND11_MODULE(pydrm, m) {
    m.doc() = "drm python bindings"; 
    m.def("open", &drmOpen, "open display device");
    m.def("close", &drmClose, "close display device");

    m.def("create_gem", &drmCreateGemObject, "create gem object");
    m.def("query_engine", &drmQueryEngineInfo, "query engine info");
}