#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#define COUNTER_SIZE 9400

int K;
int counter[COUNTER_SIZE];
char dirname[50];  // To store directory
long start_timestamp; // minimal timestamp supplied as argv

void processfile(char *filename, int *global_counter) {
    int *localcounter = (int*)malloc(9400*sizeof(int));
    memset(localcounter,0 ,COUNTER_SIZE*sizeof(int));
    // printf("%s\n", filename);
    FILE* input = fopen(filename,"r");

	if(!input){
	    printf("err:%d\n",errno);
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
    char temp[40];
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
        }
        closedir(d);
    }

    printf("%d %d %d\n", counter[0], counter[1000], counter[2012]);
    // Do top K here.
    

    // Do output here.

    return 0;
}
