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
    int data = 0;
    for (auto p: mapGetParam)
    {
        struct drm_i915_getparam gp = {.param = p.second, .value = &data};
        if (ret = ioctl(fd, DRM_IOCTL_I915_GETPARAM, &gp)) {
            printf("ERROR: DRM_IOCTL_I915_GETPARAM failed, ret= %d, param = %s(%d)\n", ret, p.first.c_str(), p.second);
            continue;
            //return -1;
        }
        printf("%s = %d\n", p.first.c_str(), *gp.value);
    }
    return 0;
}

int main()
{
    init();

    query_version();

    get_param();

    close();
    return 0;
}
