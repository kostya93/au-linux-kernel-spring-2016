#include <sys/mman.h>

#include <fcntl.h>
#include <stdlib.h>

#include "vsd_device.h"
#include "../vsd_driver/vsd_ioctl.h"

#define VSD_PATH "/dev/vsd"

static int vsd_dev;

int vsd_init()
{
    vsd_dev = open(VSD_PATH, O_RDWR);
    
    if (vsd_dev == -1)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

int vsd_deinit()
{
    return close(vsd_dev);
}

int vsd_get_size(size_t *out_size)
{
    vsd_ioctl_get_size_arg_t arg;

    int res = ioctl(vsd_dev, VSD_IOCTL_GET_SIZE, &arg);

    if (res == -1)
    {
        return EXIT_FAILURE;
    }

    *out_size = arg.size;

    return EXIT_SUCCESS;
}

int vsd_set_size(size_t size)
{
    vsd_ioctl_set_size_arg_t arg;
    arg.size = size;

    int res = ioctl(vsd_dev, VSD_IOCTL_SET_SIZE, &arg);

    if (res == -1)
    {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

ssize_t vsd_read(char* dst, off_t offset, size_t size)
{
    int res = lseek(vsd_dev, offset, SEEK_SET);

    if (res == -1)
    {
        return EXIT_FAILURE;
    }

    return read(vsd_dev, dst, size);
}

ssize_t vsd_write(const char* src, off_t offset, size_t size)
{
    int res = lseek(vsd_dev, offset, SEEK_SET);

    if (res == -1)
    {
        return EXIT_FAILURE;
    }

    return write(vsd_dev, src, size);
}

void* vsd_mmap(size_t offset)
{
    size_t vsd_dev_size = 0;
    vsd_get_size(&vsd_dev_size);

    return mmap(0, vsd_dev_size - offset, PROT_READ | PROT_WRITE, MAP_SHARED, vsd_dev, offset);
}

int vsd_munmap(void* addr, size_t offset)
{
    size_t vsd_dev_size = 0;
    vsd_get_size(&vsd_dev_size);

    return munmap(addr, vsd_dev_size - offset);
}
