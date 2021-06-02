
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/numpy.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include <va/va.h>
#include <va/va_drm.h>

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

PYBIND11_MODULE(pydrm, m) {
    m.doc() = "drm python bindings"; 
    m.def("open", &drmOpen, "open display device");
    m.def("close", &drmClose, "close display device");
}