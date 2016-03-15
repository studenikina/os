#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#define BUFFER_SIZE 256

void m_write(char* buffer, int fd) 
{
	ssize_t rres;
	while ((rres = read(fd, buffer, BUFFER_SIZE)) > 0)
	{ 
		ssize_t wres = rres, res;
		while (wres > 0 && (res = write(1, buffer + (rres - wres), wres))) 			{
			if (res < 0) 
			{
				perror("Error");
				continue;
			}
			wres = rres -res;
		}
		
	}
}

int main (int argc, char *argv[])
{
	char buffer[BUFFER_SIZE];
	if (argc > 1) 
    	{
		size_t i;
		for (i = 1; i < argc; ++i) 
		{
			int fd = open(argv[i], O_RDONLY);
			if (fd == -1) 
			{
				continue;
			}			
			m_write(buffer, fd);
	   	 	close(fd);
      		 }
    	} else if (argc == 1) m_write(buffer, 0);
	return 0;
}


