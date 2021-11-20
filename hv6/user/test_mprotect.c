//
// Created by hk on 11/19/21.
//

#include "user.h"
#define PGSIZE 4096
#define rounddown(x, y)                                                                            \
    ({                                                                                             \
        uintmax_t _x = (uintmax_t)(x);                                                             \
        const typeof(y) _y = y;                                                                    \
        (typeof(x))(((_x) / _y) * _y);                                                             \
    })


int main(int argc, char *argv[])
{
    // Manual mmap (maybe we should implement mmap and munmap later)
    pn_t pagenumber = find_free_page();
    char* addr = (char*) (pagenumber * PGSIZE);
    map_pages( (void*) addr, PGSIZE, PROT_READ | PROT_WRITE);
    addr[0] = 'A';
    printf(1, "%c\n", addr[0]);
    mprotect((void*) addr, PGSIZE, PROT_READ);
    addr[0] = 'A';
    printf(1, "Loop completed\n");  // You should not see this
    exit();
}
