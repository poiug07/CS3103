#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>
#include <stdio.h>
#include <math.h>

#define COUNTER_SIZE 9321
#define TNUM 4

int K;
int counter[COUNTER_SIZE];
// pthread_mutex_t counter_lock;
long start_timestamp; // minimal timestamp supplied as argv

struct thread_args {
    int * counter;
    char** filenames;
    // int filecount;
    int start;
    int end;
};

typedef struct thread_args ThreadArgs;

static void time_string(time_t t, char* s) {
    // sample testcase
    // input: 1645491600
    // output: Tue Feb 22 09:00:00 2022
    struct tm *tm = localtime(&t);
    strftime(s, 25, "%c", tm);
}

// static int compare_value_and_time(int *values, int t1, int t2)
// {
//     // Returns negative if value of values on t1 is smaller than t2
//     // if values same, returns negative value if t1 smaller than t2
//     if (values[t1] != values[t2])
//         return values[t1] - values[t2];
//     else
//         return t1 - t2;
// }

#define COMPARE(v, a, b) ((v[a]!=v[b])?(v[a]<v[b]):(a<b))

void swap(int *a, int *b) {
        int temp = *a;
        *a = *b;
        *b = temp;
}

static void heapify(int* heap, int* counter, int i) {
        // K - global variable defining size of heap
        int largest = i;
        int l; 
        int r;
        heapify_loop:
        l = 2*i+1;
        r = 2*i+2;
        if(l<K && COMPARE(counter, heap[l], heap[largest]))
                largest = l;
        if(r<K && COMPARE(counter, heap[r], heap[largest]))
                largest = r;
        if(largest==i) return;
        swap(&heap[i], &heap[largest]);
        i = largest;
        goto heapify_loop;
        // heapify(heap, counter, largest);
}

static void buildHeap(int *heap, int *counter) {
    // Calling Heapify for all non leaf nodes
    // K - global variable denoting size of heap
    for (int i = (K/2) - 1; i >= 0; i--) {
        heapify(heap, counter, i);
    }
}

int cmpfunc(const void * a, const void * b) {
    // requires global counter value to be set
    int t1 = *(int*)a;
    int t2 = *(int*)b;
    if (counter[t1] != counter[t2])
        return counter[t1] < counter[t2];
    else
        return t1 < t2;
//    return  -compare_value_and_time(counter, *(int*)a, *(int*)b);
}

void TopK(int *counter, int *heap)
{
        // K - global variable denoting size of heap
        for(int i=0; i!=K; i++) {
                heap[i] = i;
        }
        buildHeap(heap, counter);
        for(int i=K; i!=COUNTER_SIZE; i++) {
                if(COMPARE(counter, heap[0], i)){
                        heap[0] = i;
                        heapify(heap, counter, 0);
                }
        }

        // this should put values in descending order
        // I think it is just faster to use qsort,
        // but maybe wrong. TODO: Need to check.
        qsort(heap, K, sizeof(int), cmpfunc);
}

#define PARSE_FIRST_DIGIT  \
        time_stamp = *at++ - '0';
#define PARSE_NEXT_DIGIT              \
        time_stamp = time_stamp * 10 + *at++ - '0';

void processfile(char *filename, int *counter) {
    // printf("%s\n", filename);
    FILE* input = fopen(filename,"r");
    setvbuf(stdout, NULL, _IOFBF, 16384);

	if(!input){
	    printf("process file->err:%d\n",errno);
	    exit(errno);
	}
    int buffer_size = 40;
	char buffer[buffer_size+1];

    long time_stamp;
    char *at;
	while(fgets(buffer,buffer_size,input)!=NULL){
        // // loop unrolling
        at = buffer;
        PARSE_FIRST_DIGIT
        PARSE_NEXT_DIGIT
        PARSE_NEXT_DIGIT
        PARSE_NEXT_DIGIT
        PARSE_NEXT_DIGIT
        PARSE_NEXT_DIGIT
        PARSE_NEXT_DIGIT
        PARSE_NEXT_DIGIT
        // PARSE_NEXT_DIGIT
        // PARSE_NEXT_DIGIT
        time_stamp *= 100;

		counter[(unsigned)(time_stamp-start_timestamp)/3600]++;
	}
    fclose(input);
}

void* processfiles(void* arg) {
    ThreadArgs *args = (ThreadArgs*)arg;
    int *localcounter = args->counter;
    int end = args->end;
    char **filenames = args->filenames;
    int buffer_size=40;
    char buffer[buffer_size+1];
    long time_stamp;
    char *at;
    for(int i=args->start; i<end; i++){
        FILE* input = fopen(filenames[i] ,"r");
        // setvbuf(input, NULL, _IOFBF, 16384);

        if(!input){
            printf("process files->err:%d\n",errno);
            exit(errno);
        } 
        while(fgets(buffer,buffer_size,input)!=NULL){
            at = buffer;
            PARSE_FIRST_DIGIT
            PARSE_NEXT_DIGIT
            PARSE_NEXT_DIGIT
            PARSE_NEXT_DIGIT
            PARSE_NEXT_DIGIT
            PARSE_NEXT_DIGIT
            PARSE_NEXT_DIGIT
            PARSE_NEXT_DIGIT
            // PARSE_NEXT_DIGIT
            // PARSE_NEXT_DIGIT
            time_stamp *= 100;

            localcounter[(unsigned)(time_stamp-start_timestamp)/3600]++;
        }
        fclose(input);
    }
    return 0;
}

void startThreads(int file_count, char **filenames) {
    int *localcounters[TNUM];
    ThreadArgs arglist[file_count];
    pthread_t threads[TNUM];
    int blocksize = ceil((double)file_count/TNUM);
    int i=0;
    int start = 0;
    for(; i<TNUM; i++){
        localcounters[i] = (int*)calloc(COUNTER_SIZE, sizeof(int));
        arglist[i].filenames = filenames;
        arglist[i].start = start;
        start += blocksize;
        arglist[i].end = start;
        arglist[i].counter = localcounters[i];
    }
    arglist[TNUM - 1].end = file_count;

    // DO NOT merge 2 loops for creating and wait. It is slower for some reason.    
    for(i=0; i<TNUM; i++){
        pthread_create(&threads[i], NULL, &processfiles, &arglist[i]);
    }
    for(i=0; i<TNUM; i++) {
        pthread_join(threads[i], NULL);
    }
    for(i=0; i<TNUM; i++) {
        for(int idx=0; idx<COUNTER_SIZE; idx++)
            counter[idx] += localcounters[i][idx];
    }
}

int main(int argc, char **argv)
{
    char dirname[40];  // To store directory
    strcpy(dirname, argv[1]);
    start_timestamp = atol(argv[2]);
    K = atoi(argv[3]);
    // printf("%s %ld %d\n", dir, minstamp, K);

    memset(counter, 0, COUNTER_SIZE);

    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);
    // Count the number of files
    int file_count = 0;
    while ((dir = readdir(d)) != NULL) {
        if (dir->d_name[0]=='.')
		    continue;
        file_count++;
    }
    closedir(d);

    //Combine these two loops
    char **filenames=(char**)malloc(file_count*sizeof(char*));
    for(int i=0; i<file_count; i++) {
        filenames[i] = (char*)malloc(60*(sizeof(char*)));
        filenames[i][0] = '\0';
    }
    int i = 0;
    d = opendir(dirname);
    while ((dir = readdir(d)) != NULL)
    {
        if (dir->d_name[0]=='.')
		    continue;
        // printf("%s\n", dir->d_name);
        strcat(filenames[i], dirname);
        strcat(filenames[i], dir->d_name);
        i++;
    }
    closedir(d);

    if (file_count < 16) {
        for(int i=0; i<file_count; i++)
            processfile(filenames[i], counter);
    } else {
        startThreads(file_count, (char**)filenames);
    }

    // Do top K here.
    int topK[K];
    TopK(counter, topK);

    // Do output here.
    char temp[40];
    printf("Top K frequently accessed hour:\n");
    for(int i=0; i<K; i++) {
        // Can do printing faster
        // https://stackoverflow.com/questions/5975378/fastest-way-to-print-a-certain-number-of-characters-to-stdout-in-c
        time_string(start_timestamp+topK[i]*3600, temp);
        printf("%s\t%d\n", temp, counter[topK[i]]);
    }

    return 0;
}
