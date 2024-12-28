#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

void replace_strs_in_file(int fd, char * buffer, const char * search, const char * replace) {
	char *pos, *start = buffer;
	size_t search_size = strlen(search);
	off_t seek;
	while( (pos = strstr(start, search)) > 0) {
		seek = pos - buffer;
		if (lseek(fd, seek, SEEK_SET) == -1) {
			perror("Seek failed: ");
			return;
		}
		if ( (write(fd, replace, search_size)) == -1  ) {
			perror("Writing to file failed: ");
			return;
		}
		start = pos + search_size;
	}
}

int main() {
	const char * replace = "Mohan";
	const char * search = "Rohan";
	int fd, bytes_read;
	char buffer[1024];
	fd = open("author.txt", O_RDWR);
	if (fd == -1) {
		perror("Open failed: ");
		return 1;
	}

	while ((bytes_read = read(fd, buffer, sizeof(buffer) - 1)) > 0) {
		buffer[bytes_read] = '\0';
		replace_strs_in_file(fd, buffer, search, replace);
		if (lseek(fd, strlen(buffer), SEEK_SET) == -1) {
			perror("Seek failed: ");
			return 1;
		}
	}
	
	close(fd);

	return 0;
}
