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

int row(int pos, int len) { //find current row from integer
	return (pos-(pos % ((int)sqrt(len))))/((int)sqrt(len));
}
int column(int pos, int len) { //find current column from integer
	return (pos % ((int)sqrt(len)));
}
int conflict(int pos1, int pos2, int len) { //see if there is a conflict
	if (pos2 == -1) return 0; //no conflict by default
	if (row(pos1,len) == row(pos2,len) || column(pos1,len) == column(pos2,len))
		return 1;
	int r1 = row(pos1, len);
	int r2 = row(pos2, len);
	int d_r = r2-r1; //delta row
	int col = column(pos1, len);
	int c1 = col - d_r; int c2 = col + d_r; //two conflicts
	if (column(pos2,len) == c1 || column(pos2, len) == c2) //check diagnal
		return 1;
	return 0; //0 = no conflict, 1 = conflict
}
int attacks(int qs[], int q_len) { //get pairs of attacking queens
	int attacking_pairs = 0;
	for (int i=0; i<q_len; i++)
		for (int j=i+1; j<q_len; j++)
			if (qs[i]!=-1 && qs[j]!=-1 && conflict(qs[i],qs[j],q_len*q_len)==1)
				attacking_pairs++;
	return attacking_pairs;
}
void print_board(int qs[], int qsz, int len) {
	for (int i=0; i<((int)sqrt(len)); i++) { //loop through all rows
		int j_b=i*sqrt(len); //start
		int j_e=j_b+sqrt(len); //end
		for (j_b; j_b<j_e; j_b++) { //loop through current row (columns)
			int found = 0; //1 = found
			int n=0;
			for (n; n<qsz; n++) {
				if (qs[n] == j_b) { //if match
					found = 1;
					printf(" \e[0;32m");
					printf("q%d",n);
					printf("\e[0m");
				}
			}
			if (found == 0) {
				if (j_b < 10) printf(" ");
				printf(" %d",j_b);
			}
		}
		printf("\n"); //add a newline at end of row
	}
}
void brute_force(int nqueens, int bd_len) {
	printf("Brute force:\n");
	int q[nqueens];
	int rin = 1; //row increment
	int row = 0;
	for (int a=0;a<nqueens;a++) q[a] = -1;
	while (rin != 0) {
		int safes[] = {}; //safe moves
		int sf_len = 0;
		int strow = row*sqrt(bd_len);
		int ndrow = strow+sqrt(bd_len); //end of row
		for (int p1=q[row]+1;p1<ndrow;p1++) { //get right safe moves
			bool intersects = false;
			for (int nq=row-1;nq>=0;nq--)
				if (conflict(q[nq],p1,bd_len)==1) {
					intersects = true; break;
				}
			if (!intersects) {
				safes[sf_len] = p1;
				sf_len++;
			}
		}
		if (sf_len == 0) {
			if (row == 0) break; //quit if first row is no more
			rin = q[row] = -1;
		} else if (row == sqrt(bd_len)-1) {
			rin = 0;
			q[row] = safes[0];
			//printf("Done, solved!\n");
		} else if (sf_len > 0) {
			q[row] = safes[0];
			rin = 1;
		}
		//printf("Row increment is %d.\n",rin);
		//printf("Row is currently %d, and safe length is %d.\n",row, sf_len);
		row += rin;
	}
	print_board (q, nqueens, bd_len);
}
void hill_climb(int nqueens, int bd_len) {
	printf("Hill climbing:\n");
	int q[nqueens];
	srand(time(NULL));
	for (int i=0; i<nqueens; i++) q[i] = random() % bd_len;
	int qin = 1;
	int nq = 0;
	while (qin != 0) { //make gradual improvements to fitness
		if (attacks(q, nqueens)==0) qin = 0; //we're done
		else {
			int oldpos = q[nq];
			int newpos = random() % bd_len;
			int oldattacks = attacks(q, nqueens);
			q[nq] = newpos;
			int newattacks = attacks(q, nqueens);
			if (newattacks > oldattacks) q[nq] = oldpos; //revert if fitness reduces
			if (nq == 0) qin = 1;
			else if (nq == nqueens-1) qin = -1;
		}
		nq += qin;
	}
	print_board (q, nqueens, bd_len);
}
void stochastic_beam_search(int nqueens, int bd_len) {
	printf("Stochastic beam search:\n");
	int q[nqueens];
	srand(time(NULL));
	for (int i=0; i<nqueens; i++) q[i] = random() % bd_len;
	int qin = 1;
	int qn = 0;
	int K = 5;
	while (qin != 0) { //generate and choose K states; improve fitness
		if (attacks(q, nqueens)==0) qin = 0;
		else {
			int sts[K]; //states
			int fits[K]; //corresponding fitness
			int oldpos = q[qn];
			int totalfit = 0;
			for (int i=0; i<K; i++) { //generate distribution
				sts[i] = random() % bd_len;
				q[qn] = sts[i];
				fits[i] = 28-attacks(q, nqueens);
				totalfit += fits[i];
			}
			q[qn] = oldpos;
			int r_fit = random() % totalfit;
			int lower_bound = 0;
			int newpos;
			for (int i=0; i<K; i++) { //select from distribution
				for (int j=0; j<i; j++)
					lower_bound += fits[j];
				int upper_bound = lower_bound+fits[i];
				if (r_fit >= lower_bound && r_fit < upper_bound) { //our selection
					newpos = sts[i];
					break;
				}
			}
			int oldattacks = attacks(q, nqueens);
			q[qn] = newpos;
			int newattacks = attacks(q, nqueens);
			if (newattacks > oldattacks) //revert if fitness reduces
				q[qn] = oldpos;
			if (qn == 0) qin = 1;
			else if (qn == nqueens-1) qin = -1;
		}
		qn += qin;
	}
	print_board (q, nqueens, bd_len);
}
void simulated_annealing(int nqueens, int bd_len) {
	printf("Simulated annealing:\n");
	int q[nqueens];
	srand(time(NULL));
	for (int i=0; i<nqueens; i++) q[i] = random() % bd_len;
	int qin = 1;
	int qn = 0;
	double orig_T = 4000;
	double T = orig_T;
	double cool_amt = 1.01; //value must be greater than 1
	while (qin != 0) {
		if (attacks(q, nqueens)==0) qin = 0;
		else {
			T = T/cool_amt;
			int oldattacks = orig_T*attacks(q, nqueens)/28;
			int oldpos = q[qn];
			int newpos = random() % bd_len;
			q[qn] = newpos;
			int newattacks = orig_T*attacks(q, nqueens)/28;
			double u = (random() % 100001)/100000; //between 0 and 1
			if (u >= exp(-(newattacks-oldattacks)/T)) //revert if not satisfiable
				q[qn] = oldpos;
			if (qn == 0) qin = 1;
			else if (qn == nqueens-1) qin = -1;
		}
		qn += qin;
	}
	print_board (q, nqueens, bd_len);
}
void genetic_algorithm(int nqueens, int bd_len) {
	printf("Genetic algorithm:\n");
	int q[nqueens];
	srand(time(NULL));
	for (int i=0; i<nqueens; i++) q[i] = random() % bd_len;
	int K = 8; //population
	double pM = 0.2; //probability of mutation
	int pop[K][nqueens]; //population
	int offspr[K][nqueens]; //offspring
	for (int i=0; i<K; i++)
		for (int j=0; j<nqueens; j++)
			pop[i][j] = j*sqrt(bd_len)+(random() % (int)sqrt(bd_len));
	while (attacks(q, nqueens)!=0) { //repopulate over and over
		for (int i=0; i<K; i+=2) { //produce offspring
			int splt = 1+random()%(nqueens-1);
			for (int j=0; j<splt; j++)
				offspr[i/2][j] = pop[i][j]; //genes from parent 1
			for (int j=splt; j<nqueens; j++)
				offspr[i/2][j] = pop[i][j]; //genes from parent 2
			double mC = (random()%100000)/100000; //probability of mutation
			int rnd_qn = random()%nqueens; //mutate:
			if (mC <= pM) offspr[i/2][rnd_qn] = rnd_qn*sqrt(bd_len)+(random() % (int)sqrt(bd_len));
		}
		for (int i=0; i<K; i++) //set as new population
			for (int j=0; j<nqueens; j++)
				pop[i][j] = offspr[i][j];
		bool better = true;
		int b_i = 0;
		while (better) { //get best candidate
			better = false;
			for (int i=0; i<K; i++)
				if (i!=b_i && attacks(pop[i], nqueens) < attacks(pop[b_i], nqueens)) {
					better = true;
					b_i = i;
				}
		}
		if (attacks(pop[b_i], nqueens) < attacks(q, nqueens)) 
			for (int i=0; i<nqueens; i++) //copy if improved fitness
				q[i] = pop[b_i][i];
	}
	print_board (q, nqueens, bd_len);
}
int main() {
	//strlen is the user-set board length
	int qns = 8;
	printf("In this demo, four separate functions are used to simulate different approaches to the N Queens problem.\n");
	printf("In the below scenario, %d queens are placed on an %dx%d board.\n",qns,qns,qns);
	brute_force(qns, qns*qns); //place n queens on a n^2-tile board
	hill_climb(qns, qns*qns);
	stochastic_beam_search(qns, qns*qns);
	simulated_annealing(qns, qns*qns);
	genetic_algorithm(qns, qns*qns);
	return 0;
}