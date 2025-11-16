#include "lib.h"

int main(int argc, char **argv) {
   int server_fd;
   struct sockaddr_in address;

   // initialize mutex
   pthread_mutex_init(&clients.mutex, NULL);

   // create server socket
   if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
       perror("socket failed");
       exit(1);
   }

   address.sin_family = AF_INET;
   address.sin_addr.s_addr = INADDR_ANY;
   address.sin_port = htons(PORT);

   // bind socket
   if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        close(server_fd);
       perror("bind failed");
       exit(1);
   }

   // listen for connections
   if(listen(server_fd, MAX_CLIENTS) < 0) {
        close(server_fd);
       perror("listen");
       exit(1);
   }
   printf("Chat server listening on port %d...\n", PORT);



   while (true) {
       client_info *client = malloc(sizeof(client_info));
       socklen_t addrlen = sizeof(client->address);
      
       // accept new connection
       if((client->sockfd = accept(server_fd, (struct sockaddr *)&client->address, &addrlen)) < 0) {
           perror("accept");
           free(client);
           continue;
       }

       // Create thread for new client
       pthread_t thread_id;
       if(pthread_create(&thread_id, NULL, handle_client, (void *)client) != 0) {
           perror("pthread_create");
           close(client->sockfd);
           free(client);
       } else {
           pthread_detach(thread_id);
       }
   }

   close(server_fd);
   pthread_mutex_destroy(&clients.mutex);
   return 0;
}






/*

Client in C/C++ o Python, in TCP
Registrazione e login al server Centrale
Il client Una volta loggato può scrivere a chi inviare un messaggio tra gli utenti registrati(che siano connessi o meno) 

Server Centrale in C/C++
Si occupa di gestire il login e la registrazione TCP
Se un utente registrato riceve messaggi li salva e quando ritorna attivo glieli manda (una casella di posta) TCP
Invia messaggi di log(tipo X ha inviato un messaggio a Y, X si è registrato, etc) al server di log in UDP

Server di log
Riceve i messaggi e li salva in un file


*/