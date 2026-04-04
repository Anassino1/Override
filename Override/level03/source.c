#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void decrypt(int key)
{
    char data[] = "Q}|u`sfg~sf{}|a";

    int len = strlen(data);

    // XOR each byte with key
    for (int i = 0; i < len; i++)
    {
        data[i] ^= key;
    }

    // compare with target string
    if(strcmp(data, "Congratulations!") == 0)
    {
        system("/bin/sh");  
    }
    else
    {
        puts("\nInvalid Password");
    }
}

void test(int input, int key)
{
    int diff = key - input;

    if (diff > 0x15) {
        decrypt(rand());
        return;
    }

    switch (diff)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
        case 18:
        case 19:
        case 20:
        case 21:
            decrypt(diff);
            break;

        default:
            decrypt(rand());
            break;
    }
}

int main(int argc, char *argv[])
{
    int user_input;

    srand(time(NULL));

    puts("***********************************");
    puts("*        level03                  *");
    puts("***********************************");

    printf("Password:");
    scanf("%d", &user_input);

    // call test(user_input, 0x1337d00d)
    test(user_input, 0x1337d00d); // 322424845

    return 0;
}