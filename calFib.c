#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>  


int main() {
    	int number, result;
    	scanf("%d", &number);
	int size = 8;
	char buf[size];
        snprintf(buf, size, "%d", number);
    	system("sudo rm /dev/OS_phase0_driver");
    	system("sudo mknod -m 666 /dev/OS_phase0_driver c 241 0");
	int fd;
	fd = open("/dev/OS_phase0_driver", O_RDWR);
	if ( 0>write(fd, buf, 8))
		printf("problem in write\n");
	close(fd);
	printf("Result is: \n");
    	system("sudo cat /dev/OS_phase0_driver");
     	return 0;
}
