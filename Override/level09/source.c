#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Lv
{
    char padding[0x8c];   // before username
    char username[0x28];  // 40 bytes
    int len;              // at offset 0xb4
    char msg[0x80];       // message buffer
} Lv;

int secret_backdoor()
{
    char str[0x80];

    fgets(str, 0x80, stdin);
    system(str);

    return 0;
}

void set_username(Lv *tmp)
{
    char buf[0x80];
    int i = 0;

    memset(buf, 0, sizeof(buf));

    puts(">: Enter your username");
    printf(">>: ");
    fgets(buf, 0x80, stdin);

    while (i <= 0x28)
    {
        tmp->username[i] = buf[i];

        if (buf[i] == '\0')
            break;

        i++;
    }

    printf(">: Welcome, %s", tmp->username);
}

void set_msg(Lv *tmp)
{
    char buf[0x400];

    memset(tmp->msg, 0, sizeof(tmp->msg));

    puts(">: Msg @Unix-Dude");
    printf(">>: ");

    fgets(buf, sizeof(buf), stdin);

    // Vulnerability: controlled length
    strncpy(tmp->msg, buf, tmp->len);
}

void handle_msg()
{
    Lv data;

    // initialize struct
    memset(&data, 0, sizeof(data));

    set_username(&data);
    set_msg(&data);

    puts(">: Msg sent!");
}

int main()
{
    puts("--------------------------------------------");
    puts("|   ~Welcome to l33t-m$n ~    v1337        |");
    puts("--------------------------------------------");

    handle_msg();

    return 0;
}