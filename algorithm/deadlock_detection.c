#include <stdio.h>
#include <stdbool.h>

#define MAX_PROCESSES 10
#define MAX_RESOURCES 10

// Function to check if a process can be completed
bool canComplete(int process, int available[], int need[][MAX_RESOURCES], int num_resources) {
    for (int i = 0; i < num_resources; i++) {
        if (need[process][i] > available[i]) {
            return false;
        }
    }
    return true;
}

// Function to detect deadlock
void detectDeadlock(int processes, int resources, int allocation[][MAX_RESOURCES], int max[][MAX_RESOURCES], int available[], FILE *logFile) {
    int need[MAX_PROCESSES][MAX_RESOURCES];
    bool finished[MAX_PROCESSES] = {false};
    int safe_sequence[MAX_PROCESSES];
    int count = 0;

    // Calculate the Need matrix
    for (int i = 0; i < processes; i++) {
        for (int j = 0; j < resources; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }

    // Try to find a safe sequence
    while (count < processes) {
        bool found = false;
        for (int i = 0; i < processes; i++) {
            if (!finished[i] && canComplete(i, available, need, resources)) {
                for (int j = 0; j < resources; j++) {
                    available[j] += allocation[i][j];
                }
                safe_sequence[count++] = i;
                finished[i] = true;
                found = true;
            }
        }
        if (!found) {
            // Print deadlock message to terminal and log file
            printf("System is in deadlock. Deadlocked processes: ");
            fprintf(logFile, "System is in deadlock. Deadlocked processes: ");
            for (int i = 0; i < processes; i++) {
                if (!finished[i]) {
                    printf("P%d ", i);
                    fprintf(logFile, "P%d ", i);
                }
            }
            printf("\n");
            fprintf(logFile, "\n");
            return;
        }
    }

    // Print safe sequence if no deadlock was detected
    printf("No deadlock detected. Safe sequence is: ");
    fprintf(logFile, "No deadlock detected. Safe sequence is: ");
    for (int i = 0; i < processes; i++) {
        printf("P%d ", safe_sequence[i]);
        fprintf(logFile, "P%d ", safe_sequence[i]);
    }
    printf("\n");
    fprintf(logFile, "\n");
}

