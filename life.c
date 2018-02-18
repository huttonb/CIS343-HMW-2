/*@Author: Bryce Hutton
 *@Date: 2/16/2018
 */
#include "file_utilities.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[]){
	int i;
	int c;
	char answer[2];
	char **arr;
	int row = atoi(argv[1]);
	int col = atoi(argv[2]);
	printf("Would you like to load a file?\nY/N?\n");
	scanf(" %c", &answer);
	if(answer[0] == 'Y'){
		loadFile(&arr, &row, &col);
	}
	//https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
	//i matches row, c matches col
	else if (answer[0] == 'N'){
		 arr = (char **)malloc(row * sizeof(char *));
		for(i = 0; i < row; i++){
			arr[i] = (char *)malloc(col * sizeof(char));
	}
		for(i = 0; i < row; i++){
			for(c = 0; c < col; c++){
				(arr)[i][c] = '0';
			}	
		}
	}
	else{
		printf("goodbye!");
		return 3;
	}
	turn(&arr, row, col);
	free(arr);
	return 0;
}


/*loadFile takes a pointer to a 2d array, and fills the 2d array based off of
 * a long string from the output of read_file.
 * */

int loadFile(char ***buffer, int* row, int* col){
	char *tempArr2;
	char **tempArr = &tempArr2;
	const int firstChar = 4;
	int i;
	int c;
	int missedChars = 0;
	char *cleanArr;
	read_file("saveFile.dat", tempArr);
	//Format of saved file is first char is row, second is \n, third is col
	//Max grid size is 9
	(*row) = (*tempArr)[0] - '0';
	(*col) = (*tempArr)[2] - '0';
	//Allocate the memory based off of the size of the rows and the columns,
	//plus the amount of new lines.
	cleanArr = (char*)malloc((*row)*(*col) * sizeof(char));	
	char **arr2 = (char **)malloc(*row * sizeof(char *));
	(*buffer) = arr2;
	for(i = 0; i < *row; i++)	
			(*buffer)[i] = (char *)malloc(*col * sizeof(char));

	
	//Remove newLines from the long string from read_file so that it can easily
	//parse the line into a 2d array.
	for(i = 4; i < ((*row)*(*col)+(*row)+4); i++){
		if((*tempArr)[i] != '\n')
			cleanArr[i-missedChars-4] = (*tempArr)[i];
		else
			missedChars++;
	}

	(*tempArr) = cleanArr;
	//Parses the long string into a 2d array
	for(i = 0; i < (*row); i++){
		for(c = 0; c < (*col); c++){
			(*buffer)[i][c] = (*tempArr)[(i*(*row))+c];
		}
	}

	return 0;



}

/*Iterate generation is the game logic, it's called everytime "life" moves forward a step.
 */
int iterateGeneration(char ***buffer, int row, int col){
	int cell;
	int i;
	int c;
	char buffTemp[row][col];
	//This forloop goes through each spot in the array,
	//then sends it to numNeighbors to find the amount of neighbors it has.
	//After it has the amount of neighbors, it kills the cell or lets it live.
	//If the cell is not going to change, it changes it's spot in the the array to X.
	for(i = 0; i < row; i++){
		for(c = 0; c < col; c++){
			cell = numNeighbors(buffer, i, c, row, col);
			if(cell < 2)
				(buffTemp)[i][c] = '0';
			else if (cell > 3)
				(buffTemp)[i][c] = '0';
			else if(cell == 3)
				(buffTemp)[i][c] = '1';
			else
				buffTemp[i][c] = 'x';
		}
	}
	//This loop goes through the process of replacing cells with the
	//dead or alive versions. If the cell is X, it doesn't change.
	for(i = 0; i <row; i++){
		for(c = 0; c < col; c++){
			if(buffTemp[i][c] != 'x')
				(*buffer)[i][c] = buffTemp[i][c];
		}
	}
	return 0;
}

/*Turn is the game logic that dictates each round. It asks for an action,
 * Then going off that action it uses other functions.
 */
int turn(char ***buffer, int row, int col){
	char answer[2];
	int loop = 1;
	int i;
	iterateGeneration(buffer, row, col);	
	printCells(buffer, row, col);

	printf("\nPress S to save, L to load.");
	printf("\nPress X to continue one generation.");
	printf("\nPress V to enter a number of generations.");
	printf("\nPress Q to quit.\n \n");
	//This while loop keeps the game going while it waits for an action
	//to be put in.
	//S will save the game,
	//L will load a game.
	//X calls function iterateGeneration once
	//Q quits the loop and returns to main function, where it frees and ends.
	//V calls iterateGeneration a set number of times
	//Anything else keeps the loop going, but if any option was chosen the loop stops.
	while(loop != 0){
		scanf(" %c", answer);

		loop = 0;
		if(answer[0] == 'S'){
			saveGame(buffer, row, col);
			printf("Saving game...");
			}
		else if(answer[0] == 'L'){
			printf("Loading game...");
			free(*buffer);
			loadFile(buffer, &row, &col);
			turn(buffer, row, col);
		}
		else if(answer[0] == 'X'){
			printf("Continuing one generation...");
			turn(buffer, row, col);
			}
		else if(answer[0] == 'Q'){
			loop = 0;
			printf("Exiting...\n");
			return 1;}
		else if(answer[0] == 'V'){
			printf("\nEnter number of generations:\n");
			scanf(" %c", answer);
			int iterations = atoi(answer);
			for(i = 0; i < iterations; i++){
				iterateGeneration(buffer, row, col);
			}
			turn(buffer, row, col);
		}
		else
			loop = 1;
	}
	
	return 0;
}

/*Print cells is the graphical interface, it prints the current board.
 */
int printCells(char ***buffer, int row, int col){
	int i;
	int c;
	printf("\n \nCell Board: \n \n");
	//This forloop iterates throughout the board and prints it.
	for(i = 0; i < row; i++){
		for(c = 0; c < col; c++){
			printf("%c", (*buffer)[i][c]);

		}
		printf("\n");
	}
	printf("\n \n");
	return 0;
}


/*Savegame takes the buffer and translates it into a single array
 *that can be passed to write_file in order to save.
 *Note that this only works with grids less than 9
 */
int saveGame(char ***buffer, int row, int col){
	int i;
	int c;
	int newLines = 4;
	int charSize = (col * row) + (row-1) + 4;
	char saveArr[charSize];
	int lineSpot;
	int charSpot;
	
	saveArr[0]=col+'0';
	saveArr[1]='\n';
	saveArr[2]=row+'0';
	saveArr[3]='\n';
	//This forloop iterates throughout the array, adding
	//the characters from the board. charSpot skips every fourth
	//number, which is later input with lineSpot.
	for(i = 0; i < row; i++){
		for(c = 0; c < col; c++){
			charSpot = ((i*row)+c+newLines);
			saveArr[charSpot] = (*buffer)[i][c];
		}
		lineSpot = ((i+1)*row)+c+(newLines-row);
		saveArr[lineSpot] = '\n';
		newLines++;
	}
	write_file("saveFile.dat", saveArr, charSize);
	return 0;
}
/*numNeibhors has several if statements that check the surrounding spots for
 * any live cells, then adds them to a counter which it returns as the number
 * of surrounding cells.
 */
int numNeighbors(char ***buffer, int i, int c, int row, int col){
	int counter = 0;
	if(i < row-1){
		if((*buffer)[i+1][c] != '0')
			counter++;
		if(c < col-1){
			if((*buffer)[i+1][c+1] != '0')
				counter++;
		}
		if(c > 0){
			if((*buffer)[i+1][c-1] != '0')
				counter++;
			}
	}
	if(i > 0){
		if((*buffer)[i-1][c] != '0')
			counter++; 
		if(c > 0){
			if((*buffer)[i-1][c-1] != '0')
				counter++; 
		}
		if(c < col-1){
			if((*buffer)[i-1][c+1] != '0')
				counter++; 
		}
	}
	if(c < col-1){
		if((*buffer)[i][c+1] != '0')
			counter++; 
	}
	if(c > 0){
		if((*buffer)[i][c-1] != '0')
			counter++;
	}	
	return counter;
}
