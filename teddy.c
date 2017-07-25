#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

#define LINE_BUF_LEN 256 //81*25

int readline(char buf[], FILE * fp)
{
	return fgets(buf, LINE_BUF_LEN, fp) != NULL ? 1 : 0;
}
int main(int argc, char ** argv){
	struct termios terminfo;
	tcgetattr(0, &terminfo); 		// 0 is stdin
	terminfo.c_lflag &= ~ICANON; 	// disable canonical mode, 
									// be sure to reenable this
	terminfo.c_cc[VMIN] = 1; 		// wait for at least one keystroke
	terminfo.c_cc[VTIME] = 0; 		// no timeout
	terminfo.c_lflag &= ~ECHO;		// disable echo
	tcsetattr(0, TCSANOW, &terminfo); // Set immediately

	FILE * fp = fopen(argv[1] ? argv[1] : "file.default", "rw");
	char line_buf[LINE_BUF_LEN];
	char c;
	while(readline(line_buf, fp))
	{
		printf("%s", line_buf);
		if(line_buf[strlen(line_buf) - 1] != '\n'); //Line was too long
		while(read(0, &c, 1) != -1)
		{
			if(c == 's')
			{
				break;
			}
		}
	}
	fclose(fp);
	return 0;
}
