#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

#define BUFFER_SIZE 256

int m_write(int fd) 
{
	char buffer[BUFFER_SIZE];
	ssize_t res;
	while ((res = read(fd, buffer, BUFFER_SIZE)
	{
		ssize_t res_w = write(1, buffer, res);
		if (res_w < res) 
		    return -1;
		
	}
	return 0;
}

int main (int argc, char *argv[])
{
	if (argc > 1) 
    	{
		for (int i = 1; i < argc; ++i) 
		{
			int fd = open(argv[i], O_RDONLY);
			if (m_write(fd) == -1) 
				perror("Err");
	   	 	close(fd);
      		 }
    	}
}
