#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 5

// Global array to store moving sum
unsigned long long int moving_sum[SIZE] = {0}; // Update the data type to unsigned long long int

// Structure to hold data passed to thread function
typedef struct {
    int number;
    int index;
} ThreadData;

// Mutex to synchronize access to critical section
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to calculate factorial
unsigned long long int factorial(int n) { // Update the return type to unsigned long long int
    unsigned long long int result = 1; // Update the data type to unsigned long long int
    for (int i = 1; i <= n; ++i) {
        result *= i;
    }
    return result;
}

// Thread function
void *calculate_factorial(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    unsigned long long int fact = factorial(data->number); // Update the data type to unsigned long long int
    unsigned long long int prev_sum = 0; // Update the data type to unsigned long long int
    
    // Lock mutex
    pthread_mutex_lock(&mutex);
    
    // If index is greater than 0, get previous value from moving_sum
    if (data->index > 0) {
        prev_sum = moving_sum[data->index - 1];
    }
    
    // Calculate sum and store in moving_sum
    moving_sum[data->index] = fact + prev_sum;
    
    // Unlock mutex
    pthread_mutex_unlock(&mutex);
    
    free(arg); // Free allocated memory for ThreadData
    
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[SIZE];
    int numbers[SIZE];
    
    // Prompt user for five numbers
    printf("Enter five numbers:\n");
    for (int i = 0; i < SIZE; ++i) {
        scanf("%d", &numbers[i]);
    }
    
    // Create threads
    for (int i = 0; i < SIZE; ++i) {
        ThreadData *data = (ThreadData *)malloc(sizeof(ThreadData));
        if (data == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        data->number = numbers[i];
        data->index = i;
        pthread_create(&threads[i], NULL, calculate_factorial, (void *)data);
    }
    
    // Join threads
    for (int i = 0; i < SIZE; ++i) {
        pthread_join(threads[i], NULL);
    }
    
    // Print moving sum
    printf("Moving sum:\n");
    for (int i = 0; i < SIZE; ++i) {
        printf("%llu ", moving_sum[i]); // Update the format specifier to %llu
    }
    printf("\n");
    
    // Destroy mutex
    pthread_mutex_destroy(&mutex);
    
    return 0;
}
