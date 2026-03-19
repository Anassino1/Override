#include <stdio.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

int main() {
    pid_t pid;
    char buffer[160];   // approx (0xb0 stack - other vars)
    int status;
    int syscall;

    pid = fork();

    if (pid == 0) {
        //  CHILD PROCESS

        // Prevent tracing from outside
        prctl(1, 1);

        // Allow parent to trace this process
        ptrace(PTRACE_TRACEME, 0, 0, 0);

        puts("Give me some shellcode, k");

        // 🚨 VULNERABILITY
        gets(buffer);

    } else {
        //  PARENT PROCESS

        wait(&status);

        // Check if child exited normally
        if ((status & 0x7f) != 0) {
            puts("child is exiting...");
        } else {

            // Get syscall number
            syscall = ptrace(PTRACE_PEEKUSER, pid, 44, 0);

            //  Detect execve
            if (syscall == 11) {
                puts("no exec() for you");
                kill(pid, 9);
            }
        }
    }

    return 0;
}