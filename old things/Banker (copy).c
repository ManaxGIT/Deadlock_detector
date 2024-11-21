#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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

void logRunDetails(FILE *logFile, const char *inputFileName) {
    // Print header for the new run
    fprintf(logFile, "\n--- New Run ---\n");

    // Get current date and time
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char dateTime[30];
    strftime(dateTime, 30, "%Y-%m-%d %H:%M:%S", tm_info);

    // Print date, time, and input file name
    fprintf(logFile, "Date and Time: %s\n", dateTime);
    fprintf(logFile, "Input File: %s\n", inputFileName);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    int num_processes, num_resources;
    int allocation[MAX_PROCESSES][MAX_RESOURCES];
    int max[MAX_PROCESSES][MAX_RESOURCES];
    int available[MAX_RESOURCES];

    // Open the input file provided as argument
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", argv[1]);
        return 1;
    }

    // Open the log file in append mode
    FILE *logFile = fopen("detect.log", "a");
    if (logFile == NULL) {
        printf("Error opening log file.\n");
        fclose(file);
        return 1;
    }

    // Log run details
    logRunDetails(logFile, argv[1]);

    // Read the number of processes and resources
    fscanf(file, "%d %d", &num_processes, &num_resources);

    // Read the Allocation matrix
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) {
            fscanf(file, "%d", &allocation[i][j]);
        }
    }

    // Read the Max matrix
    for (int i = 0; i < num_processes; i++) {
        for (int j = 0; j < num_resources; j++) {
            fscanf(file, "%d", &max[i][j]);
        }
    }

    // Read the Available array
    for (int i = 0; i < num_resources; i++) {
        fscanf(file, "%d", &available[i]);
    }

    fclose(file);

    // Detect deadlock and print results to both terminal and log file
    detectDeadlock(num_processes, num_resources, allocation, max, available, logFile);

    // Close the log file
    fclose(logFile);

    return 0;
}

