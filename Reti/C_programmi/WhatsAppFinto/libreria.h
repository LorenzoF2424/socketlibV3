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


int BUFFER_SIZE=256;
int tFD=0;
void* riceviMessaggi(void *arg) {
    
     char buffer[BUFFER_SIZE];
     //int index = (int)&arg;
     while (true) {

          // !!!! la funzione recv(recieve) è una funzione bloccante(il processo si blocca finche non gli arriva qualcosa) !!!!
          int byteLetti = 0;
          if ((byteLetti=recv(tFD, buffer, BUFFER_SIZE-1, 0))<0) {
               close(tFD);
               perror("Errore: spero che santoro esploda");
               exit(EXIT_FAILURE);
          } else if (byteLetti==0) {
               printf("\nServer: Connessione chiusa\n");
               break;
          }

          buffer[byteLetti] = '\0';
          printf("%s\n", buffer);
          fflush(stdout);
     }




     return NULL;
}

void* mandaMessaggi(void* arg) {

   char buffer[BUFFER_SIZE];
   //int index = (int)&arg;
   while (true) {
       printf("> ");
       fflush(stdout);
      
       if(!fgets(buffer, BUFFER_SIZE, stdin)) break;
      
       buffer[strcspn(buffer, "\n")] = '\0';
       if(strcmp(buffer, "exit") == 0) {
           send(tFD, buffer, strlen(buffer), 0);
           close(tFD);
           exit(0);
       }
      
       if(send(tFD, buffer, strlen(buffer), 0) < 0) {
           perror("Send failed");
           exit(1);
       }
   }
   return NULL;


}

