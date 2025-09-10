#include <stdio.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include "platform.h"

//#define DEV_NAME    "/dev/vDev-0"

char DEV_NAME[512];
int main(int argc, char *argv[]) {
	
	if(argc < 2 || strcmp(argv[1], "--help") == 0) {
		printf("Usage: %s <devnode>", argv[0]);
		printf("E.g. %s /dev/vDev-1\n",argv[0]);
		return 0;
	}

	strcpy(DEV_NAME, argv[1]);

	int fd = open(DEV_NAME, O_RDWR);
	if (fd < 0) {
		perror("open");
		return -1;
	}

	//ioctl(fd, VDEV_FILLCHAR, 'A');
	ioctl(fd, VDEV_FILLZERO);

	close(fd);

	return 0; 
}
