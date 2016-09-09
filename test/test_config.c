#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <ctype.h>

#include "config.h"

#define KEYVALLEN 100


int main()
{
    char host_name[256];

    GetProfileString("./conf/dstorage.conf", "web_server", "host_name", host_name);
    printf("%s\n",host_name);

    GetProfileString("./conf/dstorage.conf", "storage_web_server", "host_name", host_name);
    printf("%s\n",host_name);

    return 0;
}
