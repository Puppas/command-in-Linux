#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


#define true 1
#define false 0


char *getUserName(__uid_t uid);
char *getGroupName(__gid_t gid);
int getNumOfLinks(struct stat* stat_ptr);
char* getFileName(struct dirent *dirent_ptr);
void  setMode(struct stat *stat_ptr, char mode[]);
void do_ls(const char dirname[]);



int main(int argc, char *argv[])
{
	if(argc == 1)
		do_ls("./");

	while(argc > 1)
	{	
		do_ls(argv[argc - 1]);
		--argc;
	}

	return 0;
}


void do_ls(const char dirname[])
{
	DIR *dir_ptr;
	char mode[11] = "----------\0";
	char *filename;
	int num_of_links;
	int size;
	time_t time;
	char *user_name;
	char *group_name;
	

	if((dir_ptr = opendir(dirname)) == NULL)
	{
		fprintf(stderr, "Can't open the directory: %s\n", dirname);
		return;
	}
	
	while(true)
	{
		struct dirent* dirent_ptr;

		if((dirent_ptr = readdir(dir_ptr)) == NULL){
			break;
		}

		filename = getFileName(dirent_ptr);
		if(filename[0] == '.'){
			continue;
		}

		struct stat* stat_ptr = (struct stat*) malloc(sizeof(struct stat));
		if(stat(filename, stat_ptr) == -1){
			fprintf(stderr, "open stat error\n");
			free(stat_ptr);
			continue;
		}
		setMode(stat_ptr, mode);
		num_of_links = getNumOfLinks(stat_ptr);
		size = stat_ptr->st_size;
		time = stat_ptr->st_mtim.tv_sec;

		user_name = getUserName(stat_ptr->st_uid);
		group_name = getGroupName(stat_ptr->st_gid);

		printf("%s\t%d\t%s\t%s\t%d\t %.24s\t%s\n",
				mode, num_of_links, user_name, group_name, size, ctime(&time), filename);

		free(stat_ptr);
	}

	closedir(dir_ptr);
}


char *getUserName(__uid_t uid)
{
	static char uid_str[10];
	struct passwd *pw_ptr;

	if((pw_ptr = getpwuid(uid)) == NULL){
		sprintf(uid_str, "%d", uid);
		return uid_str;
	}
	else return pw_ptr->pw_name;
}

char *getGroupName(__gid_t gid)
{
	static char gid_str[10];
	struct group *grp_str;

	if((grp_str = getgrgid(gid)) == NULL){
		sprintf(gid_str, "%d", gid);
		return gid_str;
	}
	else return grp_str->gr_name;
}

void setMode(struct stat *stat_ptr, char mode[])
{	

	if((stat_ptr->st_mode & S_IFMT) == S_IFREG)
		mode[0] = '-';
	else if((stat_ptr->st_mode & S_IFMT) == S_IFDIR)
		mode[0] = 'd';
	else if((stat_ptr->st_mode & S_IFMT) == S_IFCHR)
		mode[0] = 'c';
	else if((stat_ptr->st_mode & S_IFMT) == S_IFBLK)
		mode[0] = 'b';

	if(stat_ptr->st_mode & S_IRUSR)
		mode[1] = 'r';
	else mode[1] = '-';

	if(stat_ptr->st_mode & S_IWUSR)
		mode[2] = 'w';
	else mode[2] = '-';

	if(stat_ptr->st_mode & S_IXUSR)
		mode[3] = 'x';
	else mode[3] = '-';

	if(stat_ptr->st_mode & S_IRGRP)
		mode[4] = 'r';
	else mode[4] = '-';

	if(stat_ptr->st_mode & S_IWGRP)
		mode[5] = 'w';
	else mode[5] = '-';

	if(stat_ptr->st_mode & S_IXGRP)
		mode[6] = 'x';
	else mode[6] = '-';

	if(stat_ptr->st_mode & S_IROTH)
		mode[7] = 'r';
	else mode[7] = '-';

	if(stat_ptr->st_mode & S_IWOTH)
		mode[8] = 'w';
	else mode[8] = '-';

	if(stat_ptr->st_mode & S_IXOTH)
		mode[9] = 'x';
	else mode[9] = '-';
}

char *getFileName(struct dirent *dirent_ptr)
{	
	char *filename;
	filename = dirent_ptr->d_name;
	
	return filename;
}

int getNumOfLinks(struct stat* stat_ptr)
{
	return stat_ptr->st_nlink;
}









