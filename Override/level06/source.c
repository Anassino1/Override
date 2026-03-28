#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ptrace.h>

int auth(char *login, unsigned int serial)
{
    int i;
    int len;
    int key;
    int tmp;

    // --- remove newline ---
    login[strcspn(login, "\n")] = '\0';

    // --- length check ---
    len = strnlen(login, 32);
    if (len <= 5)
        return 1;

    // --- anti-debug ---
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1)
    {
        puts("Debugger detected");
        puts("Exiting...");
        return 1;
    }

    // --- initial key ---
    key = (login[3] ^ 0x1337) + 0x5eeded;

    // --- hashing loop ---
    for (i = 0; i < len; i++)
    {
        if (login[i] <= 31)
            return 1;

        tmp = login[i] ^ key;

        // equivalent of the weird assembly math
        int edx = 0x88233b2b;
        int eax = tmp;

        eax = (eax * edx);
        eax = tmp - edx;
        eax = (eax >> 1) + edx;
        eax = eax >> 10;
        eax = eax * 0x539;

        tmp = tmp - eax;

        key += tmp;
    }

    // --- final check ---
    if (serial == key)
        return 0;

    return 1;
}

int main(void)
{
    char login[32];
    unsigned int serial;

    puts("***********************************");
    puts("*        Secure Login System      *");
    puts("***********************************");

    printf("-> Enter Login: ");
    fgets(login, sizeof(login), stdin);

    puts("***********************************");
    puts("*      Enter Serial Number        *");
    puts("***********************************");

    printf("-> Enter Serial: ");
    scanf("%u", &serial);

    if (auth(login, serial) == 0)
    {
        puts("Authenticated!");
        system("/bin/sh");
    }
    else
    {
        puts("Authentication failed.");
    }

    return 0;
}