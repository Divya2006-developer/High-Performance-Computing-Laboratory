#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5
#define MAX_WORD_LEN 50
#define NUM_THREADS 3
#define DICT_SIZE 6

char buffer[BUFFER_SIZE][MAX_WORD_LEN];
char result_buffer[1000][100]; 

int count = 0;       
int in = 0, out = 0;
int result_count = 0;
int last_printed = 0; 
int keep_running = 1; 


const char *dictionary[DICT_SIZE] = {
    "apple", "banana", "grape", "orange", "mango", "pear"
};

pthread_mutex_t mutex;
pthread_cond_t not_full, not_empty;

int check_word(const char *word) {
    for (int i = 0; i < DICT_SIZE; i++) {
        if (strcmp(word, dictionary[i]) == 0)
            return 1;
    }
    return 0;
}

void* spell_check(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex);
        while (count == 0 && keep_running) {
            pthread_cond_wait(&not_empty, &mutex);
        }
        if (!keep_running && count == 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        char word[MAX_WORD_LEN];
        strcpy(word, buffer[out]);
        out = (out + 1) % BUFFER_SIZE;
        count--;

        pthread_cond_signal(&not_full);
        pthread_mutex_unlock(&mutex);

        char result[100];
        if (check_word(word))
            sprintf(result, "'%s' -> Correct", word);
        else
            sprintf(result, "'%s' -> Incorrect", word);

        pthread_mutex_lock(&mutex);
        strcpy(result_buffer[result_count++], result);
        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* master(void* arg) {
    char input[500];

    while (1) {
        usleep(100000); 

        printf("\n[Master] Enter a word/sentence (type 'exit' to quit): ");
        fflush(stdout); 

        if (fgets(input, sizeof(input), stdin) == NULL) break;
        input[strcspn(input, "\n")] = 0; 

        if (strcmp(input, "exit") == 0) {
            pthread_mutex_lock(&mutex);
            keep_running = 0;
            pthread_cond_broadcast(&not_empty);
            pthread_mutex_unlock(&mutex);
            break;
        }

        char *token = strtok(input, " ");
        while (token != NULL) {
            pthread_mutex_lock(&mutex);
            while (count == BUFFER_SIZE) {
                pthread_cond_wait(&not_full, &mutex);
            }
            strncpy(buffer[in], token, MAX_WORD_LEN - 1);
            in = (in + 1) % BUFFER_SIZE;
            count++;
            pthread_cond_signal(&not_empty);
            pthread_mutex_unlock(&mutex);
            token = strtok(NULL, " ");
        }
    }
    return NULL;
}

void* server(void* arg) {
    while (keep_running || last_printed < result_count) {
        pthread_mutex_lock(&mutex);
        if (last_printed < result_count) {
            printf("[Server Result]: %s\n", result_buffer[last_printed]);
            last_printed++;
        }
        pthread_mutex_unlock(&mutex);
        usleep(10000); 
    }
    return NULL;
}

int main() {

    printf("====================================\n");
    printf("WELCOME TO SPELL CHECKER\n");
    printf("VALID DICTIONARY WORDS:\n");
    for (int i = 0; i < DICT_SIZE; i++) {
        printf(" - %s\n", dictionary[i]);
    }
    printf("====================================\n");

    pthread_t master_thread, workers[NUM_THREADS], server_thread;
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&not_full, NULL);
    pthread_cond_init(&not_empty, NULL);

    pthread_create(&master_thread, NULL, master, NULL);
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&workers[i], NULL, spell_check, NULL);
    }
    pthread_create(&server_thread, NULL, server, NULL);


    pthread_join(master_thread, NULL);
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(workers[i], NULL);
    }
    pthread_join(server_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&not_full);
    pthread_cond_destroy(&not_empty);

    return 0;
}
