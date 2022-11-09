#include <stdio.h>
#include <stdlib.h>
#include <linux/fb.h>
#include <fcntl.h>
#include <error.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int fd = 0, fd1 = 0;
	int index = 44;
	int ret =0;
	char sgpio[64];

	if(argc < 3){
		printf("argc have %d, must >=  3\n", argc);
		return -1;
	}
	
	fd1 = open("/sys/class/gpio/gpiochip0/ngpio", O_RDONLY);
	if(fd1 < 0){
		printf("open fd1 err\n");
		return -1;
	} else 
		printf("open file: %d ok\n", fd1);

	unsigned char rch2[32];
	printf("len strlen: %d\n",  sizeof(rch2));
	int read_len2;
	read_len2 = read(fd1, rch2, 8);
	printf("recver buf %d: %d\n", read_len2, atoi(rch2));
	

	sprintf(sgpio, "/sys/class/gpio/gpio%d/direction", atoi(argv[1]));
	printf("sgpio is: %s\n", sgpio);
	fd = open(sgpio, O_RDWR);
	if(fd < 0){
		printf("open gpio 44 err\n");
		return -1;
	} else 
		printf("open file: %d ok\n", fd);

	//unsigned char *sch = "out";
	unsigned char sch[32] = "in";
	write(fd, sch, strlen(sch));	
	lseek(fd, 0, SEEK_SET);		//must add this for next read

	unsigned char rch[32];
	printf("len strlen: %d, %d\n",  strlen(rch), sizeof(rch));
	int read_len;
	read_len = read(fd, rch, 8);
	printf("recver buf %d: %s\n", read_len, rch);
	lseek(fd, 0, SEEK_SET);
	

	unsigned char sch1[32] = "high";
	write(fd, sch1, strlen(sch1));	
	lseek(fd, 0, SEEK_SET);

	unsigned char rch1[32];
	int read_len1;

	read_len1 = read(fd, rch1, sizeof(rch1));
	printf("recver buf1 %d : %s\n", read_len1, rch1);
	

	close(fd);

	return 0;

}
