#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define MSG_SIZE 1024

char mensagem[MSG_SIZE];
int lido = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* writer_thread(void* arg) {
    pthread_mutex_lock(&mutex);
    strcpy(mensagem, "Olá, comunicação via THREAD!");
    printf("Escritor (thread): Mensagem escrita.\n");
    lido = 0;
    pthread_cond_signal(&cond); // Notifica o leitor
    while (!lido) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    printf("Escritor (thread): Leitura confirmada. Finalizado.\n");
    return NULL;
}

void* reader_thread(void* arg) {
    pthread_mutex_lock(&mutex);
    while (strlen(mensagem) == 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    printf("Leitor (thread): Mensagem lida: \"%s\"\n", mensagem);
    lido = 1;
    pthread_cond_signal(&cond); // Notifica o escritor
    pthread_mutex_unlock(&mutex);
    return NULL;
}

int main() {
    pthread_t writer, reader;

    pthread_create(&writer, NULL, writer_thread, NULL);
    pthread_create(&reader, NULL, reader_thread, NULL);

    pthread_join(writer, NULL);
    pthread_join(reader, NULL);

    return 0;
}
