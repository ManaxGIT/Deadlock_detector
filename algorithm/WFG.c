#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>

#define MAX_PROCESSES 100  // Max number of processes

// Function prototypes
void detectDeadlock(int graph[MAX_PROCESSES][MAX_PROCESSES], int numProcesses, FILE *logFile);
bool dfs(int graph[MAX_PROCESSES][MAX_PROCESSES], int numProcesses, int process, bool visited[], bool recStack[], FILE *logFile);

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        printf("Usage: %s [-n] <input_file>\n", argv[0]);
        return 1;
    }

    bool newLog = false;
    char *inputFileName;

    if (argc == 3 && strcmp(argv[1], "-n") == 0) {
        newLog = true;
        inputFileName = argv[2];
    } else {
        inputFileName = argv[1];
    }

    int numProcesses;
    int graph[MAX_PROCESSES][MAX_PROCESSES];
    FILE *inputFile, *logFile;

    // Open input file from argument
    inputFile = fopen(inputFileName, "r");
    if (inputFile == NULL) {
        printf("Error: Unable to open input file.\n");
        return 1;
    }

    // Handle logging
    if (newLog) {
        // Create the "Logging" folder if it doesn't exist
        mkdir("Logging", 0777);

        // Generate the log file name with datetime
        time_t now = time(NULL);
        struct tm *timeinfo = localtime(&now);
        char logFileName[50];
        strftime(logFileName, sizeof(logFileName), "Logging/wfg_log_%Y-%m-%d_%H-%M-%S.txt", timeinfo);

        logFile = fopen(logFileName, "w");
        if (logFile == NULL) {
            printf("Error: Unable to create log file: %s\n", logFileName);
            fclose(inputFile);
            return 1;
        }
    } else {
        // Open log file in append mode
        logFile = fopen("detect.log", "a");
        if (logFile == NULL) {
            printf("Error: Unable to open or create log file.\n");
            fclose(inputFile);
            return 1;
        }
    }

    // Log run details
    time_t now = time(NULL);
    struct tm *timeinfo = localtime(&now);
    char dateTime[20];
    strftime(dateTime, sizeof(dateTime), "%Y-%m-%d %H:%M:%S", timeinfo);

    fprintf(logFile, "\n--- New Run ---\n");
    fprintf(logFile, "Date and Time: %s\n", dateTime);
    fprintf(logFile, "Input File: %s\n", inputFileName);

    // Read the number of processes
    fscanf(inputFile, "%d", &numProcesses);

    // Read the wait-for graph as an adjacency matrix
    for (int i = 0; i < numProcesses; i++) {
        for (int j = 0; j < numProcesses; j++) {
            fscanf(inputFile, "%d", &graph[i][j]);
        }
    }

    fclose(inputFile);

    // Detect deadlock and log results
    detectDeadlock(graph, numProcesses, logFile);

    fclose(logFile);

    return 0;
}

void detectDeadlock(int graph[MAX_PROCESSES][MAX_PROCESSES], int numProcesses, FILE *logFile) {
    bool visited[MAX_PROCESSES] = {false};
    bool recStack[MAX_PROCESSES] = {false};
    bool deadlockDetected = false;

    for (int i = 0; i < numProcesses; i++) {
        if (!visited[i]) {
            if (dfs(graph, numProcesses, i, visited, recStack, logFile)) {
                deadlockDetected = true;
            }
        }
    }

    if (deadlockDetected) {
        printf("Deadlock detected.\n");
        fprintf(logFile, "Deadlock detected.\n");
    } else {
        printf("No deadlock detected.\n");
        fprintf(logFile, "No deadlock detected.\n");
    }
}

bool dfs(int graph[MAX_PROCESSES][MAX_PROCESSES], int numProcesses, int process, bool visited[], bool recStack[], FILE *logFile) {
    if (!visited[process]) {
        // Mark the current node as visited and add to recursion stack
        visited[process] = true;
        recStack[process] = true;

        // Recur for all the vertices adjacent to this vertex
        for (int i = 0; i < numProcesses; i++) {
            if (graph[process][i] == 1) { // If there's an edge from process to i
                if (!visited[i] && dfs(graph, numProcesses, i, visited, recStack, logFile)) {
                    fprintf(logFile, "Cycle detected involving process %d\n", process);
                    printf("Cycle detected involving process %d\n", process);
                    return true;
                } else if (recStack[i]) {
                    fprintf(logFile, "Cycle detected involving process %d\n", process);
                    printf("Cycle detected involving process %d\n", process);
                    return true;
                }
            }
        }
    }

    // Remove the vertex from recursion stack
    recStack[process] = false;
    return false;
}

