/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  11/12/2014 08:40:05 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Kevin (###), kevin101@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>

#define BUF_SIZE	33
#define VD_DEV		"/dev/virtualdisk"

int main(int argc, char *argv[])
{
	int fd;
	char rbuf[BUF_SIZE];
	char *wbuf = "test for write";

	fd = open(VD_DEV, O_RDWR, S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH);
	if (fd == -1)
	{
		printf("open failed!\n");
		exit(EXIT_FAILURE);
	}
	
	if (argc > 1)
		wbuf = argv[1];

	if (lseek(fd, 0, SEEK_SET) == -1)
	{
		printf("lseek failed!\n");
		exit(EXIT_FAILURE);
	}

	printf("%s\n", wbuf);
	if (write(fd, wbuf, strlen(wbuf)) == -1)
	{
		printf("write failed!\n");
		exit(EXIT_FAILURE);
	}

	if (lseek(fd, 0, SEEK_SET) == -1)
	{
		printf("lseek failed!\n");
		exit(EXIT_FAILURE);
	}

	bzero(&rbuf, strlen(rbuf));
	if (read(fd, rbuf, sizeof(rbuf)) == -1)
	{
		printf("read failed!\n");
		exit(EXIT_FAILURE);
	}

	printf("Read from '%s' : |- %s -|\n", VD_DEV, rbuf);

	if (close(fd) == -1)
	{
		printf("close failed!\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}
