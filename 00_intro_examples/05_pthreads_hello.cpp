// Slide reference: SLIDE 11 - Pthreads and OpenMP
// Introductory example: Pthreads basics
// pthread_create starts a new thread; pthread_join waits for it to finish.

#include <cstdio>
#include <pthread.h>

void* sayHello(void* arg) {
    int id = *(int*)arg;
    printf("Hello from pthread %d\n", id);
    return nullptr;
}

int main() {
    const int NUM_THREADS = 4;
    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        ids[i] = i;
        pthread_create(&threads[i], nullptr, sayHello, &ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], nullptr);
    }

    return 0;
}
