#include <stdio.h>
#include <utmp.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SHOWHOST
#define BUFFER_SIZE 16
#define UTMP_SIZE (sizeof(struct utmp))

static struct utmp utmp_buffer[BUFFER_SIZE];
static int current_records = 0;
static int utmp_fd = -1;

void utmp_open(const char *filename);
void show_info(struct utmp *utmp_ptr);
int reLoad();
void utmp_close();
struct utmp *utmp_next();


int main()
{
	utmp_open(UTMP_FILE);
	struct utmp *rec_ptr;
	while((rec_ptr = utmp_next()) != NULL)
		show_info(rec_ptr);

	utmp_close();
	return 0;
}

void utmp_open(const char* filename)
{
	utmp_fd = open(filename, O_RDONLY);
	if(utmp_fd == -1){
		fprintf(stderr, "Can't open %s\n", filename);
		exit(EXIT_FAILURE);
	}
}

int reLoad()
{
	int bytes_read = read(utmp_fd, utmp_buffer, BUFFER_SIZE * UTMP_SIZE);
	if(bytes_read == -1){
		perror("READ ERROR ");
		utmp_close();
		exit(EXIT_FAILURE);
	}
	current_records = bytes_read / UTMP_SIZE;
	return current_records;
}

struct utmp *utmp_next()
{
	struct utmp *rec_ptr;
	if(current_records == 0 && reLoad() == 0){
		return NULL;
	}
	
	rec_ptr = &utmp_buffer[current_records - 1];
	--current_records;
	return rec_ptr;
}

void show_info(struct utmp *utmp_ptr)
{
	if(utmp_ptr->ut_type != USER_PROCESS)
		return;

	printf("%s ", utmp_ptr->ut_user);
	printf("%s ", utmp_ptr->ut_line);
	time_t t = utmp_ptr->ut_tv.tv_sec;
	printf("%s", ctime(&t));

	#ifdef SHOWHOST
		printf("%s\n", utmp_ptr->ut_host);
	#endif
}

void utmp_close()
{
	if(utmp_fd != -1)
		close(utmp_fd);
}
