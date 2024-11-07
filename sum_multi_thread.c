#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

typedef struct {
    int n;
    int numThreads;
    int k;
    unsigned long long* result;
} ThreadData;

void* sum_partial(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    unsigned long long s = 0;
    for (int i = ((data->k * data->n) / data->numThreads) + 1; i <= ((data->k + 1) * data->n) / data->numThreads; i++) {
        s += i;
    }
    *(data->result) = s;
    return NULL;
}

int main(int argc, char* argv[]) {
    int numThreads = atoi(argv[1]);
    int n = atoi(argv[2]);
    if (numThreads <= 0 || n <= 0) {
        return 1;
    }

    pthread_t* thread = malloc(numThreads * sizeof(pthread_t));
    ThreadData* threadData = malloc(numThreads * sizeof(ThreadData));

    unsigned long long* results = malloc(numThreads * sizeof(unsigned long long));

    for (int i = 0; i < numThreads; i++) {
        threadData[i].n = n;
        threadData[i].numThreads = numThreads;
        threadData[i].k = i;
        threadData[i].result = &results[i];

        pthread_create(&thread[i], NULL, sum_partial, &threadData[i]);
    }

    unsigned long long total_sum = 0;
    for (int i = 0; i < numThreads; i++) {
        pthread_join(thread[i], NULL);
        total_sum += results[i];
    }

    printf("%llu\n", total_sum);

    free(thread);
    free(threadData);
    free(results);

    return 0;
}

