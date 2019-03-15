//Lab 3
//Group 8

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define COUNT 9
#define CHECK 362880

pthread_t threads[4];

//Global Variables
int sudoku[COUNT][COUNT];
int temp[3];
int a, b;

//Solves the sudoku
int *solve(){
	int thread = 1;
	for(int i = 0; i < 9; i++){
		for( int j = 0; j < 9; j++){
			if(sudoku[i][j] == 0){
				for(int k = 1; k <= 9; k++){
					a = i;
					b = j;
					if(isAllowed(k)){
						sudoku[i][j] = k;
						if(solve()){
							return 1;
						}
						else{
							sudoku[i][j] = 0;
						}
					}
				}
				return 0;	
			}
		}
	}
	return 1;
}

//Reads in the file from puzzle.txt
void *readFile(){
	char ch;
	FILE *fp;
	fp = fopen("puzzle.txt", "r");

	if (fp == NULL){
		perror("Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

	for(int i = 0; i < COUNT; i++){
		for(int j = 0; j < COUNT; j++){
			fscanf(fp, "%d", &sudoku[i][j]);
		}
	}

	fclose(fp);
}

//Saves file in output.txt
void *saveFile(){
	FILE *g = fopen("output.txt", "w");
	for(int i = 0; i < COUNT; i++){
		for(int j = 0; j < COUNT; j++){
			fprintf(g, "%d ", sudoku[i][j]);
		}
		fprintf(g, "\n");
	}
	fclose(g);
}

//Checks if row is valid
int *checkRow(int num){
	int total = 1;
	for(int i = 0; i < 9; i++){
		total *= sudoku[a][i];
		if(num == sudoku[a][i]){
			temp[0] = 0;
			return 0;
		}
	}
	temp[0] = 1;
	return 1;
}

//Checks if col is valid
int *checkCol(int num){
	int total = 1;
	for(int i = 0; i < 9; i++){
		total *= sudoku[i][b];
		if(num == sudoku[i][b]){
			temp[1] = 0;
			return 0;
		}
	}
	temp[1] = 1;
	return 1;
}

//Checks if square is valid
int *checkSq(int num){

	int iFixed;
	int jFixed;

	if(a <= 2){
		iFixed = 0;
	}
	else if(a <=5) {
		iFixed = 3;
	}
	else{
		iFixed = 6;
	}

	if(b <= 2){
		jFixed = 0;
	}
	else if(b<=5) {
		jFixed = 3;
	}
	else{
		jFixed = 6;
	}

	int total = 1;
	for(int i = 0; i < 3; i++){
		for(int j = 0; j < 3; j++){
			total *= sudoku[i+iFixed][j+jFixed];
			if(num == sudoku[i+iFixed][j+jFixed]){
				temp[2] = 0;
				return 0;
			}
		}
	}
	temp[2] = 1;
	return 1;
}

//Calls three threads to check if each col, row and square that is affected by num is valid
int isAllowed(int num){	
	
	pthread_create(&threads[1], 0, &checkCol, num);
	pthread_create(&threads[2], 0, &checkRow, num);
	pthread_create(&threads[3], 0, &checkSq, num);

	pthread_join(threads[1], 0);
	pthread_join(threads[2], 0);
	pthread_join(threads[3], 0);
	
	if(temp[0] && temp[1] && temp[2]){
		return 1;
	}
	return 0;
}

//Main function, clock is used to time the duration
int main(int argc, char *argv[]){

	pthread_create(&threads[0], 0, readFile, (void *) NULL);
	pthread_join(threads[0], 0);

	

	clock_t begin = clock();
	solve();

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("%f\n", time_spent);

	pthread_create(&threads[0], 0, saveFile, (void *) NULL);
	pthread_join(threads[0], 0);
}