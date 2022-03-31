#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

int K;

int main(int argc, char **argv)
{
    char dirname[50];  // To store directory
    long start_timestamp; // minimal timestamp supplied as argv

    strcpy(dirname, argv[1]);
    start_timestamp = atol(argv[2]);
    K = atoi(argv[3]);
    // printf("%s %ld %d\n", dir, minstamp, K);

    // 65536 is preffered block size


    DIR *d;
    struct dirent *dir;
    d = opendir(dirname);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            printf("%s\n", dir->d_name);
            // can start operation here
        }
        closedir(d);
    }
    return 0;
}
