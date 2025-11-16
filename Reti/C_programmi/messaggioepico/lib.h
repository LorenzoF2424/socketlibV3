#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <semaphore.h>
#include <math.h>
#include <time.h>


#define PORT 7777
#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024
#pragma pack(push, 1)   // disable the architecture padding
typedef struct {
   char sender[32];
   char receiver[32];    // fixed-size sender name
   char message[256];  // fixed-size message
   uint8_t msg_type;   // 0=text, 1=file, 2=command
   uint32_t timestamp; // unix timestamp
} ChatMessage;          // total 302 bytes

// ... other messages structures

#pragma pack(pop)       // re-enable the architecture padding


typedef struct {
   int sockfd;
   struct sockaddr_in address;
   char nome[32];
   char password[32];
   ChatMessage cronologia[500];
   int cronologia_index;
} client_info;

// shared data structure with mutex
typedef struct {
   client_info *clients[MAX_CLIENTS];
   int count;
   pthread_mutex_t mutex;
} client_list;

client_list clients = { .count = 0 };


typedef struct {

    int sock_fd;

}tharg;

typedef struct {
    char logline[256];
}logl;

void broadcast_message(const ChatMessage message, int sender_fd) {
   pthread_mutex_lock(&clients.mutex);
  
   for(int i = 0; i < clients.count; i++) {
       if(clients.clients[i]->sockfd != sender_fd) {
            clients.clients[i]->cronologia[clients.clients[i]->cronologia_index]=message;
           send(clients.clients[i]->sockfd, (void*)&message, sizeof(ChatMessage), 0);
       }
   }
  
   pthread_mutex_unlock(&clients.mutex);
}

void send_message_to(const ChatMessage message, int sender_fd) {
   pthread_mutex_lock(&clients.mutex);
  printf("%s\n",message.receiver);
   for(int i = 0; i < clients.count; i++) {
       if((strcmp(clients.clients[i]->nome,message.receiver))==0) {
        printf("test if\n");
            clients.clients[i]->cronologia[clients.clients[i]->cronologia_index]=message;
           send(clients.clients[i]->sockfd, (void*)&message, sizeof(ChatMessage), 0);
       }
   }
  
   pthread_mutex_unlock(&clients.mutex);
}


void *handle_client(void *arg) {
   client_info *info = (client_info *)arg;
   int client_sock = info->sockfd;
   ChatMessage msg;
   char client_ip[INET_ADDRSTRLEN];
  
   inet_ntop(AF_INET, &info->address.sin_addr, client_ip, INET_ADDRSTRLEN);
   int client_port = ntohs(info->address.sin_port);

   printf("New connection from %s:%d\n", client_ip, client_port);

   // Add client to list
   pthread_mutex_lock(&clients.mutex);
   clients.clients[clients.count]->cronologia_index = 0;
   clients.clients[clients.count] = info;
   clients.count++;

   pthread_mutex_unlock(&clients.mutex);

   while (true) {
       ssize_t bytes_received = recv(client_sock, (void*)&msg, sizeof(ChatMessage), 0);
       if(bytes_received <= 0) {
           if(bytes_received == 0)
               printf("Client %s:%d disconnected\n", client_ip, client_port);
           else perror("recv failed");
           break;
       }
       pthread_mutex_lock(&clients.mutex);
        strcpy(info->nome,msg.sender);
        info->cronologia[info->cronologia_index]=msg;
          info->cronologia_index++;

        pthread_mutex_unlock(&clients.mutex);
       
      
       // format message: [IP:Port] message
       
        msg.sender[strcspn(msg.sender, "\n")] = '\0';
        msg.receiver[strcspn(msg.sender, "\n")] = '\0';
        msg.message[strcspn(msg.sender, "\n")] = '\0';

       printf("[%s]: %s",msg.sender,msg.receiver);
       send_message_to(msg, client_sock);

       if(strcmp(msg.message, "exit\n") == 0) {
           break;
       }
   }

    // remove client from list
   pthread_mutex_lock(&clients.mutex);
   for(int i = 0; i < clients.count; i++) {
       if(clients.clients[i]->sockfd == client_sock) {
           free(clients.clients[i]);
           for(int j = i; j < clients.count - 1; j++) {
               clients.clients[j] = clients.clients[j + 1];
           }
           clients.count--;
           break;
       }
   }
   pthread_mutex_unlock(&clients.mutex);

   close(client_sock);
   return NULL;
}

void* receive_handler(void* arg) {
    tharg *args = (tharg *)arg;
   ChatMessage message;
   for(;;) {
       ssize_t bytes = recv(args->sock_fd, (void*)&message, sizeof(ChatMessage), 0);
       if(bytes <= 0) {
           if(bytes == 0)
               printf("\nServer closed connection\n");
           else
               perror("Receive error");
           exit(1);
       }
       printf("\n[%s]: %s\n> ",message.sender, message.message);
       fflush(stdout);
   }
   return NULL;
}

void* send_handler(void* arg) {
        tharg *args = (tharg*)arg;
    ChatMessage message;
    printf("Inserisci nome: ");
    if(!fgets(message.sender, sizeof(message.sender), stdin)) perror("Errore: fgets fallito");

   while (true) {
       printf("> ");
       fflush(stdout);
      
       if(!fgets(message.message, sizeof(message.message), stdin)) break;
      
       printf(">Inserisci destinatario: ");
        if(!fgets(message.receiver, sizeof(message.receiver), stdin)) perror("Errore: fgets fallito");
       if(strcmp(message.message, "exit") == 0) {
           send(args->sock_fd, (void*)&message, sizeof(ChatMessage), 0);
           close(args->sock_fd);
           exit(0);
       }
      
       if(send(args->sock_fd, (void*)&message, sizeof(ChatMessage), 0) < 0) {
           perror("Send failed");
           exit(1);
       }
   }
   return NULL;
}
