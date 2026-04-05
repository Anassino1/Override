#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    char input[0x70] = {0};        // rbp-0x70
    char filebuf[0xA0] = {0};      // rbp-0xa0
    char password[0x110] = {0};    // rbp-0x110

    FILE *fp = NULL;
    int nread = 0;

    fp = fopen("/home/users/level03/.pass", "r");

    if (fp == NULL) {
        fwrite("ERROR: failed to open password file\n", 1, 0x24, stderr);
        exit(1);
    }

    nread = fread(filebuf, 1, 0x29, fp); // read 0x29 bytes
    filebuf[strcspn(filebuf, "\n")] = '\0';// remove newline

    if (nread != 0x29) {
        fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
        exit(1);
    }

    fclose(fp);

    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");
    printf("--[ Username: ");

    // read username/input
    fgets(input, 0x64, stdin);
    input[strcspn(input, "\n")] = '\0';

    printf("--[ Password: ");

    // read password
    fgets(password, 0x64, stdin);
    password[strcspn(password, "\n")] = '\0';

    puts("*****************************************");

    // compare password with file content
    if (strncmp(filebuf, password, 0x29) == 0) //41
    {
        printf("Greetings, %s!\n", input);
        system("/bin/sh");
        return 0;
    } else {
        printf(input);   // this is the vulnerability
        puts(" does not have access!");
        exit(1);
    }
}