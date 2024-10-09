#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

int main(int argc, char** argv) {
	if (argc < 2) {
		exit(errno);
	}

	int fd = open(argv[1], O_WRONLY);
	if (fd < 0) {
		exit(errno);
	}
	
	int fd2 = dup(fd);
	if (fd2 < 0) {
		close(fd);
		exit(errno);
	}
	
	const char* line1 = "fisrt line\n";
	const char* line2 = "secod line\n";

        int lf1 = write(fd, line1, strlen(line1));
	if (lf1 < 0) {
		close(fd);
		close(fd2);
		exit(errno);		
	}

	int lf2 = write(fd2, line2, strlen(line2));
	if (lf2 < 0) {
                close(fd);
                close(fd2);
                exit(errno);
        }
	
	close(fd);
	close(fd2);
	return 0;

}
