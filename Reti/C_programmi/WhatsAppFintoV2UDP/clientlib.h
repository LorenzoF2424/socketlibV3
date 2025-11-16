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
bool finito=false;
void* riceviMessaggi(void *arg) {
     
     chatMessage message;
     char mittente[30];
     strncpy(mittente,(char*)arg,sizeof(mittente));
     struct sockaddr_in from;
     socklen_t len = sizeof(from);
     //int index = (int)&arg;
     while (!finito) {

          // !!!! la funzione recv(recieve) è una funzione bloccante(il processo si blocca finche non gli arriva qualcosa) !!!!
          //int byteLetti = 0;
          int bytes=recvfrom(tFD, &message, sizeof(message), 0, (struct sockaddr*)&from, &len);
          if (bytes <= 0) continue;  // Non stampare se non valido

          
          printf("<%s> %s\n> ", message.mittente, message.messaggio);
          fflush(stdout);
          //printf("<%s> ",mittente);

     }


     return NULL;
}
struct sockaddr_in server_addr;
void* mandaMessaggi(void* arg) {

     char mittente[30];
     strncpy(mittente,(char*)arg,sizeof(mittente));
     //printf("Mittente: %s",mittente);
     chatMessage message;
     //socklen_t len = sizeof(server_addr);
   //int index = (int)&arg;

   while (!finito) {
       printf("> ");
       fflush(stdout);
      
       if(!fgets(message.messaggio, sizeof(message.messaggio), stdin)) break;
          /*strncpy(message.mittente,(char*)arg,strlen(message.mittente));
          printf("Mittente: %s",message.mittente);
          message.mittente[strlen(message.mittente)-1] = '\0';*/
          message.messaggio[strcspn(message.messaggio, "\n")] = '\0';

          snprintf(message.mittente, sizeof(message.mittente), "%s", (char*)arg);
          message.timestamp=time(NULL);
          message.comando=0;
       if(strcmp(message.messaggio, "exit") == 0) {
          finito=true;
          message.comando=2;

       }
      
       sendto(tFD, &message, sizeof(message), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
           
       
   }
   return NULL;


}

