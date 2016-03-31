#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
int main()
{
    size_t sum = 0;
    size_t current_block = 18446744073709551615UL;
    char * new_adr;
    int i = 0;
    while(current_block != 0)
    {
        i++;
        new_adr = mmap(NULL, current_block, PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, 0, 0);
        if (MAP_FAILED == new_adr)
        {
            current_block /= 2;
        }
        else
        {
            sum += current_block;
        }
    }

    printf("%d\n", i);

    printf("%zu\n", sum);

    return 0;
}
