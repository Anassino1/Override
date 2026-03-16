#include <stdio.h>
#include <string.h>

struct user {
    char a_user_name[256];
};

struct user obj;

int verify_user_name() {
    printf("verifying username....\n\n");
    if (strncmp(obj.a_user_name, "dat_wil", 7) == 0)
        return 0;
    return 1;
}

int main() {
    char buffer[64];   // small stack buffer (vulnerable)

    printf("********* ADMIN LOGIN PROMPT *********\n");

    printf("Enter Username: ");
    fgets(obj.a_user_name, 0x100, stdin);   // first fgets (global buffer)

    if (verify_user_name()) {
        printf("nope, incorrect username...\n");
        return 1;
    }

    printf("Enter Password: ");
    fgets(buffer, 0x64, stdin);             // second fgets (overflow here)

    printf("nope, incorrect password...\n");

    return 0;
}