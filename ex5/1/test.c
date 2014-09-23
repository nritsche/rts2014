#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "io.h"
int main(int argc, char **argv)
{
if (io_init() < 0){
printf("io_init error\n");
return 0;
}

while (1){
io_write(1,1);
usleep(1000000);
io_write(1,0);
usleep(1000000);
}

return 0;
}
