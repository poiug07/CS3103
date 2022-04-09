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


int main(int argc, char **argv)
{
    strcpy(dirname, argv[1]);
    start_timestamp = atol(argv[2]);
    K = atoi(argv[3]);
    // printf("%s %ld %d\n", dir, minstamp, K);

    // 65536 is preffered block size

    // memset(counter, 0, COUNTER_SIZE);

    DIR *d;
    struct dirent *dir;
    char temp[60];
    temp[0] = '\0';
    d = opendir(dirname);
    if (d)
    {
        char arr[]

        int fcount = 0;

        while ((dir = readdir(d)) != NULL) {
            if (dir->d_type == DT_REG) {
                fcount++;
            }
        }

        rewinddir(d);


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


            // processfile(temp, counter);
            printf("%s\n", temp);

            temp[0] = '\0';
        }


        closedir(d);
    }
}

// ./test.o case5/ 1645491600 5