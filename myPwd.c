#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


ino_t get_inode(const char *filename);
void print_path(ino_t inode);
void inode_to_name(ino_t inode, char *name);

int main()
{
	print_path(get_inode("."));
	putchar('\n');
	return 0;
}


ino_t get_inode(const char *filename)
{
	struct stat info;
	if(stat(filename, &info) == -1)
	{
		fprintf(stderr, "open stat error");
		exit(EXIT_FAILURE);
	}
	return info.st_ino;
}


void inode_to_name(ino_t inode, char *name)
{
	DIR *dir_ptr;
	if((dir_ptr = opendir(".")) == NULL)
	{
		perror(".");
		exit(EXIT_FAILURE);
	}

	struct dirent* dirent_ptr;
	while((dirent_ptr = readdir(dir_ptr)) != NULL)
	{
		if(dirent_ptr->d_ino == inode)
		{
			strcpy(name, dirent_ptr->d_name);
			closedir(dir_ptr);
			return;
		}
	}
	closedir(dir_ptr);
	fprintf(stderr, "can't find the corresponding inode");
	exit(EXIT_FAILURE);
}

void print_path(ino_t inode)
{
	if(get_inode("..") != inode)
	{
		chdir("..");
		char name[50];
	    inode_to_name(inode, name);
		print_path(get_inode("."));
		printf("/%s", name);
	}
}



















