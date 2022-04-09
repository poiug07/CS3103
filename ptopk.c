#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

#define COUNTER_SIZE 9400
#define BLK_SIZE 4096

int K;
int counter[COUNTER_SIZE];
char dirname[40];  // To store directory
long start_timestamp; // minimal timestamp supplied as argv

long get_file_length(const char* file_name){
	struct stat sb;
	if (stat(file_name,&sb)==-1){exit(-1);}
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

void parse_block(int *localcounter, char* buffer, char* tail,int block_size, char* value_string){
	int parsed_posi = 0;
	int val_po=0;
	long time_stamp;
	// parse the first entry
	while(tail[val_po]!='\0'){
		value_string[val_po]=tail[val_po];
		val_po++;
	}
	while(buffer[parsed_posi]!=','){
		value_string[val_po]=buffer[parsed_posi];
		parsed_posi++;
		val_po++;
	}

    char *temp;
    value_string[val_po] = '\0';
    time_stamp = strtol(value_string+val_po-10,&temp,10);
	localcounter[(time_stamp-start_timestamp)/3600]++;
	val_po=0;
	parsed_posi++;

	char current;
	while(parsed_posi<block_size){
		current = buffer[parsed_posi];
		if (current==','){
		    // former part is the target_value
            buffer[parsed_posi]='\0';
            // time_stamp = atol(buffer+parsed_posi-10);
            time_stamp = strtol(buffer+parsed_posi-10, &temp,10);
            localcounter[(time_stamp-start_timestamp)/3600]++;
            value_string[0] = '\0';
		}
		tail[val_po]=current;
		if(current=='\n'){
		    // when it swtich the line
			val_po=-1;// reset it again
		}
		// move the index
		val_po++;
		parsed_posi++;
	}
	tail[val_po]='\0';
}

void processfile(char *filename, int *global_counter) {
    // malloc is slow, should init once and use it many times. Size is fixed anyways.
    long file_len = get_file_length(filename);

    int *localcounter = (int*)malloc(COUNTER_SIZE*sizeof(int));
    memset(localcounter,0, COUNTER_SIZE*sizeof(int));
    FILE* input = fopen(filename,"r");

	if(!input){
	    printf("process file->err:%d\n",errno);
	    exit(errno);
	}

    // fseek(input,0,SEEK_SET);
    char buffer[BLK_SIZE];

    char tail[40];
    tail[0] = '\0';
    char value_string[40];
    value_string[0] = '\0';

    int current_read=0;
    long readed = 0;

    while(readed<file_len){
		current_read = fread(buffer, 1, BLK_SIZE, input);
		readed+=current_read;
		parse_block(localcounter, buffer, tail,current_read, value_string);
	}

    for(int i=0; i!=COUNTER_SIZE; ++i) {
        global_counter[i] += localcounter[i];
    }

    free(localcounter);
    fclose(input);
}


void time_string(time_t t, char* s) {
    // sample testcase
    // input: 1645491600
    // output: Tue Feb 22 09:00:00 2022
    struct tm *tm = localtime(&t);
    strftime(s, 25, "%c", tm);
}

int main(int argc, char **argv)
{
    strcpy(dirname, argv[1]);
    start_timestamp = atol(argv[2]);
    K = atoi(argv[3]);
    // printf("%s %ld %d\n", dir, minstamp, K);

    // 65536 is preffered block size

    memset(counter, 0, COUNTER_SIZE);

    DIR *d;
    struct dirent *dir;
    char temp[60];
    temp[0] = '\0';
    d = opendir(dirname);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (!strcmp (dir->d_name, "."))
		        continue;
		    if (!strcmp (dir->d_name, ".."))
		        continue;
            // printf("%s\n", dir->d_name);
            // can start operation here
            strcat(temp, dirname);
            strcat(temp, dir->d_name);
            processfile(temp, counter);
            temp[0] = '\0';
        }
        closedir(d);
    }

    // Do top K here.
    int topK[K];
    TopK(counter, topK);

    // // Assume here I have array of indices with top values.
    // int topK[5] = {1, 2, 3, 4, 5};

    // Do output here.
    printf("Top K frequently accessed hour:\n");
    for(int i=0; i<K; i++) {
        // Can do printing faster
        // https://stackoverflow.com/questions/5975378/fastest-way-to-print-a-certain-number-of-characters-to-stdout-in-c
        time_string((time_t)start_timestamp+topK[i]*3600, temp);
        printf("%s\t%d\n", temp, counter[topK[i]]);
    }

    return 0;
}
