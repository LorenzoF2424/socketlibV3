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
#include "structure.h"

int createServer(uint16_t port, int max_client) {

   int sS;
   struct sockaddr_in sin;
   if ((sS = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
   }

   memset(&sin, 0, sizeof(sin));
   sin.sin_family = AF_INET;
   sin.sin_port = htons(port);
   sin.sin_addr.s_addr = htonl(INADDR_ANY);

   // Bind and listen
   int opt=1;
   setsockopt(sS, SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt));
   if (bind(sS, (struct sockaddr*)&sin, sizeof(sin)) < 0) {
        close(sS);
        perror("bind failed");
        exit(EXIT_FAILURE);
   }



   return sS;
}

#define MAX_CLIENTS 10
typedef struct {
   struct sockaddr_in address;
   char mittente[30];
} client_info;

// shared data structure with mutex
typedef struct {
   client_info *client[MAX_CLIENTS];
   int count;
   pthread_mutex_t mutex;
} client_list;

client_list clients = { .count = 0 };
bool addr_equal(struct sockaddr_in *a, struct sockaddr_in *b) {
    return a->sin_family == b->sin_family &&
           a->sin_port == b->sin_port &&
           a->sin_addr.s_addr == b->sin_addr.s_addr;
}
void broadcast_message(client_info *client, chatMessage *message, int sFD) {
   pthread_mutex_lock(&clients.mutex);
   for(int i = 0; i < clients.count; i++) {
       if(!addr_equal(&clients.client[i]->address, &client->address)) {
           sendto(sFD, message, sizeof(*message), 0, (struct sockaddr*)&clients.client[i]->address, sizeof(clients.client[i]->address));
       }
   }
  
   pthread_mutex_unlock(&clients.mutex);
}




int BUFFER_SIZE=256;

void handle_client(client_info *client, int sFD) {

    
    
    chatMessage message;
    char client_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &client->address.sin_addr, client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client->address.sin_port);

    printf("New connection from %s:%d\n", client_ip, client_port);


    while (true) {

        socklen_t lenC = sizeof(client->address);
        ssize_t bytes_received = recvfrom(sFD, &message, sizeof(message), 0, (struct sockaddr*)&client->address, &lenC);
        if(bytes_received <= 0) {
            if(bytes_received == 0)
                printf("Client %s:%d disconnected\n", client_ip, client_port);
            else perror("recv failed");
            break;
        }

       


         // format message: [IP:Port] message
      
        if (message.comando==1) { // 1 = inserimento

            bool esiste=false;
            for (int i =0; i<clients.count;i++) {

                if(addr_equal(&client->address, &clients.client[i]->address)) {
                    esiste = true;
                    break;
                }
            }

            if (!esiste && clients.count < MAX_CLIENTS) {
                clients.client[clients.count] = malloc(sizeof(client_info));
                clients.client[clients.count]->address.sin_addr = client->address.sin_addr;
                clients.client[clients.count]->address.sin_family = client->address.sin_family;
                clients.client[clients.count]->address.sin_port = client->address.sin_port;
                strncpy(clients.client[clients.count]->mittente,message.mittente,sizeof(clients.client[clients.count]->mittente) - 1);
                clients.client[clients.count]->mittente[sizeof(clients.client[clients.count]->mittente) - 1] = '\0';

                clients.count++;
            }


      
        } else if (message.comando==2) { // 2 = cancellazione

            int index=0;
                bool esiste=false;
                for (int i =0; i<clients.count;i++) {

                    if(addr_equal(&client->address, &clients.client[i]->address)) {
                    esiste = true;
                    index = i;
                    break;
                    }
                }

            if (esiste) {
                free(clients.client[index]);
                for(int i = index; i < clients.count-1; i++) {
                   
                        clients.client[i] = clients.client[i + 1];
                    
                }
                clients.count--;
            }

        } else {

            broadcast_message(client, &message, sFD);

        }
        


        
    }
    
   



   
   


    
}

