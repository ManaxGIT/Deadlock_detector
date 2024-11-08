#ifndef DEADLOCK_DETECTION_H
#define DEADLOCK_DETECTION_H

void detectDeadlock(int processes, int resources, int allocation[][10], int max[][10], int available[], FILE *logFile);

#endif

