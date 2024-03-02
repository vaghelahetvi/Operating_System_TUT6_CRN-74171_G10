#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    // Creating two child processes
    pid_t pid1, pid2;

    pid1 = fork();

    if (pid1 < 0) {
        fprintf(stderr, "Oops! Something went wrong with the first child process.\n");
        return 1;
    } else if (pid1 == 0) {
        // First Child Process
        FILE *file1 = fopen("child1.txt", "w");
        if (file1 == NULL) {
            perror("Error opening file");
            exit(1);
        }
        fprintf(file1, "Hello from Child 1!\n");
        fclose(file1);
        exit(0);
    }

    // Parent process

    pid2 = fork();

    if (pid2 < 0) {
        fprintf(stderr, "Oops! Something went wrong with the second child process.\n");
        return 1;
    } else if (pid2 == 0) {
        // Second Child Process
        FILE *file2 = fopen("child2.txt", "w");
        if (file2 == NULL) {
            perror("Error opening file");
            exit(1);
        }
        fprintf(file2, "Hello from Child 2!\n");
        fclose(file2);
        exit(0);
    }

    // Parent process waits for both children to finish
    wait(NULL);
    wait(NULL);

    // Children are done writing files, now they will read and print the contents

    pid_t child1_pid = fork();

    if (child1_pid < 0) {
        fprintf(stderr, "Oops! Something went wrong with the first child reading process.\n");
        return 1;
    } else if (child1_pid == 0) {
        // Child 1 reads and prints the content of child1.txt
        sleep(1);
        FILE *file1 = fopen("child1.txt", "r");
        if (file1 == NULL) {
            perror("Error opening file");
            exit(1);
        }
        char line[100];
        fgets(line, sizeof(line), file1);
        printf("Child 1 says: %s", line);
        fclose(file1);
        exit(0);
    }

    pid_t child2_pid = fork();

    if (child2_pid < 0) {
        fprintf(stderr, "Oops! Something went wrong with the second child reading process.\n");
        return 1;
    } else if (child2_pid == 0) {
        // Child 2 reads and prints the content of child2.txt
        sleep(1);
        FILE *file2 = fopen("child2.txt", "r");
        if (file2 == NULL) {
            perror("Error opening file");
            exit(1);
        }
        char line[100];
        fgets(line, sizeof(line), file2);
        printf("Child 2 says: %s", line);
        fclose(file2);
        exit(0);
    }

    // Parent process waits for both children to finish reading and printing
    wait(NULL);
    wait(NULL);

    return 0;
}
