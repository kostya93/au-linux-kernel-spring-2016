#include "../vsd_driver/vsd_ioctl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define VSD_PATH "/dev/vsd"

void help(char * str)
{
    printf("use the program correctly:\n");
    printf("%s size_get\n", str);
    printf("%s size_set SIZE_IN_BYTES\n", str);
}

int opn_vsd()
{
    int file = open(VSD_PATH, O_RDONLY);
    if (file == -1)
    {
        printf("cant open vsd\n");
        return EXIT_FAILURE;
    }
    return file;
}

int size_get()
{
    int file = opn_vsd();

    if (file == EXIT_FAILURE)
        return EXIT_FAILURE;

    vsd_ioctl_get_size_arg_t arg;
    if (ioctl(file, VSD_IOCTL_GET_SIZE, &arg))
    {
        printf("cant read size of vsd\n");
        close(file);
        return EXIT_FAILURE;
    }
    close(file);
    printf("size = %lu \n", arg.size);
    return EXIT_SUCCESS;
}

int size_set(int new_size)
{
    int file = opn_vsd();
    
    if (file == EXIT_FAILURE)
        return EXIT_FAILURE;

    vsd_ioctl_get_size_arg_t arg;
    arg.size = new_size;
    if (ioctl(file, VSD_IOCTL_SET_SIZE, &arg))
    {
        printf("cant set new size to vsd\n");
        close(file);
        return EXIT_FAILURE;
    }
    close(file);
    return EXIT_SUCCESS;
}


int main(int argc, char **argv) 
{
    if (argc < 2 || argc > 3)
    {
        help(argv[0]);
        return EXIT_FAILURE;
    }
    if (argc == 2 && strcmp(argv[1], "size_get") == 0)
    {
        return size_get();
    }
    else if (argc == 3 && strcmp(argv[1], "size_set") == 0)
    {
        return size_set(atoi(argv[2]));
    }
    else
    {
        help(argv[0]);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
