
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

#include "def.h"

using namespace std;
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

map<string, vector<string>> drmQueryEngineInfo()
{
    map<string, vector<string>> ei_list;

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

    vector<char> engine_info(len, 0);
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

        auto it = engine_class_map.find(ei->engine.engine_class);
        if (it == engine_class_map.end())
            continue;
        
        char idx[16] = {};
        sprintf(idx, "%02d", ei->engine.engine_instance);
        string str_info = engine_class_map[ei->engine.engine_class];
        str_info += "_" + string(idx); //to_string(ei->engine.engine_instance);

        vector<string> caps;
        if (ei->capabilities & 0x1)
            switch (ei->engine.engine_class)
            {
            case 0:
                caps.push_back("3D");
                break;
            case 1:
                caps.push_back("BLOCK_COPY");
                break;
            case 2:
                caps.push_back("HEVC");
                break;
            default:
                break;
            }
        if (ei->capabilities & 0x4)
            caps.push_back("VDENC");
        if (ei->capabilities & 0x2)
            caps.push_back("SFC");
        ei_list[str_info] = caps;
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