#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>

#define LINE_BUF_LEN 256 //81*25

typedef struct line {
	struct line * prev;
	char buf[LINE_BUF_LEN];
	struct line * next;
} line_t;
line_t * cur;
line_t * root;
line_t * tail;

int readline(char buf[], FILE * fp)
{
	return fgets(buf, LINE_BUF_LEN, fp) != NULL ? 1 : 0;
}
int loadfile(FILE * fp)
{
	while(tail == (line_t *) NULL)
	{
		cur->next = (line_t *) malloc(sizeof(line_t));
		cur->next->prev = cur;
		if(!readline(cur->next->buf, fp))
		{
			tail = cur->next;
			cur = cur->prev;
			tail->next = 0;
		}
		cur = cur->next;
	}
}
int scroll(char lines, FILE * fp)
{
	if(lines > 0)
	{
		if(cur->next)
		{
			cur = cur->next;
			printf("[%p|%p|%p]%s", cur->prev, cur, cur->next, cur->buf);
			fflush(stdout);
		}
	} else if (lines < 0) {
		if(cur->prev)
		{
			cur = cur->prev;
			printf("[%p|%p|%p]%s", cur->prev, cur, cur->next, cur->buf);
		} else { printf("At top\n");}
		fflush(stdout);
	} else if (lines == 0) {
		printf("Can't scroll 0 lines.Sorry\n");
		fflush(stdout);
	}
}
int main(int argc, char ** argv){
	struct termios terminfo;
	struct termios terminfo_old;
	tcgetattr(0, &terminfo); 		// 0 is stdin
	terminfo_old = terminfo;
	terminfo.c_lflag &= ~ICANON; 	// disable canonical mode, 
									// be sure to reenable this
	terminfo.c_cc[VMIN] = 1; 		// wait for at least one keystroke
	terminfo.c_cc[VTIME] = 0; 		// no timeout
	terminfo.c_lflag &= ~ECHO;		// disable echo
	tcsetattr(0, TCSANOW, &terminfo); // Set immediately
	
	//setbuf(stdout, NULL);			// Disable buffering of stdout

	root = malloc(sizeof(line_t));
	root->prev = 0;
	root->next = (line_t *)&cur;
	
	tail = (line_t *) NULL;

	FILE * fp = fopen(argv[1] ? argv[1] : "file.default", "rw");
	cur = root;
//	readline(cur->buf, fp);
	loadfile(fp);
	cur = root;
	char c;
	while(read(0, &c, 1) != -1)
	{
		if(c == 'j' && cur != tail)
		{
			scroll(1, fp);
		} else if (c == 'k' && cur != root) {
			scroll(-1, fp);
		}
		else if (c == 'q') {
			break;
		}
	}	

	fclose(fp); 
	tcsetattr(0, TCSANOW, &terminfo_old); // restore old settings
	return 0;
}
