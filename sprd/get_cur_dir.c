#include <stdio.h>
#include <unistd.h>

void main()
{
	char cur_dir[1024] = {0};
	getcwd(cur_dir, 1024);
	printf("cur dir is %s\n", cur_dir);
}