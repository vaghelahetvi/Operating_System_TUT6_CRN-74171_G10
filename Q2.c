#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    // Creating a child process
    pid_t pid = fork();

    if (pid < 0) {
        // If fork fails
        fprintf(stderr, "Oops! Something went wrong while creating a child process.\n");
        return 1;
    } else if (pid == 0) {
        // Child process
        // Sleep for 1 second
        sleep(1);
        // Printing from child process
        printf("Hey, I'm the child process!\n");
        // Exiting child process
        exit(0);
    } else {
        // Parent process
        int status;
        // Waiting for the child process to finish
        wait(&status);
        // Checking if child process terminated normally
        if (WIFEXITED(status)) {
            // Printing from parent process
            printf("Hello from the parent process!\n");
            // Printing exit status of child process
            printf("The child process exited with status: %d\n", WEXITSTATUS(status));
        } else {
            // Printing if child process terminated abnormally
            printf("The child process terminated abnormally.\n");
        }
    }

    return 0;
}
