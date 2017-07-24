#include <stdio.h>

#define BUF_LEN 2024 //81*25

int main(int argc, char ** argv){
	printf("%s %s\n", argv[0], argv[1]);
	FILE * fp = fopen(argv[1] ? argv[1] : "file.default", "rw");
	char buf[BUF_LEN];
	fread(buf, 1, BUF_LEN, fp);
	for (int i = 0; i < BUF_LEN && buf[i] != 0; i++)
		printf("%c", buf[i]);
	fclose(fp);
	return 0;
}
