/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2012 Princeton Ferro <princetonf@optonline.net>
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int row(int pos, int len) { //find current row from integer
	return (pos-(pos % ((int)sqrt(len))))/((int)sqrt(len));
}

int column(int pos, int len) { //find current column from integer
	return (pos % ((int)sqrt(len)));
}

int conflict(int pos1, int pos2, int len) { //see if there is a conflict
	if (row(pos1,len) == row(pos2,len) || column(pos1,len) == column(pos2,len))
		return 1;
	int row_width = (int)sqrt(len);
	
	return 0; //0 = no conflict, 1 = conflict
}

void hill_climb(int board[], int queens, int len) {
	int q[queens];
	/* Above is the list of queen positions, which follows in format:
	 * q[n] = pos, where n = number of queen and pos = position on board
	 */
	q[0] = 0;
	int row = 1;
	for (row; row<((int)sqrt(len)); row++) { //iterate down through rows
		int a = row*sqrt(len); //starting point
		int b = a+sqrt(len); //ending point
		for (a; a<b; a++) {
			
		}
	}
}

int main(int strlen, char *args[]) {
	//strlen is the user-set board length
	printf("In this demo, four separate functions are used to simulate different approaches to the N Queens problem.\n");
	int len = 64;
	int p = 8;
	return 0;
}
