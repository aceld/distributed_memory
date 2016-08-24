#include <stdio.h>
#include <stdlib.h>
#include "time.h"
#include <sys/time.h>
int main()
{
    struct timeval tv;
    struct tm* ptm;
    char time_str[128];

    gettimeofday(&tv, NULL);

    ptm = localtime(&tv.tv_sec);

    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", ptm);

    printf("%s\n", time_str);

    return 0;
}
