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

#define COUNTER_SIZE 9323
#define TNUM 4

int K;
int counter[COUNTER_SIZE];
// pthread_mutex_t counter_lock;
char dirname[40];  // To store directory
long start_timestamp; // minimal timestamp supplied as argv

struct thread_args {
    char** filenames;
    int * counter;
    int filecount;
    int start;
    int end;
};

typedef struct thread_args ThreadArgs;

void time_string(time_t t, char* s) {
    // sample testcase
    // input: 1645491600
    // output: Tue Feb 22 09:00:00 2022
    struct tm *tm = localtime(&t);
    strftime(s, 25, "%c", tm);
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

void swap(int *a, int *b) {
        int temp = *a;
        *a = *b;
        *b = temp;
}

void heapify(int* heap, int* counter, int i) {
        // K - global variable defining size of heap
        int largest = i;
        int l = 2*i+1;
        int r = 2*i+2;
        if(l<K && compare_value_and_time(counter, heap[l], heap[largest]) < 0)
                largest = l;
        if(r<K && compare_value_and_time(counter, heap[r], heap[largest]) < 0)
                largest = r;
        if(largest!=i){
                swap(&heap[i], &heap[largest]);
                heapify(heap, counter, largest);
        }
}

void buildHeap(int *heap, int *counter) {
    // Calling Heapify for all non leaf nodes
    // K - global variable denoting size of heap
    for (int i = K / 2 - 1; i >= 0; i--) {
        heapify(heap, counter, i);
    }
}

int cmpfunc(const void * a, const void * b) {
    // requires global counter value to be set
   return  -compare_value_and_time(counter, *(int*)a, *(int*)b);
}

void TopK(int *counter, int *heap)
{
        // K - global variable denoting size of heap
        for(int i=0; i<K; i++) {
                heap[i] = i;
        }
        buildHeap(heap, counter);
        for(int i=K; i<COUNTER_SIZE; i++) {
                if(compare_value_and_time(counter, heap[0], i)<0){
                        heap[0] = i;
                        heapify(heap, counter, 0);
                }
        }

        // this should put values in descending order
        // I think it is just faster to use qsort,
        // but maybe wrong. TODO: Need to check.
        qsort(heap, K, sizeof(int), cmpfunc);
}

void processfile(char *filename, int *counter) {
    // printf("%s\n", filename);
    FILE* input = fopen(filename,"r");

	if(!input){
	    printf("process file->err:%d\n",errno);
	    exit(errno);
	} 
    int buffer_size=40;
	char buffer[buffer_size+1];

	int line=0;
	while(fgets(buffer,buffer_size,input)!=NULL){
		char* temp;
		long time_stamp = strtol(buffer,&temp,10);
		counter[(time_stamp-start_timestamp)/3600]++;
	}

    // pthread_mutex_lock(&counter_lock);
    // for(int i=0; i!=COUNTER_SIZE; ++i) {
    //     global_counter[i] += localcounter[i];
    // }
    // pthread_mutex_unlock(&counter_lock);
    // free(localcounter);
    fclose(input);
}

void* processfiles(void* arg) {
    ThreadArgs *args = (ThreadArgs*)arg;
    int *localcounter = args->counter;
    int start = args->start;
    int end = args->end;
    char **filenames = args->filenames;
    for(int i=start; i<end; i++){
        // printf("%s\n", filenames[i]);
        FILE* input = fopen(filenames[i] ,"r");
        if(!input){
            printf("process files->err:%d\n",errno);
            exit(errno);
        } 
        int buffer_size=40;
        char buffer[buffer_size+1];
        int line=0;
        while(fgets(buffer,buffer_size,input)!=NULL){
            char* temp;
            long time_stamp = strtol(buffer,&temp,10);
            localcounter[(time_stamp-start_timestamp)/3600]++;
        }
        fclose(input);
    }

    // pthread_mutex_lock(&counter_lock);
    // for(int idx=0; idx<COUNTER_SIZE; idx++)
    //     counter[idx] += localcounter[idx];
    // pthread_mutex_unlock(&counter_lock);

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
        // printf("%d here\n", i);
        localcounters[i] = (int*)malloc(9400*sizeof(int));
        memset(localcounters[i], 0, COUNTER_SIZE*sizeof(int));
        arglist[i].filenames = filenames;
        arglist[i].start = start;
        arglist[i].end = start + blocksize;
        arglist[i].counter = localcounters[i];
        arglist[i].filecount = file_count;
        start += blocksize;
    }
    arglist[TNUM - 1].end = file_count;

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
    // Sleep function to artificially slown down program
    // struct timespec ts;
    // ts.tv_sec = 150 / 1000;
    // ts.tv_nsec = (150 % 1000) * 1000000;
    // nanosleep(&ts, NULL);

    strcpy(dirname, argv[1]);
    start_timestamp = atol(argv[2]);
    K = atoi(argv[3]);
    // printf("%s %ld %d\n", dir, minstamp, K);

    // 65536 is preffered block size

    memset(counter, 0, COUNTER_SIZE);

    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);
    // Count the number of files
    int file_count = 0;
    while ((dir = readdir(d)) != NULL) {
        if (!strcmp (dir->d_name, "."))
		    continue;
	    if (!strcmp (dir->d_name, ".."))
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
        time_string((time_t)start_timestamp+topK[i]*3600, temp);
        printf("%s\t%d\n", temp, counter[topK[i]]);
    }

    return 0;
}
