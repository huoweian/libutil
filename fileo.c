#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <dirent.h>
#include "fileo.h"

#define BUFFER_SIZE 1024
#define DIR_MAXPATH 250

int copy_file(const char *from_file, const char *to_file)
{
	int iRet = -1, size = 0, read_size, write_size, from_fd, to_fd;
	byte buffer[BUFFER_SIZE], *p = NULL;

	if (!from_file || !to_file) {
		errno = EBADF;
		goto ERR;
	}
	if ((from_fd = open(from_file, O_RDONLY)) == -1) {
		goto ERR;
	}
	if ((to_fd = open(to_file, O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR)) == -1) {
		goto ERR_FROM;
	}
	while ((read_size = read(from_fd, buffer, BUFFER_SIZE))) {
		if (read_size == -1 && errno != EINTR) {
			goto ERR_TO;
		} else if (read_size > 0) {
            p = buffer;
            while ((write_size = write(to_fd, p, read_size))) {
				size += write_size;
				if (write_size == -1 && errno != EINTR) {
					goto ERR_TO;
				} else if (write_size == read_size) {
					break;
				} else if (write_size < read_size) {
				   p += write_size;
				   read_size -= write_size;
				} else if (write_size > read_size) {
				   errno = EINVAL;
				   goto ERR_TO;
				}
			}
        }
    }
	iRet = size;
ERR_TO:
	close(to_fd);
ERR_FROM:
	close(from_fd);
ERR:
	return iRet;
}

int copy_dir(const char *from_path, const char *to_path)
{
	int iRet = -1, size = 0;
	DIR *from_dir = NULL, *to_dir = NULL;
	struct dirent *ptr = NULL;
	char *from_sub_path = NULL, *to_sub_path = NULL;

	if (!from_path || !to_path) {
		errno = EBADF;
		return iRet;
	}
	if ((from_sub_path = (char *)malloc(DIR_MAXPATH)) == NULL) {
		errno = ENOMEM;
		return iRet;
	}
	if ((to_sub_path = (char *)malloc(DIR_MAXPATH)) == NULL) {
		free(from_sub_path);
		errno = ENOMEM;
		return iRet;
	}
	if ((from_dir = opendir(from_path)) == NULL) {
		goto ERR;
	}
	if ((to_dir = opendir(to_path)) == NULL) {
		if (mkdir(to_path, 0770) == -1) {
			goto ERR_FROM;
		}
		if ((to_dir = opendir(to_path)) == NULL) {
			goto ERR_FROM;
		}
	}
	while ((ptr = readdir(from_dir))) {
		if (ptr->d_type == DT_DIR) {
			if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) {
				continue;
			}
			if (strlen(ptr->d_name) > DIR_MAXPATH) {
				errno = ENOMEM;
				goto ERR_TO;
			}
			memset(from_sub_path, 0, DIR_MAXPATH);
			sprintf(from_sub_path, "%s/%s", from_path, ptr->d_name);
			to_sub_path = (char *)malloc(DIR_MAXPATH);
			memset(to_sub_path, 0, DIR_MAXPATH);
			sprintf(to_sub_path, "%s/%s", to_path, ptr->d_name);
			if ((iRet = copy_dir(from_sub_path, to_sub_path)) == -1) {
				goto ERR_TO;
			}
			size += iRet;
		} else if (ptr->d_type == DT_REG) {
			from_sub_path = (char *)malloc(DIR_MAXPATH);
			memset(from_sub_path, 0, DIR_MAXPATH);
			sprintf(from_sub_path, "%s/%s", from_path, ptr->d_name);
			memset(to_sub_path, 0, DIR_MAXPATH);
			sprintf(to_sub_path, "%s/%s", to_path, ptr->d_name);
			if ((iRet = copy_file(from_sub_path, to_sub_path)) == -1) {
				goto ERR_TO;
			}
			size += iRet;
		} else {
			continue;
		}
	}
	iRet = size;
ERR_TO:
	closedir(to_dir);
ERR_FROM:
	closedir(from_dir);
ERR:
	free(to_sub_path);
	free(from_sub_path);
	return iRet;
}

