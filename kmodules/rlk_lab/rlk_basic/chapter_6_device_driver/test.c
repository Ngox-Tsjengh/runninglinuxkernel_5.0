#include <stdio.h>
#include <stdlib.h>	//malloc
#include <string.h>	//memset
#include <unistd.h>
#include <fcntl.h>
#include <fcntl.h>
#define DEMO_DEV_NAME "/dev/my_demo_dev"

int main() {

	char message[90] = "Testing the virtual FIFO device with length of 80 and more and more and how dare you"; //and more character and more and more than 64";
	size_t len = sizeof(message);

	char* read_buffer = (char*)malloc(2*len);
	memset(read_buffer, 0, 2*len);

	int fd = open(DEMO_DEV_NAME, O_RDWR | O_NONBLOCK);
	if (fd < 0) {
		printf("open devicei %s failed\n", DEMO_DEV_NAME);
		return -1;
	}

	int ret = read(fd, read_buffer, 2*len);
	printf("read %d bytes, buffer=%s\n", ret, read_buffer);

	ret = write(fd, message, len);
	if (ret != len) {
		printf("have write %d bytes\n", ret);
	}

	ret = write(fd, message, len);
	if (ret != len) {
		printf("have write %d bytes\n", ret);
	}

	//close(fd);
	
	//fd = open(DEMO_DEV_NAME, O_RDWR);
	//if (fd < 0) {
		//printf("open devicei %s failed\n", DEMO_DEV_NAME);
		//return -1;
	//}

	ret = read(fd, read_buffer, 2*len);
	printf("read %d bytes, buffer=%s\n", ret, read_buffer);

	close(fd);
	return 0;
}
