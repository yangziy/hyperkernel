/*
    Bad version of create_uuid.c
 */

#include "user.h"

int main(int argc, char *argv[])
{
    sys_seccomp(); // This prevents next_uuid() from increasing

    printf(1, "%d\n", next_uuid());
    exit();
}
