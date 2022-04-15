/****************
Code by group 2:
KOZHIN Assan
SALTER Glenn
TOROMANOVIC Jovan
****************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <time.h>

#define COUNTER_SIZE 9321
#define TNUM 4

int K;
int counter[COUNTER_SIZE]; // global counter array
long start_timestamp; // minimal timestamp supplied as argv

struct thread_args
{
    int *counter;
    char **filenames;
    int start;
    int end;
};

typedef struct thread_args ThreadArgs;

// Convert timestamp to a required format and store it s
static void time_string(time_t t, char *s)
{
    // sample testcase
    // input: 1645491600
    // output: Tue Feb 22 09:00:00 2022
    struct tm *tm = localtime(&t);
    strftime(s, 25, "%c", tm);
}

#define COMPARE(v, a, b) ((v[a] != v[b]) ? (v[a] < v[b]) : (a < b))

// Sink item i in the min-heap
static void heapify(int *heap, int *counter, int i, const int n)
{
    // K - global variable defining size of heap
    int smallest = i;
    int l;
    int r;
    int temp;
heapify_loop:
    l = 2 * i + 1;
    r = 2 * i + 2;
    if (l < n && COMPARE(counter, heap[l], heap[smallest]))
        smallest = l;
    if (r < n && COMPARE(counter, heap[r], heap[smallest]))
        smallest = r;
    if (smallest == i)
        return;
    temp = heap[i];
    heap[i] = heap[smallest];
    heap[smallest] = temp;
    // swap(&heap[i], &heap[smallest]);
    i = smallest;
    goto heapify_loop;
}

// Build up initial heap of K elements
static void buildHeap(int *heap, int *counter)
{
    // Calling Heapify for all non leaf nodes
    // K - global variable denoting size of heap
    for (int i = (K / 2) - 1; i >= 0; i--)
    {
        heapify(heap, counter, i, K);
    }
}

// TopK procedure places indices(pseudotimestamps) in heap in descending order
void TopK(int *counter, int *heap)
{
    // K - global variable denoting size of heap
    // First build heap from first K elements
    for (int i = 0; i < K; i++)
    {
        heap[i] = i;
    }
    buildHeap(heap, counter);
    // Now add and check elements one-by-one
    for (int i = K; i != COUNTER_SIZE; i++)
    {
        if (COMPARE(counter, heap[0], i))
        {
            // insert into heap only if larger than smallest element
            heap[0] = i;
            heapify(heap, counter, 0, K);
        }
    }

    // Need to sort K items in descdending order.
    // Removed qsort because of compatability, should run at least 0.1 ms faster
    // qsort(heap, K, sizeof(int), cmpfunc);
    int temp;
    for (int i = K - 1; i >= 1; i--)
    {
        temp = heap[0];
        heap[0] = heap[i];
        heap[i] = temp;
        heapify(heap, counter, 0, i);
    }
}

#define PARSE_FIRST_DIGIT \
    time_stamp = *at++ - '0';
#define PARSE_NEXT_DIGIT \
    time_stamp = time_stamp * 10 + *at++ - '0';

#define BUFFER_SIZE 40

// Reading, parsing and incrementing count in counter for a single file
void processfile(char *filename, int *counter)
{
    FILE *input = fopen(filename, "r");
    if (!input)
    {
        printf("process file->err:%d\n", errno);
        exit(errno);
    }
    char buffer[BUFFER_SIZE + 1];
    long time_stamp;
    char *at;
    while (fgets(buffer, BUFFER_SIZE, input) != NULL)
    {
        at = buffer;
        // loop unrolling
        PARSE_FIRST_DIGIT
        PARSE_NEXT_DIGIT
        PARSE_NEXT_DIGIT
        PARSE_NEXT_DIGIT
        PARSE_NEXT_DIGIT
        PARSE_NEXT_DIGIT
        PARSE_NEXT_DIGIT
        PARSE_NEXT_DIGIT
        time_stamp *= 100;

        ++counter[(unsigned)(time_stamp - start_timestamp) / 3600];
    }
    fclose(input);
}

/*Multithreaded version of processfile,
processes number of files instead of sinlge file*/
void *processfiles(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    int *localcounter = args->counter;
    int end = args->end;
    char **filenames = args->filenames;
    char buffer[BUFFER_SIZE + 1];
    long time_stamp;
    char *at;
    for (int i = args->start; i < end; i++)
    {
        FILE *input = fopen(filenames[i], "r");
        if (!input)
        {
            printf("process files->err:%d\n", errno);
            exit(errno);
        }
        while (fgets(buffer, BUFFER_SIZE, input) != NULL)
        {
            at = buffer;
            // loop unrolling
            PARSE_FIRST_DIGIT
            PARSE_NEXT_DIGIT
            PARSE_NEXT_DIGIT
            PARSE_NEXT_DIGIT
            PARSE_NEXT_DIGIT
            PARSE_NEXT_DIGIT
            PARSE_NEXT_DIGIT
            PARSE_NEXT_DIGIT
            time_stamp *= 100;

            ++localcounter[(unsigned)(time_stamp - start_timestamp) / 3600];
            // Can ignore string part of the line since it is not used
        }
        fclose(input);
    }
    return 0;
}

/*procedure to distribute files to threads, start them and finalize result*/
void startThreads(int file_count, char **filenames)
{
    int *localcounters[TNUM];
    ThreadArgs arglist[TNUM];
    pthread_t threads[TNUM];
    int blocksize = (file_count + TNUM - 1) / TNUM; // ceil(file_count / TNUM);
    int start = 0;
    int i = 0;
    for (; i < TNUM; i++)
    {
        // calloc sets all bytes to 0
        localcounters[i] = (int *)calloc(COUNTER_SIZE, sizeof(int));
        arglist[i].filenames = filenames;
        arglist[i].start = start;
        start += blocksize;
        arglist[i].end = start;
        arglist[i].counter = localcounters[i];
    }
    arglist[i - 1].end = file_count;

    // DO NOT merge 2 loops for creating and wait. It is slower for some reason.
    for (i = 0; i < TNUM; i++)
    {
        pthread_create(&threads[i], NULL, &processfiles, &arglist[i]);
    }
    for (i = 0; i < TNUM; i++)
    {
        pthread_join(threads[i], NULL);
    }
    // loop unrolling doesn't help here
    for (i = 0; i < TNUM; i++)
    {
        for (int idx = 0; idx < COUNTER_SIZE; idx++)
            counter[idx] += localcounters[i][idx];
    }
}

int main(int argc, char **argv)
{
    char dirname[40]; // To store directory
    strcpy(dirname, argv[1]);
    start_timestamp = atol(argv[2]);
    K = atoi(argv[3]);

    // set counter to all 0 to avoid unpredictable behavior
    memset(counter, 0, COUNTER_SIZE);

    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);
    // Count the number of files
    int file_count = 0;
    while ((dir = readdir(d)) != NULL)
    {
        // skip . and ..
        if (dir->d_name[0] == '.')
            continue;
        file_count++;
    }
    closedir(d);

    // Assume maximum path len is 59 characters.
    // Dynamically allocate to match the number of files in directory.
    // Filenames are actually paths to the files, but it is easier to
    // name them as filenames
    char **filenames = (char **)malloc(file_count * sizeof(filenames));
    int i = 0;
    d = opendir(dirname);
    while ((dir = readdir(d)) != NULL)
    {
        // skip . and ..
        if (dir->d_name[0] == '.')
            continue;
        filenames[i] = (char *)malloc(60 * (sizeof(char *)));
        filenames[i][0] = '\0';
        strcat(filenames[i], dirname);
        strcat(filenames[i], dir->d_name);
        i++;
    }
    closedir(d);

    if (file_count < 4)
    {
        // If file count < 4 it is faster to process files sequentially(assuming they are quite small).
        // We assume 1 or many files if 3 large files then this is not effective.
        for (int i = 0; i < file_count; i++)
            processfile(filenames[i], counter);
    }
    else
    {
        startThreads(file_count, (char **)filenames);
    }

    // Do top K here.
    int topK[K];
    TopK(counter, topK);

    // Do output here.
    char temp[40];
    printf("Top K frequently accessed hour:\n");
    for (int i = 0; i < K; i++)
    {
        time_string(start_timestamp + topK[i] * 3600, temp);
        printf("%s\t%d\n", temp, counter[topK[i]]);
    }
    return 0;
}
