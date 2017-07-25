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

int readline(char buf[], FILE * fp)
{
	return fgets(buf, LINE_BUF_LEN, fp) != NULL ? 1 : 0;
}
int scroll(char down, FILE * fp)
{
	if(down)
	{ 
		cur->next = (line_t *) malloc(sizeof(line_t));
		cur->next->prev = cur;
		printf("[%p|%p|%p]%s", cur->prev, cur, cur->next, cur->buf);
		cur = cur->next;
		if(!readline(cur->buf, fp))
		{
			cur = (line_t *) NULL;
			printf("EOF reached, line_t: %luB", sizeof (line_t));
		}
		fflush(stdout); 
	} else {
		printf("Only down scrolling supported for now");
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

	line_t * root = malloc(sizeof(line_t));
	root->prev = 0;
	root->next = (line_t *)&cur;
	
	FILE * fp = fopen(argv[1] ? argv[1] : "file.default", "rw");
	cur = root;
	readline(cur->buf, fp);
	char c;
	while(read(0, &c, 1) != -1)
	{
		if(c == 'j' && cur != (line_t *) NULL)
		{
			scroll(1, fp);
		} else if (c == 'q') {
			break;
		}
	}	

	fclose(fp); 
	tcsetattr(0, TCSANOW, &terminfo_old); // restore old settings
	return 0;
}
