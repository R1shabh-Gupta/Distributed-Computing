#include <stdio.h>
#include <stdlib.h>

#define MAX 10

int processes[MAX];
int n;

void initiateElection(int initiator) {
    printf("Process %d initiated an election.\n", initiator + 1);
    int higherProcess = -1;

    for (int i = initiator + 1; i < n; i++) {
        if (processes[i] > processes[initiator]) {
            higherProcess = i;
            printf("Process %d sends an election message to Process %d.\n", initiator + 1, i + 1);
        }
    }

    if (higherProcess == -1) {
        printf("Process %d becomes the new coordinator.\n", initiator + 1);
        for (int i = 0; i < initiator; i++) {
            printf("Process %d informs Process %d that it is the new coordinator.\n", initiator + 1, i + 1);
        }
    } else {
        initiateElection(higherProcess);
    }
}

void startElection(int failedProcess) {
    printf("\nProcess %d failed.\n", failedProcess + 1);

    int initiator = -1;
    for (int i = failedProcess - 1; i >= 0; i--) {
        if (processes[i] != -1) {
            initiator = i;
            break;
        }
    }

    if (initiator != -1) {
        initiateElection(initiator);
    } else {
        printf("No processes available to initiate an election.\n");
    }
}

int main() {
    printf("Enter the number of processes: ");
    scanf("%d", &n);

    printf("Enter the process IDs (in ascending order):\n");
    for (int i = 0; i < n; i++) {
        scanf("%d", &processes[i]);
    }

    int failedProcess;
    printf("Enter the process number that failed (1 to %d): ", n);
    scanf("%d", &failedProcess);

    startElection(failedProcess - 1);

    return 0;
}