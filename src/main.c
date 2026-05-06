#ifndef _WIN32
    #error Compilation failed: Currently this game only support Windows.
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    srand(time(NULL));

    return 0;
}