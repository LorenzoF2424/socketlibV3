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
#include "structure.h"



int tFD=0;
void* riceviMessaggi(void *arg) {
     
     chatMessage message;
     char mittente[30];
     strncpy(mittente,(char*)arg,sizeof(mittente));
     //int index = (int)&arg;
     while (true) {

          // !!!! la funzione recv(recieve) è una funzione bloccante(il processo si blocca finche non gli arriva qualcosa) !!!!
          int byteLetti = 0;
          if ((byteLetti=recv(tFD, &message, sizeof(message), 0))<0) {
               close(tFD);
               perror("Errore: spero che santoro esploda");
               exit(EXIT_FAILURE);
          } else if (byteLetti==0) {
               printf("\nServer: Connessione chiusa\n");
               break;
          }
          printf("<%s> %s", message.mittente, message.messaggio);
          fflush(stdout);
          //printf("<%s> ",mittente);

     }

     


     return NULL;
}

void* mandaMessaggi(void* arg) {

     char mittente[30];
     strncpy(mittente,(char*)arg,sizeof(mittente));
     //printf("Mittente: %s",mittente);
     chatMessage message;
   //int index = (int)&arg;
   while (true) {
       printf("> ");
       fflush(stdout);
      
       if(!fgets(message.messaggio, sizeof(message.messaggio), stdin)) break;
          /*strncpy(message.mittente,(char*)arg,strlen(message.mittente));
          printf("Mittente: %s",message.mittente);
          message.mittente[strlen(message.mittente)-1] = '\0';*/

          snprintf(message.mittente, sizeof(message.mittente), "%s", (char*)arg);
          message.timestamp=time(NULL);
       if(strcmp(message.messaggio, "exit") == 0) {
           send(tFD, &message, sizeof(message), 0);
           close(tFD);
           exit(0);
       }
      
       if(send(tFD, &message, sizeof(message), 0) < 0) {
           perror("Send failed");
           exit(1);
       }
   }
   return NULL;


}

