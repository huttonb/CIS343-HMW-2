/*@Author: Bryce Hutton
 *@Date: 2/16/2018
 */
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include "file_utilities.h"

/*read_file takes a filename, as well as a pointer to a string,
 * it then reads a file and puts the contents of the file into the string.
 */
int read_file(char* filename, char **buffer){
	FILE *fp;
	int i;

	fp = fopen(filename, "r");
	if (fp == NULL){
		printf("File could not be opened");
		return -1;
	}
	//http://www.zentut.com/c-tutorial/c-read-text-file/
	int size = file_size(filename);
	//https://stackoverflow.com/questions/238603/how-can-i-get-a-files-size-in-c
	(*buffer) = (char*)malloc(size * sizeof(char));	
	for(i = 0; i < size; i++){
		(*buffer)[i] = fgetc(fp);		
	}
	fclose(fp);
	return 0;
}

/*write_file takes a size, a string, and a file name, then writes the
 * string to the filename.
 */
int write_file(char* filename, char *buffer, int size){
	FILE *fp;
	fp = fopen(filename, "w");
	fputs(buffer, fp);
	fclose(fp);
	return 0;
}

//file_size takes the pointer to the file name, declares it as a constant,
//And finds the size of the file in Bytes.
//Code is from :https://stackoverflow.com/questions/8236/how-do-you-determine-the-size-of-a-file-in-c
//With minor modifications.
int file_size(const char *fileName){
	struct stat st;

	if(stat(fileName, &st) == 0)
		return st.st_size;
	return -1;
}

