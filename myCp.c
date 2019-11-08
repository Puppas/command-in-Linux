#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

#define BUFFER_SIZE 4096
#define COPY_MODE 0644

int main(int argc, char *argv[])
{
	if(argc != 3){
		fprintf(stderr, "The arguments can't match");
		exit(1);
	}

	int source_fd;
	int target_fd;
	char buffer[BUFFER_SIZE];

	if((source_fd = open(argv[1], O_RDONLY)) == -1){
		fprintf(stderr, "Can't open %s\n", argv[1]);
		exit(1);
	}

	if((target_fd = creat(argv[2], COPY_MODE)) == -1){
		fprintf(stderr, "Can't create %s\n", argv[2]);
		exit(1);
	}

	int read_chars;
	while((read_chars = read(source_fd, buffer, BUFFER_SIZE)) > 0)
	{
		if(write(target_fd, buffer, read_chars) != read_chars)
		{
			perror(argv[2]);
			close(source_fd);
			close(target_fd);
			exit(1);
		}
	}

	if(read_chars == -1){
		perror(argv[1]);
		close(source_fd);
		close(target_fd);
		exit(1);
	}

	close(source_fd);
	close(target_fd);
	return 0;
}
