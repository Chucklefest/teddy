#include <stdio.h>
#include <string.h>

#define LINE_BUF_LEN 256 //81*25

int readline(char buf[], FILE * fp)
{
	return fgets(buf, sizeof buf, fp) != NULL ? 0 : 1;
}
int main(int argc, char ** argv){
	FILE * fp = fopen(argv[1] ? argv[1] : "file.default", "rw");
	char line_buf[LINE_BUF_LEN];
	while(!readline(line_buf, fp))
	{
		printf("%s", line_buf);
		if(line_buf[strlen(line_buf)] != '\n')
		{
			printf("LINE TOO LONG at %zu c:%x ", strlen(line_buf) - 1, line_buf[strlen(line_buf) - 1]);
		}
	}
	fclose(fp);
	return 0;
}
