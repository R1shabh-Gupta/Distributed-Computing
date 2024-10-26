#include <stdio.h>
#define N 11 

void update_clock(int sender_process_clocks[], int receiver_process_clocks[], int sending_time[]) {
    int senderMessageSize = sender_process_clocks[sending_time[0]];
    
      int receiverMessageSize = receiver_process_clocks[sending_time[1]];
    
    if (senderMessageSize <= receiverMessageSize) {
        // continue
    } 
    else {
        int diff_value = (senderMessageSize + 1) - receiverMessageSize;
        
        for (int i = sending_time[1]; i < N; i++) {
            receiver_process_clocks[i] += diff_value;
        }
    }
}

int main() {
    int p1[N] = {0, 6, 12, 18, 24, 30, 36, 42, 48, 54, 60};  
    int p2[N] = {0, 8, 16, 24, 32, 40, 48, 56, 64, 72, 80}; 
    int p3[N] = {0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100}; 
    
    printf("Initial Logical Clocks:\n");
    printf("P1: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", p1[i]);
    }
    printf("\nP2: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", p2[i]);
    }
    printf("\nP3: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", p3[i]);
    }
    printf("\n");
    printf("\n");
    
    // int num_messages;
    // printf("Enter the number of messages to send: ");
    // scanf("%d", &num_messages);
    
    // int messages[num_messages][4];
    
    // printf("Enter the sender and receiver process indices, and their time indices for each message:\n");
    // for (int i = 0; i < num_messages; i++) 
    // {
    //     printf("Message %d (sender process || receiver process || sender index || receiver index): ", i + 1);
    //     scanf("%d %d %d %d", &messages[i][0], &messages[i][1], &messages[i][2], &messages[i][3]);
    // }
    
    // Process each message
    // for (int i = 0; i < num_messages; i++) {
    //     int sender = messages[i][0];
    //     int receiver = messages[i][1];
        
    //     // Determine the sender and receiver process clocks
    //     int* sender_process_clocks;
    //     int* receiver_process_clocks;
        
    //     switch (sender) {
    //         case 1: sender_process_clocks = p1; break;
    //         case 2: sender_process_clocks = p2; break;
    //         case 3: sender_process_clocks = p3; break;
    //         default:
    //             printf("Invalid sender process %d.\n", sender);
    //             continue;
    //     }
        
    //     switch (receiver) {
    //         case 1: receiver_process_clocks = p1; break;
    //         case 2: receiver_process_clocks = p2; break;
    //         case 3: receiver_process_clocks = p3; break;
    //         default:
    //             printf("Invalid receiver process %d.\n", receiver);
    //             continue;
    //     }
        
    //     int sending_time[] = {messages[i][2], messages[i][3]};
    //     update_clock(sender_process_clocks, receiver_process_clocks, sending_time);
    // }


    int m1[] = {1, 2};
    int m2[] = {3, 4};
    int m3[] = {6, 7};
    int m4[] = {8, 9};

    update_clock(p1, p2, m1);
    update_clock(p2, p3, m2);
    update_clock(p3, p2, m3);
    update_clock(p2, p1, m4);

    printf("Updated Logical Clocks:\n");
    printf("P1: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", p1[i]);
    }
    printf("\nP2: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", p2[i]);
    }
    printf("\nP3: ");
    for (int i = 0; i < N; i++) {
        printf("%d ", p3[i]);
    }
    printf("\n");

    return 0;
}