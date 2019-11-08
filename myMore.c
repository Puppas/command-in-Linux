#include <stdio.h>
#include <stdlib.h>

#define PAGE_LEN 36
#define LINE_LEN 512

void do_more(FILE *fp);
int see_more(FILE *fp_tty);

int main(int argc, char *argv[])
{
	if(argc == 1)
		do_more(stdin);
	else
	{
		FILE *fp;
		while((fp = fopen(*++argv, "r")) != NULL && argc-- > 1)
		{
			do_more(fp);
			fclose(fp);
		}
	}
	return 0;
}

void do_more(FILE *fp)
{
	FILE *fp_tty = fopen("/dev/tty", "r");
	if(fp_tty == NULL){
		printf("Can't find the file(/dev/tty)");
		exit(EXIT_FAILURE);
	}

	char line[LINE_LEN + 1];
	int numOfLine = 0;

	while(fgets(line, LINE_LEN + 1, fp) != NULL)
	{
		fputs(line, stdout);
		++numOfLine;

		int reply;
		if(numOfLine >= 24){
			if((reply = see_more(fp_tty))){
				numOfLine -= reply;
			}
			else break;
		}
	}

	fclose(fp_tty);
}

int see_more(FILE *fp_tty)
{
	printf("\033[0;34m more? \033[m");
	int ch;
	while((ch = getc(fp_tty)) != EOF)
	{
		getc(fp_tty);
		if(ch == ' ')
			return PAGE_LEN;
		if(ch == '\n')
			return 1;
		if(ch == 'q')
			return 0;
	}
}
























