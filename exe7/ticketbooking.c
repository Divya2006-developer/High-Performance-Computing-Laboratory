#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_SEATS 10

int available_seats = MAX_SEATS;
int request = 0;
int request_flag = 0;
int keep_running = 1; 

pthread_mutex_t mutex;
pthread_cond_t cond;

void* user_thread(void* arg) {
    while (keep_running) {
        
        pthread_mutex_lock(&mutex);
        if (available_seats <= 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);

        int seats;
        printf("\n[User] Enter number of seats to book: ");
        if (scanf("%d", &seats) <= 0) break;

        pthread_mutex_lock(&mutex);
        if (!keep_running) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        request = seats;
        request_flag = 1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

        
        usleep(500000); 
    }
    return NULL;
}

void* booking_thread(void* arg) {
    while (keep_running) {
        pthread_mutex_lock(&mutex);

        while (request_flag == 0 && keep_running) {
            pthread_cond_wait(&cond, &mutex);
        }

        if (!keep_running) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        if (request <= available_seats && request > 0) {
            available_seats -= request;
            printf("[Booking]: Confirmed %d seats. (Left: %d)\n", request, available_seats);
        } else if (request > available_seats) {
            printf("[Booking]: Failed! Only %d seats available.\n", available_seats);
        }

        request_flag = 0;

        if (available_seats <= 0) {
            printf("\n--- ALL SEATS BOOKED. SYSTEM CLOSING. ---\n");
            keep_running = 0;
            pthread_cond_broadcast(&cond); 
        }

        pthread_mutex_unlock(&mutex);
    }
    return NULL;
}

void* display_thread(void* arg) {
    while (keep_running) {
        pthread_mutex_lock(&mutex);
        if (available_seats > 0) {
            printf("\n--- Current Status: %d seats remaining ---\n", available_seats);
        }
        pthread_mutex_unlock(&mutex);

       
        for(int i=0; i<50 && keep_running; i++) usleep(100000); 
    }
    return NULL;
}

int main() {
    pthread_t user, booking, display;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    printf("Welcome to the Seat Booking System\n");
    printf("Total seats available: %d\n", MAX_SEATS);

    pthread_create(&booking, NULL, booking_thread, NULL);
    pthread_create(&display, NULL, display_thread, NULL);
    pthread_create(&user, NULL, user_thread, NULL);

 
    pthread_join(user, NULL);
    pthread_join(booking, NULL);
    pthread_join(display, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    printf("Program exited successfully.\n");

    return 0;
}
