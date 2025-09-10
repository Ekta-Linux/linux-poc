/*
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * ******************************************************************************
 * This program is part of the source code provided with
 * "Embedded Linux Drivers" Training program by Raghu Bharadwaj
 * (c) 2020 - 2022 Techveda www.techveda.org
 * ******************************************************************************
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 * *******************************************************************************
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include "platform.h"

//#define DEV_NAME    "/dev/vDev-2"
#define BUFF_SIZE   2048

char tranfer_buff[BUFF_SIZE];
char receive_buff[BUFF_SIZE];
char DEV_NAME[512];
int user_write(void)
{
	int fd, ret;

	fd = open(DEV_NAME, O_WRONLY);
	if (fd < 0) {
		perror("Failed to open the device\n");
		close(fd);
		return errno;
	}

	printf("Type in a short string to write to device:\n");
	scanf(" %[^\n]%*c", tranfer_buff);

	ret = write(fd, tranfer_buff, BUFF_SIZE);
	if (ret < 0) {
		printf("Failed to write the message to the device\n");
		close(fd);
		return errno;
	}

	close(fd);
	return 0;
}

int user_read(int size)
{
	int fd, ret;
	int total_read = 0, tmp = 0;

	fd = open(DEV_NAME, O_RDONLY);
	if (fd < 0) {
		perror("Failed to open the device\n");
		close(fd);
		return errno;
	}

	while (size) {
		tmp = read(fd, &receive_buff[total_read], size);

		if (!tmp) {
			printf("End of file \n");
			close(fd);
			break;
		} else if (tmp <= size) {
			printf("read %d bytes of data \n", tmp);
			/* 'ret' contains count of data bytes successfully read , so add it to 'total_read' */
			total_read += tmp;
			/* We read some data, so decrement 'remaining' */
			size -= tmp;
		} else if (tmp < 0) {
			printf("something went wrong\n");
			close(fd);
			break;
		}
	}

	/* Dump buffer */
	for (int i = 0; i < total_read; i++)
		printf("%c", receive_buff[i]);

	printf("\n");
	close(fd);
	return 0;
}

int main(int argc, char *argv[])
{
	int size;

	if (argc < 3 || strcmp(argv[1], "--help") == 0) {
		printf("Usage: %s <devnode> [read/write] <read size> \n", argv[0]);
		printf("E.g. %s /dev/vDev-1 read 1024\n", argv[0]);
		return 0;
	}

	strcpy(DEV_NAME, argv[1]);
	if (strcmp(argv[2], "write") == 0)
		user_write();

	if (strcmp(argv[2], "read") == 0) {
		size = atoi(argv[3]);
		user_read(size);
	}

	/* Activate this for lseek testing */
#if  0
	ret = lseek(fd, -10, SEEK_SET);
	if (ret < 0) {
		perror("lseek");
		close(fd);
		return ret;
	}
#endif

	return 0;
}
