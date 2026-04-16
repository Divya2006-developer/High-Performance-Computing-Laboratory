#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define BUFFER_SIZE 5
#define MAX_TASKS 20

typedef struct {
    int num1;
    int num2;
    char op;
} Task;

Task buffer[BUFFER_SIZE];
int in = 0, out = 0, count = 0, tasks_created = 0, tasks_processed = 0;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t cond_not_empty = PTHREAD_COND_INITIALIZER;

FILE *result_file;

double compute(Task t) {
    switch (t.op) {
        case '+': return t.num1 + t.num2;
        case '-': return t.num1 - t.num2;
        case '*': return t.num1 * t.num2;
        case '/': return (t.num2 != 0) ? (double)t.num1 / t.num2 : 0;
    }
    return 0;
}

void* producer(void* arg) {
    char ops[] = {'+', '-', '*', '/'};
    while (tasks_created < MAX_TASKS) {
        Task newTask;
        newTask.num1 = rand() % 100;
        newTask.num2 = rand() % 100;
        newTask.op = ops[rand() % 4];

        pthread_mutex_lock(&mutex);
        while (count == BUFFER_SIZE) {
            pthread_cond_wait(&cond_not_full, &mutex);
        }

        buffer[in] = newTask;
        in = (in + 1) % BUFFER_SIZE;
        count++;
        tasks_created++;



        pthread_cond_broadcast(&cond_not_empty);
        pthread_mutex_unlock(&mutex);
        usleep(100000);
    }
    pthread_cond_broadcast(&cond_not_empty);
    return NULL;
}

void* consumer(void* arg) {
    char myOp = *(char*)arg;
    while (1) {
        pthread_mutex_lock(&mutex);

        while (tasks_processed < MAX_TASKS && (count == 0 || buffer[out].op != myOp)) {
            pthread_cond_wait(&cond_not_empty, &mutex);
            if (tasks_processed >= MAX_TASKS) break;
        }

        if (tasks_processed >= MAX_TASKS) {
            pthread_cond_broadcast(&cond_not_empty);
            pthread_mutex_unlock(&mutex);
            break;
        }

        Task t = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--;
        tasks_processed++;

        pthread_cond_broadcast(&cond_not_full);
        pthread_cond_broadcast(&cond_not_empty);
        pthread_mutex_unlock(&mutex);

        double result = compute(t);
        fprintf(result_file, " %d %c %d = %.2lf\n",t.num1, t.op, t.num2, result);
        fflush(result_file);
        printf("Worker %c processed: %d %c %d = %.2lf\n", myOp, t.num1, t.op, t.num2, result);
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    result_file = fopen("result.txt", "w");

    pthread_t prod, t1, t2, t3, t4;
    static char op1 = '+', op2 = '-', op3 = '*', op4 = '/';

    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&t1, NULL, consumer, &op1);
    pthread_create(&t2, NULL, consumer, &op2);
    pthread_create(&t3, NULL, consumer, &op3);
    pthread_create(&t4, NULL, consumer, &op4);

    pthread_join(prod, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);

    fclose(result_file);
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond_not_full);
    pthread_cond_destroy(&cond_not_empty);


    return 0;
}
