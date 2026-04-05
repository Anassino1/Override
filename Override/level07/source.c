#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int store_number(unsigned int *buffer);
int read_number(unsigned int *buffer);

int main(int argc, char **argv)
{
    char command[20];
    unsigned int buffer[100];  // important

    // initialize buffer to 0
    memset(buffer, 0, sizeof(buffer));

    printf("----------------------------------------------------\n");
    printf("  Welcome to wil's crappy number storage service!   \n");
    printf("----------------------------------------------------\n");

    while (1)
    {
        printf("Input command: ");
        fgets(command, sizeof(command), stdin);

        // remove newline
        command[strcspn(command, "\n")] = 0;

        if (strncmp(command, "store", 5) == 0)
        {
            if (store_number(buffer) != 0)
                printf(" Failed to do %s command\n", command);
            else
                printf(" Completed store command successfully\n");
        }
        else if (strncmp(command, "read", 4) == 0)
        {
            if (read_number(buffer) != 0)
                printf(" Failed to do %s command\n", command);
            else
                printf(" Completed read command successfully\n");
        }
        else if (strncmp(command, "quit", 4) == 0)
        {
            break;
        }
        else
        {
            printf(" Unknown command: %s\n", command);
        }
    }

    return 0;
}


int store_number(unsigned int *buffer)
{
    unsigned int value;
    unsigned int index;

    printf(" Number: ");
    scanf("%u", &value);

    printf(" Index: ");
    scanf("%u", &index);

    //  restriction 1
    if (index % 3 == 0)
    {
        printf(" *** ERROR! ***\n");
        printf("   This index is reserved!\n");
        printf(" *** ERROR! ***\n");
        return 1;
    }

    //  restriction 2
    if ((value >> 24) == 0xb7)
    {
        printf(" *** ERROR! ***\n");
        printf("   This value is not allowed!\n");
        printf(" *** ERROR! ***\n");
        return 1;
    }

    // 💣 VULNERABILITY: no bounds check
    buffer[index] = value;

    return 0;
}

int read_number(unsigned int *buffer)
{
    unsigned int index;

    printf(" Index: ");
    scanf("%u", &index);

    printf(" Number at data[%u] is %u\n", index, buffer[index]);

    return 0;
}