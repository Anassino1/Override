#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void log_wrapper(FILE *stream, const char *prefix, const char *msg)
{
    char buffer[0x110]; // 272 bytes

    // Copy prefix
    strcpy(buffer, prefix);

    // Find current length
    size_t len = strlen(buffer);

    // Remaining space (0xfe = 254)
    size_t remaining = 0xfe - len;

    // Append formatted msg safely
    snprintf(buffer + len, remaining, "%s", msg);

    // Remove newline (strcspn with "\n")
    buffer[strcspn(buffer, "\n")] = '\0';

    // Print to stream
    fprintf(stream, "%s\n", buffer);
}

int main(int argc, char **argv)
{
    FILE *log_file;
    FILE *input_file;
    int fd = -1;
    char buffer[0x70];   // 112 bytes
    char ch = 0xFF;

    // If argc != 2 → print usage
    if (argc != 2) {
        printf("Usage: %s <file>\n", argv[0]);
    }

    // Open log file
    log_file = fopen("log.txt", "a");   // (0x400d6d, 0x400d6b)
    if (!log_file) {
        printf("Error opening log file\n");
        exit(1);
    }

    // Log start
    log_wrapper(log_file, "[START] ", argv[1]);

    // Open input file
    input_file = fopen(argv[1], "r");   // (mode from 0x400da9)
    if (!input_file) {
        printf("Error opening file: %s\n", argv[1]);
        exit(1);
    }

    // Build output filename
    strcpy(buffer, "output_");  // (0x400dab)

    // Append argv[1] safely (manual length calc in asm)
    strncat(buffer, argv[1], 0x63 - strlen(buffer));

    // Open output file
    fd = open(buffer, 0xC1, 0x1B0);  
    // flags ≈ O_WRONLY | O_CREAT | O_TRUNC
    // mode  = 0660

    if (fd < 0) {
        printf("Error creating file: %s\n", buffer);
        exit(1);
    }

    // Copy loop (byte-by-byte)
    while (1) {
        ch = fgetc(input_file);
        if (ch == (char)0xFF)  // EOF check (buggy!)
            break;

        write(fd, &ch, 1);
    }

    // Log end
    log_wrapper(log_file, "[END] ", argv[1]);

    fclose(input_file);
    close(fd);

    return 0;
}
