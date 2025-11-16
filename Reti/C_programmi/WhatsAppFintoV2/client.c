#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "clientlib.h"

int main(int argc, char **argv) {

    if (argc!=4) {
        perror("Errore: numero argomenti errato");
        exit(EXIT_FAILURE);
    }

   
    if ((tFD=socket(AF_INET, SOCK_STREAM, 0))<0) {
        perror("Errore: buone le patate");
        exit(EXIT_FAILURE);
    }

    char ip[16];
    strcpy(ip, argv[1]);
    int port = atoi(argv[2]);
    //printf("torrisi dove sei1\n");

    memset(&SAI, 0, sizeof(SAI));

    SAI = (struct sockaddr_in) {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };

    if ((inet_pton(AF_INET, ip, &SAI.sin_addr))<=0) {
        close(tFD);
        perror("Errore: buona la brioche");
        exit(EXIT_FAILURE);
    }
      //printf("torrisi dove sei2\n");
      

    if (connect(tFD, (struct sockaddr*)&SAI, sizeof(SAI))<0) {
        close(tFD);
        perror("Errore: buona la granita");
        exit(EXIT_FAILURE);
    }

    printf("Connected to %s:%s\n", argv[1], argv[2]);
    printf("Type messages (enter 'exit' to quit):\n");

    pthread_t mandaT,riceviT;
    char mittente[30];
    strncpy(mittente, argv[3],sizeof(mittente));

    pthread_create(&mandaT, NULL, mandaMessaggi, (void*)mittente);
    pthread_create(&riceviT, NULL, riceviMessaggi, (void*)mittente);


    pthread_join(mandaT, NULL);
    pthread_join(riceviT, NULL);

    close(tFD);
    printf("\n");
    return 0;
}