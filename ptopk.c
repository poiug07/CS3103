#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>

#include <stdint.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define COUNTER_SIZE 9323

int K;
int counter[COUNTER_SIZE];
pthread_mutex_t counter_lock;
char dirname[40];     // To store directory
long start_timestamp; // minimal timestamp supplied as argv

struct thread_args
{
    char **filenames;
    int *counter;
    int filecount;
    int start;
    int end;
};

typedef struct thread_args ThreadArgs;

void time_string(time_t t, char *s)
{
    // sample testcase
    // input: 1645491600
    // output: Tue Feb 22 09:00:00 2022
    struct tm *tm = localtime(&t);
    strftime(s, 25, "%c", tm);
}

long get_file_length(const char *file_name)
{
    struct stat sb;
    if (stat(file_name, &sb) == -1)
    {
        exit(-1);
    }
    return sb.st_size;
}

int compare_value_and_time(int *values, int t1, int t2)
{
    // Returns negative if value of values on t1 is smaller than t2
    // if values same, returns negative value if t1 smaller than t2
    if (values[t1] != values[t2])
        return values[t1] - values[t2];
    else
        return t1 - t2;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(int *heap, int *counter, int i)
{
    // K - global variable defining size of heap
    int largest = i;
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    if (l < K && compare_value_and_time(counter, heap[l], heap[largest]) < 0)
        largest = l;
    if (r < K && compare_value_and_time(counter, heap[r], heap[largest]) < 0)
        largest = r;
    if (largest != i)
    {
        swap(&heap[i], &heap[largest]);
        heapify(heap, counter, largest);
    }
}

void buildHeap(int *heap, int *counter)
{
    // Calling Heapify for all non leaf nodes
    // K - global variable denoting size of heap
    for (int i = K / 2 - 1; i >= 0; i--)
    {
        heapify(heap, counter, i);
    }
}

int cmpfunc(const void *a, const void *b)
{
    // requires global counter value to be set
    return -compare_value_and_time(counter, *(int *)a, *(int *)b);
}

void TopK(int *counter, int *heap)
{
    // K - global variable denoting size of heap
    for (int i = 0; i < K; i++)
    {
        heap[i] = i;
    }
    buildHeap(heap, counter);
    for (int i = K; i < COUNTER_SIZE; i++)
    {
        if (compare_value_and_time(counter, heap[0], i) < 0)
        {
            heap[0] = i;
            heapify(heap, counter, 0);
        }
    }

    // this should put values in descending order
    // I think it is just faster to use qsort,
    // but maybe wrong. TODO: Need to check.
    qsort(heap, K, sizeof(int), cmpfunc);
}

static void parse_block(int *localcounter, char *buffer, char *tail, int block_size, char *value_string)
{
    int parsed_posi = 0;
    int val_po = 0;
    long time_stamp;
    // parse the first entry
    while (tail[val_po] != '\0')
    {
        value_string[val_po] = tail[val_po];
        val_po++;
    }
    while (buffer[parsed_posi] != ',')
    {
        value_string[val_po] = buffer[parsed_posi];
        parsed_posi++;
        val_po++;
    }

    char *temp;
    value_string[val_po] = '\0';
    time_stamp = atol(value_string + val_po - 10);
    localcounter[(time_stamp - start_timestamp) / 3600]++;
    val_po = 0;
    parsed_posi++;

    char current;
    while (parsed_posi < block_size)
    {
        current = buffer[parsed_posi];
        if (current == ',')
        {
            // former part is the target_value
            buffer[parsed_posi] = '\0';
            time_stamp = atol(buffer + parsed_posi - 10);
            localcounter[(time_stamp - start_timestamp) / 3600]++;
            value_string[0] = '\0';
            parsed_posi += 5;
        }
        tail[val_po] = current;
        if (current == '\n')
        {
            // when it swtich the line
            val_po = -1; // reset it again
        }
        // move the index
        val_po++;
        parsed_posi++;
    }
    tail[val_po] = '\0';
}

#define PARSE_FIRST_DIGIT  \
    if (*at >= '0')        \
    {                      \
        val = *at++ - '0'; \
    }                      \
    else                   \
    {                      \
        goto done;         \
    }
#define PARSE_NEXT_DIGIT              \
    if (*at >= '0')                   \
    {                                 \
        val = val * 10 + *at++ - '0'; \
    }                                 \
    else                              \
    {                                 \
        goto done;                    \
    }

#define TNUM 2
#define BLK_SIZE 4096

static void
parse_chunk(char *at, const char *end, int *counter)
{
    long val = 0;
    while (at < end)
    {
        val = strtol(at, &at, 10);
    // goto done;
    //     // Parse up to 10 digits.
    //     // PARSE_FIRST_DIGIT;
    //     // PARSE_NEXT_DIGIT;
    //     // PARSE_NEXT_DIGIT;
    //     // PARSE_NEXT_DIGIT;
    //     // PARSE_NEXT_DIGIT;
    //     // PARSE_NEXT_DIGIT;
    //     // PARSE_NEXT_DIGIT;
    //     // PARSE_NEXT_DIGIT;
    //     // PARSE_NEXT_DIGIT;
    //     // PARSE_NEXT_DIGIT;
    // done:
        __sync_fetch_and_add(&counter[(val - start_timestamp) / 3600], 1);
        // skip letters
        at += 4;
        while (*at != '\n')
            at++;
        // Skip newline character.
        at++;
    }
}

typedef struct
{
    char *chunk_start;
    char *chunk_end;
    int *counter;
} parse_chunk_thread_args;

static void *
parse_chunk_thread(void *args)
{
    parse_chunk_thread_args *a = (parse_chunk_thread_args *)args;
    parse_chunk(a->chunk_start, a->chunk_end, a->counter);
    return NULL;
}

static void parse_chunks_single_thread(char *filename, int *localcounter) {
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        printf("process file->err:%d\n", errno);
        exit(errno);
    }
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        printf("process file->err:%d\n", errno);
        exit(errno);
    }
    uint64_t n_bytes = sb.st_size;
    char *buf_start = mmap(NULL, n_bytes, PROT_READ, MAP_PRIVATE, fd, 0);
    char *buf_end = buf_start + n_bytes;
    parse_chunk(buf_start, buf_end, localcounter);

    munmap(buf_start, n_bytes);
}

void processfile(char *filename, int *localcounter)
{
    int fd = open(filename, O_RDONLY);
    if (fd == -1)
    {
        printf("process file->err:%d\n", errno);
        exit(errno);
    }
    struct stat sb;
    if (fstat(fd, &sb) == -1)
    {
        printf("process file->err:%d\n", errno);
        exit(errno);
    }
    uint64_t n_bytes = sb.st_size;
    char *buf_start = mmap(NULL, n_bytes, PROT_READ, MAP_PRIVATE, fd, 0);
    char *buf_end = buf_start + n_bytes;

    char *chunks[TNUM];
    for (int i = 0; i < TNUM; i++)
    {
        chunks[i] = buf_start + (n_bytes / TNUM) * i;
        if (i > 0)
        {
            // Adjust the chunks starting points until they reach past
            // a newline.
            while (*chunks[i] != '\n')
            {
                chunks[i]++;
            }
            chunks[i]++;
        }
    }

    int *counter = calloc(COUNTER_SIZE, sizeof(int));
    pthread_t threads[TNUM];
    parse_chunk_thread_args args[TNUM];
    for (int i = 0; i < TNUM; i++)
    {
        char *chunk_start = chunks[i];
        char *chunk_end = buf_end;
        if (i < TNUM - 1)
        {
            chunk_end = chunks[i + 1];
        }
        args[i].chunk_start = chunk_start;
        args[i].chunk_end = chunk_end;
        args[i].counter = localcounter;
        pthread_create(&threads[i], NULL, parse_chunk_thread, &args[i]);
    }
    for (int i = 0; i < TNUM; i++)
    {
        pthread_join(threads[i], NULL);
    }
    for(int i=0; i<COUNTER_SIZE; i++){
        localcounter[i] += counter[i];
    }

    free(counter);
    munmap(buf_start, n_bytes);
}

void *processfiles(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    int *localcounter = args->counter;
    int start = args->start;
    int end = args->end;
    char **filenames = args->filenames;
    for (int i = start; i != end; ++i)
    {
        parse_chunks_single_thread(filenames[i], localcounter);
    }

    return 0;
}

void startThreads(int file_count, char **filenames)
{
    int *localcounters[TNUM];
    ThreadArgs arglist[file_count];
    pthread_t threads[TNUM];
    int blocksize = file_count / TNUM;
    int i = 0;
    int start = 0;
    for (; i < TNUM; i++)
    {
        localcounters[i] = (int *)calloc(COUNTER_SIZE, sizeof(int));
        arglist[i].filenames = filenames;
        arglist[i].start = start;
        start += blocksize;
        arglist[i].end = start;
        arglist[i].counter = localcounters[i];
        arglist[i].filecount = file_count;
    }
    arglist[TNUM - 1].end = file_count;

    // DO NOT merge 2 loops for creating and wait. It is slower for some reason.
    for (i = 0; i < TNUM; i++)
    {
        pthread_create(&threads[i], NULL, &processfiles, &arglist[i]);
    }
    for (i = 0; i < TNUM; i++)
    {
        pthread_join(threads[i], NULL);
    }
    for (i = 0; i < TNUM; i++)
    {
        for (int idx = 0; idx < COUNTER_SIZE; idx++)
            counter[idx] += localcounters[i][idx];
    }
}

int main(int argc, char **argv)
{
    strcpy(dirname, argv[1]);
    start_timestamp = atol(argv[2]);
    K = atoi(argv[3]);

    memset(counter, 0, COUNTER_SIZE);

    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);
    // Count the number of files
    int file_count = 0;
    while ((dir = readdir(d)) != NULL)
    {
        if (!strcmp(dir->d_name, "."))
            continue;
        if (!strcmp(dir->d_name, ".."))
            continue;
        file_count++;
    }
    closedir(d);

    // Combine these two loops
    char **filenames = (char **)malloc(file_count * sizeof(char *));
    for (int i = 0; i < file_count; i++)
    {
        filenames[i] = (char *)malloc(60 * (sizeof(char *)));
        filenames[i][0] = '\0';
    }
    int i = 0;
    d = opendir(dirname);
    while ((dir = readdir(d)) != NULL)
    {
        if (!strcmp(dir->d_name, "."))
            continue;
        if (!strcmp(dir->d_name, ".."))
            continue;
        // printf("%s\n", dir->d_name);
        strcat(filenames[i], dirname);
        strcat(filenames[i], dir->d_name);
        i++;
    }
    closedir(d);

    if (file_count < 4) {
    // if (1) {
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
        // Can do printing faster
        // https://stackoverflow.com/questions/5975378/fastest-way-to-print-a-certain-number-of-characters-to-stdout-in-c
        time_string((time_t)start_timestamp + topK[i] * 3600, temp);
        printf("%s\t%d\n", temp, counter[topK[i]]);
    }

    return 0;
}
