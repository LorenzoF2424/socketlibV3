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


#pragma pack(push, 1)

typedef struct {
    char mittente[30];
    char messaggio[256];
    int type;
    long timestamp;

}chatMessage;

#pragma pack(pop)