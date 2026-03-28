#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char buffer[100];
    int i = 0;

    fgets(buffer, 100, stdin);

    i = 0;
    while (i < strlen(buffer))
    {
        if (buffer[i] > '@' && buffer[i] <= 'Z')
        {
            buffer[i] ^= 0x20; // convert uppercase to lowercase
        }
        i++;
    }

    printf(buffer);   // ⚠️ VULNERABILITY
    exit(0);
}