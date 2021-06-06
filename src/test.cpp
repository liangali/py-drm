#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include <vector>

#include "drm.h"
#include "i915_drm.h"

#include "def.h"

using namespace std;

int ret = 0;
int fd = 0;

void init()
{
    fd = open("/dev/dri/renderD128", O_RDWR);
    printf("fd = %d\n", fd);
}

void close()
{
    close(fd);
    printf("fd closed\n");
}

int query_version()
{
    drm_version_t version = {};
    if (ret = ioctl(fd, DRM_IOCTL_VERSION, &version)) {
        printf("ERROR: ioctl DRM_IOCTL_VERSION failed\n");
        return -1;
    }
    vector<char> vername(version.name_len+1, 0);
    vector<char> verdate(version.date_len+1, 0);
    vector<char> verdesc(version.desc_len+1, 0);

    version.name = vername.data();
    version.date = verdate.data();
    version.desc = verdesc.data();
    if (ret = ioctl(fd, DRM_IOCTL_VERSION, &version)) {
        printf("ERROR: ioctl DRM_IOCTL_VERSION failed\n");
        return -1;
    }
    printf("drm-info: version = %d.%d.%d\n", version.version_major, version.version_minor, version.version_patchlevel);
    printf("drm-info: name = %s\n", version.name);
    printf("drm-info: date = %s\n", version.date);
    printf("drm-info: desc = %s\n", version.desc);

    return 0;
}

int get_param()
{
    printf("\nINFO: DRM_IOCTL_I915_GETPARAM \n");
    for (auto p: mapGetParam)
    {
        int data = 0;
        struct drm_i915_getparam gp = {.param = p.second, .value = &data};
        if (ret = ioctl(fd, DRM_IOCTL_I915_GETPARAM, &gp)) {
            printf("ERROR: DRM_IOCTL_I915_GETPARAM failed, ret= %d, param = %s(%d)\n", ret, p.first.c_str(), p.second);
            continue;
        }
        printf("%s = %d\n", p.first.c_str(), *gp.value);
    }

    printf("\nINFO: DRM_IOCTL_I915_GEM_GET_APERTURE \n");
    drm_i915_gem_get_aperture aperture = {};
    if(ret = ioctl(fd, DRM_IOCTL_I915_GEM_GET_APERTURE,  &aperture)) {
        printf("ERROR: DRM_IOCTL_I915_GEM_GET_APERTURE failed\n");
    }
    printf("INFO: aper_size = %lld, aper_available_size = %lld\n", aperture.aper_size, aperture.aper_available_size);

    printf("\nINFO: DRM_IOCTL_I915_GEM_CONTEXT_GETPARAM \n");
    for (auto p: mapContextParam)
    {
        uint64_t data = 0;
        drm_i915_gem_context_param cp = {.param = p.second};
        if (ret = ioctl(fd, DRM_IOCTL_I915_GEM_CONTEXT_GETPARAM, &cp)) {
            printf("ERROR: DRM_IOCTL_I915_GEM_CONTEXT_GETPARAM failed, ret= %d, param = %s(%d)\n", ret, p.first.c_str(), p.second);
            continue;
        }
        printf("%s = %lld\n", p.first.c_str(), cp.value);
    }

    return 0;
}

int query_engine()
{
    drm_i915_query_item query_item = {};
    query_item.query_id = DRM_I915_QUERY_ENGINE_INFO;
    query_item.length = 0;

    drm_i915_query query = {};
    query.num_items = 1;
    query.items_ptr = (uintptr_t)&query_item;

    if (ret = ioctl(fd, DRM_IOCTL_I915_QUERY, &query)) {
        printf("ERROR: DRM_IOCTL_I915_GEM_GET_APERTURE failed\n");
        return -1;
    } 
    printf("INFO: engine info length = %d\n", query_item.length);
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
        return -1;
    } 

    drm_i915_query_engine_info* info = (drm_i915_query_engine_info*)engine_info.data();
    for (size_t i = 0; i < info->num_engines; i++)
    {
        drm_i915_engine_info *ei = (drm_i915_engine_info *)&info->engines[i];
        printf("INFO: i = %d, engine_class = %d, engine_instance = %d\n", i, ei->engine.engine_class, ei->engine.engine_instance);
    }
    
    return 0;
}

int main()
{
    init();

    query_version();
    get_param();
    query_engine();

    close();
    return 0;
}
