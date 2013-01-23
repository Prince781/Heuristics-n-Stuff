/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * main.c
 * Copyright (C) 2012 Princeton Ferro <princetonf@optonline.net>
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

typedef struct input {
	double z; //desired output
	double *x; //x list
} input;

void print_weights(double w[], int wln) {
	for (int i=0; i<wln; i++)
		printf("weight[%d] is %f.\n",i,(float)w[i]);
}

void print_parts(input y[], int Pl, int nl) {
	for (int i=0; i<Pl; i++) {
		printf("Z: %e, Y: [",y[i].z);
		printf("%e,",y[i].x[0]);
		for (int j=1; j<nl-1; j++)
			printf(" %e,",y[i].x[j]);
		printf(" %e]\n",y[i].x[nl-1]);
	}
}

void perceptron(int n, int P) { //n = dimensions, P = patterns
	double b, r, t; //bias, learning rate, threshold
	t = 0.8;
	r = 0.1;
	b = 0;
	
	double weights[n];
	input training_set[P]; //training_set is y
	srand(time(NULL));
	for (int i=0; i<P; i++) {
		training_set[i].x = malloc(n*sizeof(double));
		for (int j=0; j<n; j++) {
			training_set[i].x[j] = (double)(random()%2);
			training_set[i].z = (double)(random()%2);
			weights[j] = 0;
		}
	}
	bool error = true;
	while (error) {
		for (int i=0; i<P; i++) {
			double sum = 0;
			for (int j=0; j<n; j++)
				sum += training_set[i].x[j]*weights[j];
			int ns = 0;
			if (sum > t) ns = 1;
			int err = (int)training_set[i].z - ns; //-1, 0, or 1
			double d = r*err; //correction factor
			for (int j=0; j<n; j++)
				weights[j] += training_set[i].x[j]*d+b;
		}
		double wsum = 0;
		for (int j=0; j<n; j++)
			wsum += weights[j];
		printf("Weight sum is %f.\n",(float)wsum);
		if (wsum >= t) error = false;
	}
	print_weights (weights, n);
	print_parts(training_set, P, n);
}

int main() {
	perceptron(3,10);
	return 0;
}
