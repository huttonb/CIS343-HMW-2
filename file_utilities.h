#ifndef			__H_MY_LIBRARY__
#define			__H_MY_LIBRARY__

int read_file(char* filename, char **buffer);
int write_file(char* filename, char *buffer, int size);
int file_size(const char *filename);

#endif
