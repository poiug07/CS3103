#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

#define COUNTER_SIZE 9400

int K;
int counter[COUNTER_SIZE];
char dirname[40];  // To store directory
long start_timestamp; // minimal timestamp supplied as argv

void processfile(char *filename, int *global_counter) {
    // malloc is slow, should init once and use it many times. Size is fixed.
    int *localcounter = (int*)malloc(9400*sizeof(int));
    memset(localcounter,0 ,COUNTER_SIZE*sizeof(int));
    // printf("%s\n", filename);
    FILE* input = fopen(filename,"r");

	if(!input){
	    printf("process file->err:%d\n",errno);
	    exit(errno);
	} 
    int buffer_size=40;
	char buffer[buffer_size+1];

	int i=0;
	int line=0;
	while(fgets(buffer,buffer_size,input)!=NULL){
		char* temp;
		long time_stamp = strtol(buffer,&temp,10);
		localcounter[(time_stamp-start_timestamp)/3600]++;
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
    

    // Assume here I have array of indices with top values.
    int topK[5] = {1, 2, 3, 4, 5};
    // Do output here.
    for(int i=0; i<K; i++) {
        // Can do printing faster
        // https://stackoverflow.com/questions/5975378/fastest-way-to-print-a-certain-number-of-characters-to-stdout-in-c
        time_string((time_t)start_timestamp+topK[i]*3600, temp);
        printf("%s\t\t%d\n", temp, counter[topK[i]]);
    }

    return 0;
}
