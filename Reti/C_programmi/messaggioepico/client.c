#include "lib.h"



int main(int argc, char **argv) {
   int sock_fd;
   struct sockaddr_in address;
   char buffer[BUFFER_SIZE];
    tharg arg;

   // initialize mutex
   pthread_mutex_init(&clients.mutex, NULL);

   // create sock socket
   if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
       perror("socket failed");
       exit(1);
   }

   address.sin_family = AF_INET;
   address.sin_addr.s_addr = inet_addr("127.0.0.1");
   address.sin_port = htons(PORT);

   // bind socket
   if(connect(sock_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        close(sock_fd);
       perror("connect failed");
       exit(1);
   }

   // listen for connections
   arg.sock_fd=sock_fd;
   pthread_t thread_id;
    printf("Connected to 127.0.0.1:%d\n", PORT);

    pthread_t send_thread, recv_thread;
   pthread_create(&recv_thread, NULL, receive_handler, (void*)&arg);
   pthread_create(&send_thread, NULL, send_handler, (void*)&arg);

   pthread_join(send_thread, NULL);
   pthread_join(recv_thread, NULL);


   close(sock_fd);
   pthread_mutex_destroy(&clients.mutex);
   return 0;
}