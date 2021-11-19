/*
    Print a universally unique identifier (UUID) to stdout
    The UUID starts from 1 and increases by 1 each time this program is executed
 */

#include "user.h" // This is for printf(); exit() and sys_uuid() DO NOT need it.

int main(int argc, char *argv[])
{
    // sys_seccomp(); // This prevents next_uuid() from increasing

    printf(1, "%d\n", next_uuid());
    exit(); // calling exit() because echo.c does so
}
