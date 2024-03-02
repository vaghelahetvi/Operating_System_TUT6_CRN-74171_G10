#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_THREADS 5

// Global variable to store total sum
int total_sum = 0;

// Semaphore for thread-safe access to total_sum
sem_t semaphore;

// Function to calculate factorial
void *factorial(void *arg) {
    int number = *((int *)arg);
    int fact = 1;
    
    // Calculate factorial
    for (int i = 1; i <= number; ++i) {
        fact *= i;
    }

    // Increment total_sum in a thread-safe manner
    sem_wait(&semaphore);
    total_sum += fact;
    sem_post(&semaphore);

    // Print current factorial value and calculated factorial
    printf("Factorial of %d: %d\n", number, fact);
    printf("Current total sum: %d\n", total_sum);

    pthread_exit(NULL);
}

int main() {
    // Open file for writing
    FILE *file = fopen("numbers.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    // Prompt user for five numbers and write them to the file
    int numbers[NUM_THREADS];
    printf("Enter five numbers:\n");
    for (int i = 0; i < NUM_THREADS; ++i) {
        scanf("%d", &numbers[i]);
        fprintf(file, "%d\n", numbers[i]);
    }

    // Close the file
    fclose(file);

    // Fork a child process
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (pid == 0) { // Child process
        // Open file for reading
        FILE *file = fopen("numbers.txt", "r");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

        // Read numbers from the file and create threads
        pthread_t threads[NUM_THREADS];
        for (int i = 0; i < NUM_THREADS; ++i) {
            fscanf(file, "%d", &numbers[i]);
            pthread_create(&threads[i], NULL, factorial, (void *)&numbers[i]);
        }

        // Close the file
        fclose(file);

        // Wait for all threads to finish
        for (int i = 0; i < NUM_THREADS; ++i) {
            pthread_join(threads[i], NULL);
        }

        // Open file for writing
        file = fopen("sum.txt", "w");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

        // Write total_sum to the file
        fprintf(file, "%d\n", total_sum);

        // Close the file
        fclose(file);

        return 0;
    } else { // Parent process
        // Wait for the child process to terminate
        wait(NULL);

        // Open file for reading
        file = fopen("sum.txt", "r");
        if (file == NULL) {
            perror("Error opening file");
            return 1;
        }

        // Read total_sum from the file and print it
        fscanf(file, "%d", &total_sum);
        printf("Total sum: %d\n", total_sum);

        // Close the file
        fclose(file);

        return 0;
    }
}