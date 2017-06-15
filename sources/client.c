#include "client.h"

Client_t initClient(int i, Client_t client){
    client.ID = i;
    client.zone = get_random(ZONE);
    printf("Client ID est de %d, et sa zone est %d\n", client.ID, client.zone);
    return client;
}

void* fonction_client(void* arg){
    int i = (int) arg;
    pthread_mutex_lock(&client[i].mClient);
    pthread_mutex_unlock(&client[i].mClient);
    pthread_exit(NULL);
}