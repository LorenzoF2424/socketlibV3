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
   if ((sS = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
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

   if (listen(sS, max_client) < 0) {
        close(sS);
        perror("listen failed");
        exit(EXIT_FAILURE);
   }

   return sS;
}

#define MAX_CLIENTS 10
typedef struct {
   int sockfd;
   struct sockaddr_in address;
} client_info;

// shared data structure with mutex
typedef struct {
   client_info *client[MAX_CLIENTS];
   int count;
   pthread_mutex_t mutex;
} client_list;

client_list clients = { .count = 0 };

void broadcast_message(chatMessage *message, int sender_fd) {
   pthread_mutex_lock(&clients.mutex);
  
   for(int i = 0; i < clients.count; i++) {
       if(clients.client[i]->sockfd != sender_fd) {
           send(clients.client[i]->sockfd, message, sizeof(*message), 0);
       }
   }
  
   pthread_mutex_unlock(&clients.mutex);
}

int BUFFER_SIZE=256;
void* handle_client(void *arg) {

    client_info *client = (client_info*)arg;
    int sFD = client->sockfd;
    chatMessage message;
    char client_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &client->address.sin_addr, client_ip, INET_ADDRSTRLEN);
    int client_port = ntohs(client->address.sin_port);

    printf("New connection from %s:%d\n", client_ip, client_port);

    pthread_mutex_lock(&clients.mutex);
    clients.client[clients.count++] = client;
    pthread_mutex_unlock(&clients.mutex);

    while (true) {


        ssize_t bytes_received = recv(sFD, &message, sizeof(message), 0);
        if(bytes_received <= 0) {
            if(bytes_received == 0)
                printf("Client %s:%d disconnected\n", client_ip, client_port);
            else perror("recv failed");
            break;
        }

       


         // format message: [IP:Port] message
      
      
       
       broadcast_message(&message, sFD);

        if(strcmp(message.messaggio, "exit\n") == 0) {
            break;
        }


        
    }
    
    pthread_mutex_lock(&clients.mutex);
        for(int i = 0; i < clients.count; i++) {
            if(clients.client[i]->sockfd == sFD) {
                free(clients.client[i]);
                for(int j = i; j < clients.count - 1; j++) {
                    clients.client[j] = clients.client[j + 1];
                }
                clients.count--;
                break;
            }

        }

    pthread_mutex_unlock(&clients.mutex);



    close(sFD);
   


    return NULL;
}
